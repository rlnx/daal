load("@onedal//build/bazel:utils.bzl", "unique")

load(
    "@bazel_tools//tools/cpp:lib_cc_configure.bzl",
    "auto_configure_fail",
    "auto_configure_warning",
    "get_starlark_list",
    "write_builtin_include_directory_paths",
)

_TEST_CPP_FILE = "empty.cpp"
_INC_DIR_MARKER_BEGIN = "#include <...>"
_INC_DIR_MARKER_END = "End of search list"

# OSX add " (framework directory)" at the end of line, strip it.
_MAC_FRAMEWORK_SUFFIX = " (framework directory)"
_MAC_FRAMEWORK_SUFFIX_LEN = len(_MAC_FRAMEWORK_SUFFIX)

def _prepare_include_path(repo_ctx, path):
    """Resolve and sanitize include path before outputting it into the crosstool.
    Args:
      repo_ctx: repository_ctx object.
      path: an include path to be sanitized.
    Returns:
      Sanitized include path that can be written to the crosstoot. Resulting path
      is absolute if it is outside the repository and relative otherwise.
    """
    path = path.strip()
    if path.endswith(_MAC_FRAMEWORK_SUFFIX):
        path = path[:-_MAC_FRAMEWORK_SUFFIX_LEN].strip()

    # We're on UNIX, so the path delimiter is '/'.
    repo_root = str(repo_ctx.path(".")) + "/"
    path = str(repo_ctx.path(path))
    if path.startswith(repo_root):
        return path[len(repo_root):]
    return path

def _get_cxx_inc_directories(repo_ctx, cc, lang_flag, additional_flags = []):
    """Compute the list of default C++ include directories."""
    result = repo_ctx.execute([cc, "-E", lang_flag, "-", "-v"] + additional_flags)
    index1 = result.stderr.rfind(_INC_DIR_MARKER_BEGIN)
    index2 = result.stderr.rfind(_INC_DIR_MARKER_END)
    inc_dirs = result.stderr[index1:index2].strip()
    return [ _prepare_include_path(repo_ctx, p) for p in inc_dirs.split("\n") ]


def _is_compiler_option_supported(repo_ctx, cc, option):
    """Checks that `option` is supported by the C compiler."""
    result = repo_ctx.execute([
        cc,
        option,
        "-o",
        "/dev/null",
        "-c",
        str(repo_ctx.path(_TEST_CPP_FILE)),
    ])
    return result.stderr.find(option) == -1


def _is_linker_option_supported(repo_ctx, cc, option, pattern):
    """Checks that `option` is supported by the C linker."""
    result = repo_ctx.execute([
        cc,
        option,
        "-o",
        "/dev/null",
        str(repo_ctx.path(_TEST_CPP_FILE)),
    ])
    return result.stderr.find(pattern) == -1


def _find_gold_linker_path(repo_ctx, cc):
    """Checks if `gold` is supported by the C compiler.

    Args:
      repo_ctx: repo_ctx.
      cc: path to the C compiler.

    Returns:
      String to put as value to -fuse-ld= flag, or None if gold couldn't be found.
    """
    result = repo_ctx.execute([
        cc,
        str(repo_ctx.path(_TEST_CPP_FILE)),
        "-o",
        "/dev/null",
        # Some macos clang versions don't fail when setting -fuse-ld=gold, adding
        # these lines to force it to. This also means that we will not detect
        # gold when only a very old (year 2010 and older) is present.
        "-Wl,--start-lib",
        "-Wl,--end-lib",
        "-fuse-ld=gold",
        "-v",
    ])
    if result.return_code != 0:
        return None

    for line in result.stderr.splitlines():
        if line.find("gold") == -1:
            continue
        for flag in line.split(" "):
            if flag.find("gold") == -1:
                continue
            if flag.find("--enable-gold") > -1 or flag.find("--with-plugin-ld") > -1:
                # skip build configuration options of gcc itself
                continue

            # flag is '-fuse-ld=gold' for GCC or "/usr/lib/ld.gold" for Clang
            # strip space, single quote, and double quotes
            flag = flag.strip(" \"'")

            # remove -fuse-ld= from GCC output so we have only the flag value part
            flag = flag.replace("-fuse-ld=", "")
            return flag
    auto_configure_warning(
        "CC with -fuse-ld=gold returned 0, but its -v output " +
        "didn't contain 'gold', falling back to the default linker.",
    )
    return None


def _add_compiler_option_if_supported(repo_ctx, cc, option):
    """Returns `[option]` if supported, `[]` otherwise."""
    return [option] if _is_compiler_option_supported(repo_ctx, cc, option) else []


def _add_linker_option_if_supported(repo_ctx, cc, option, pattern):
    """Returns `[option]` if supported, `[]` otherwise."""
    return [option] if _is_linker_option_supported(repo_ctx, cc, option, pattern) else []


def _get_no_canonical_prefixes_opt(repo_ctx, cc):
    # If the compiler sometimes rewrites paths in the .d files without symlinks
    # (ie when they're shorter), it confuses Bazel's logic for verifying all
    # #included header files are listed as inputs to the action.

    # The '-fno-canonical-system-headers' should be enough, but clang does not
    # support it, so we also try '-no-canonical-prefixes' if first option does
    # not work.
    opt = _add_compiler_option_if_supported(
        repo_ctx,
        cc,
        "-fno-canonical-system-headers",
    )
    if len(opt) == 0:
        return _add_compiler_option_if_supported(
            repo_ctx,
            cc,
            "-no-canonical-prefixes",
        )
    return opt


def _get_coverage_flags(repo_ctx, reqs):
    if reqs.compiler_id == "clang":
        compile_flags = '"-fprofile-instr-generate",  "-fcoverage-mapping"'
        link_flags = '"-fprofile-instr-generate"'
    else:
        # gcc requires --coverage being passed for compilation and linking
        # https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#Instrumentation-Options
        compile_flags = '"--coverage"'
        link_flags = '"--coverage"'
    return compile_flags, link_flags


def _find_tool(repo_ctx, tool_name, mandatory=False):
    if tool_name.startswith("/"):
        return tool_name
    tool_path = repo_ctx.which(tool_name)
    if not tool_path:
        if mandatory:
            auto_configure_fail("Cannot find {}; try to correct your $PATH".format(tool_name))
        else:
            repo_ctx.template(
                "tool_not_found_wrapper.sh",
                Label("@onedal//build/bazel/toolchains:tool_not_found.sh.tpl"),
                { "%{tool_name}": tool_name }
            )
            tool_path = "tool_not_found.sh"
    return str(tool_path)


def _try_wrap_compiler(repo_ctx, reqs, wrapper_name, compiler_path):
    if reqs.os_id == "mac":
        wrapper_path = "{}.sh".format(wrapper_name)
        wrapper_src = Label("@bazel_tools//tools/cpp:osx_cc_wrapper.sh.tpl")
        # TODO: Extra environment variables may be required for
        #       some compilers, no need for gcc/clang.
        env = ""
        repo_ctx.template(
            wrapper_path,
            wrapper_src,
            {
                "%{cc}": compiler_path,
                "%{env}": env,
            }
        )
        return wrapper_path


def _find_tools(repo_ctx, reqs):
    ar_tool_name = {
        "mac": "libtool",
        "lnx": "ar",
    }[reqs.os_id]
    # TODO: Use full compiler path from reqs
    compiler_name = reqs.compiler_id
    cpp_compiler_name = {
        "gcc": "g++",
        "clang": "clang++",
    }[reqs.compiler_id]
    dpcpp_compiler_name = reqs.compiler_dpcpp_id
    # { <key>: ( <tool_name>, <mandatory_flag> ) }
    tools_meta = {
        "ar":      ( ar_tool_name,        True  ),
        "ld":      ( "ld",                True  ),
        "gcc":     ( compiler_name,       True  ),
        "cpp":     ( cpp_compiler_name,   True  ),
        "dpcc":    ( dpcpp_compiler_name, False ),
        "dwp":     ( "dwp",               False ),
        "gcov":    ( "gcov",              True  ),
        "nm":      ( "nm",                True  ),
        "objcopy": ( "objcopy",           False ),
        "objdump": ( "objdump",           True  ),
        "strip":   ( "strip",             True  ),
    }
    tool_paths = {
        key: _find_tool(repo_ctx, *meta)
        for key, meta in tools_meta.items()
    }
    return struct(
        cc = tool_paths["gcc"],
        dpcc = tool_paths["dpcc"],
        paths = tool_paths,
        cc_wrapped = _try_wrap_compiler(repo_ctx, reqs, "cc_wrapper", tool_paths["gcc"]),
        cpp_wrapped = _try_wrap_compiler(repo_ctx, reqs, "cpp_wrapper", tool_paths["cpp"]),
        dpcc_wrapped = _try_wrap_compiler(repo_ctx, reqs, "dpcc_wrapper", tool_paths["dpcc"]),
    )


def _preapre_builtin_include_directory_paths(repo_ctx, reqs, tools, cxx_opts):
    builtin_include_directories = unique(
        _get_cxx_inc_directories(repo_ctx, tools.cc, "-xc") +
        _get_cxx_inc_directories(repo_ctx, tools.cc, "-xc++", cxx_opts) +
        _get_cxx_inc_directories(
            repo_ctx,
            tools.dpcc,
            "-xc++",
            cxx_opts + [
                "--gcc-toolchain={}".format(_get_gcc_toolchain_path(repo_ctx, tools)),
            ],
        ) +
        _get_cxx_inc_directories(
            repo_ctx,
            tools.cc,
            "-xc",
            _get_no_canonical_prefixes_opt(repo_ctx, tools.cc),
        ) +
        _get_cxx_inc_directories(
            repo_ctx,
            tools.cc,
            "-xc++",
            cxx_opts + _get_no_canonical_prefixes_opt(repo_ctx, tools.cc),
        )
    )
    write_builtin_include_directory_paths(repo_ctx, tools.cc, builtin_include_directories)
    return builtin_include_directories


def _add_tools_deps(tools):
    deps = [
        tools.cc_wrapped,
        tools.cpp_wrapped,
        tools.dpcc_wrapped,
    ]
    return [x for x in deps if x]


def _get_tool_paths(tools):
    tool_paths = {k: v for k, v in tools.paths.items()}
    if tools.cc_wrapped:   tool_paths["gcc"] = tools.cc_wrapped
    if tools.cpp_wrapped:  tool_paths["cpp"] = tools.cpp_wrapped
    if tools.dpcc_wrapped: tool_paths["dpcc"] = tools.dpcc_wrapped
    return tool_paths


def _get_toolchain_identifier(reqs):
    return "{}-{}-{}-{}".format(reqs.os_id, reqs.target_arch_id,
                                reqs.compiler_id, reqs.compiler_version)


def _get_bin_search_flag(repo_ctx, cc_path):
    cc_path = repo_ctx.path(cc_path)
    if not str(cc_path).startswith(str(repo_ctx.path(".")) + "/"):
        # cc is outside the repository, set -B
        bin_search_flag = ["-B" + str(cc_path.dirname)]
    else:
        # cc is inside the repository, don't set -B.
        bin_search_flag = []
    return bin_search_flag


def _get_gcc_toolchain_path(repo_ctx, tools):
    return str(repo_ctx.path(tools.cc).dirname.dirname.realpath)


def configure_cc_toolchain_unix(repo_ctx, reqs):
    if reqs.os_id not in ["lnx", "mac"]:
        auto_configure_fail("Cannot configure Unix toolchain on '{}', " +
                            "only Linux and Mac supported".format(reqs.os_id))
    is_mac = reqs.os_id == "mac"
    repo_ctx.file(_TEST_CPP_FILE, "int main() { return 0; }")

    # Default compilations/link options
    cxx_opts  = [ "-std=c++17" ]
    link_opts = [ ]
    link_libs = [ "-lstdc++", "-lm" ]

    # Paths to tools/compiler includes
    tools = _find_tools(repo_ctx, reqs)
    gold_linker_path = _find_gold_linker_path(repo_ctx, tools.cc)
    builtin_include_directories = _preapre_builtin_include_directory_paths(
        repo_ctx, reqs, tools, cxx_opts)

    # Addition compile/link flags
    bin_search_flag_cc = _get_bin_search_flag(repo_ctx, tools.cc)
    bin_search_flag_dpcc = _get_bin_search_flag(repo_ctx, tools.dpcc)
    coverage_compile_flags, coverage_link_flags = _get_coverage_flags(repo_ctx, reqs)

    repo_ctx.template(
        "BUILD",
        Label("@onedal//build/bazel/toolchains:BUILD_cc_toolchain_def_unix.tpl"),
        {
            # Various IDs
            "%{cc_toolchain_identifier}": _get_toolchain_identifier(reqs),
            "%{compiler}":                reqs.compiler_id + "-" + reqs.compiler_version,
            "%{abi_version}":             reqs.compiler_abi_version,
            "%{abi_libc_version}":        reqs.libc_abi_version,
            "%{target_libc}":             reqs.libc_version,
            "%{target_cpu}":              reqs.target_arch_id,
            "%{host_system_name}":        reqs.os_id + "-" + reqs.host_arch_id,
            "%{target_system_name}":      reqs.os_id + "-" + reqs.target_arch_id,

            "%{supports_param_files}": "0" if is_mac else "1",
            "%{cc_compiler_deps}": get_starlark_list(
                [":builtin_include_directory_paths"] +
                _add_tools_deps(tools)
            ),
            "%{tool_paths}": ",\n        ".join(
                ['"%s": "%s"' % (k, v) for k, v in _get_tool_paths(tools).items()],
            ),
            "%{gcc_toolchain_path}": "/export/users/cache/gcc/gnu_9.1.0",
            "%{cxx_builtin_include_directories}": get_starlark_list(builtin_include_directories),
            "%{compile_flags_cc}": get_starlark_list(
                [
                    # TODO: Add clang to clang if on Linux?
                    # "--gcc-toolchain=",

                    "-U_FORTIFY_SOURCE",
                    "-fstack-protector",
                    "-Wall",
                ] + (
                    _add_compiler_option_if_supported(repo_ctx, tools.cc, "-Wthread-safety") +
                    _add_compiler_option_if_supported(repo_ctx, tools.cc, "-Wself-assign")
                ) + (
                    # Disable problematic warnings.
                    _add_compiler_option_if_supported(repo_ctx, tools.cc, "-Wunused-but-set-parameter") +
                    # has false positives
                    _add_compiler_option_if_supported(repo_ctx, tools.cc, "-Wno-free-nonheap-object") +
                    # Enable coloring even if there's no attached terminal. Bazel removes the
                    # escape sequences if --nocolor is specified.
                    _add_compiler_option_if_supported(repo_ctx, tools.cc, "-fcolor-diagnostics")
                ) + [
                    # Keep stack frames for debugging, even in opt mode.
                    "-fno-omit-frame-pointer",
                ],
            ),
            "%{compile_flags_dpcc}": get_starlark_list(
                [
                    # TODO: Fix path to GCC toolchain
                    "--gcc-toolchain={}".format(_get_gcc_toolchain_path(repo_ctx, tools)),

                    "-U_FORTIFY_SOURCE",
                    "-fstack-protector",
                    "-Wall",
                ] + (
                    _add_compiler_option_if_supported(repo_ctx, tools.dpcc, "-Wthread-safety") +
                    _add_compiler_option_if_supported(repo_ctx, tools.dpcc, "-Wself-assign")
                ) + (
                    # Disable problematic warnings.
                    _add_compiler_option_if_supported(repo_ctx, tools.dpcc, "-Wunused-but-set-parameter") +
                    # has false positives
                    _add_compiler_option_if_supported(repo_ctx, tools.dpcc, "-Wno-free-nonheap-object") +
                    # Enable coloring even if there's no attached terminal. Bazel removes the
                    # escape sequences if --nocolor is specified.
                    _add_compiler_option_if_supported(repo_ctx, tools.dpcc, "-fcolor-diagnostics")
                ) + [
                    # Keep stack frames for debugging, even in opt mode.
                    "-fno-omit-frame-pointer",
                ],
            ),
            "%{cxx_flags}": get_starlark_list(cxx_opts),
            "%{link_flags_cc}": get_starlark_list((
                ["-fuse-ld=" + gold_linker_path] if gold_linker_path else []
            ) + _add_linker_option_if_supported(
                repo_ctx,
                tools.cc,
                "-Wl,-no-as-needed",
                "-no-as-needed",
            ) + _add_linker_option_if_supported(
                repo_ctx,
                tools.cc,
                "-Wl,-z,relro,-z,now",
                "-z",
            ) + (
                [
                    "-undefined",
                    "dynamic_lookup",
                    "-headerpad_max_install_names",
                ] if is_mac else bin_search_flag_cc + [
                    # Gold linker only? Can we enable this by default?
                    # "-Wl,--warn-execstack",
                    # "-Wl,--detect-odr-violations"
                ] + _add_compiler_option_if_supported(
                    # Have gcc return the exit code from ld.
                    repo_ctx,
                    tools.cc,
                    "-pass-exit-codes",
                )
            ) + link_opts),
            "%{link_flags_dpcc}": get_starlark_list(
                [
                    # TODO: Fix path to GCC toolchain
                    "--gcc-toolchain={}".format(_get_gcc_toolchain_path(repo_ctx, tools)),
                ] + (
                    ["-fuse-ld=" + gold_linker_path] if gold_linker_path else []
            ) + _add_linker_option_if_supported(
                repo_ctx,
                tools.dpcc,
                "-Wl,-no-as-needed",
                "-no-as-needed",
            ) + _add_linker_option_if_supported(
                repo_ctx,
                tools.dpcc,
                "-Wl,-z,relro,-z,now",
                "-z",
            ) + (
                [
                    "-undefined",
                    "dynamic_lookup",
                    "-headerpad_max_install_names",
                ] if is_mac else bin_search_flag_dpcc + [
                    # Gold linker only? Can we enable this by default?
                    # "-Wl,--warn-execstack",
                    # "-Wl,--detect-odr-violations"
                ] + _add_compiler_option_if_supported(
                    # Have gcc return the exit code from ld.
                    repo_ctx,
                    tools.dpcc,
                    "-pass-exit-codes",
                )
            ) + link_opts),
            "%{link_libs}": get_starlark_list(link_libs),
            "%{opt_compile_flags}": get_starlark_list(
                [
                    # No debug symbols.
                    # Maybe we should enable https://gcc.gnu.org/wiki/DebugFission for opt or
                    # even generally? However, that can't happen here, as it requires special
                    # handling in Bazel.
                    "-g0",

                    # Conservative choice for -O
                    # -O3 can increase binary size and even slow down the resulting binaries.
                    # Profile first and / or use FDO if you need better performance than this.
                    "-O2",

                    # Security hardening on by default.
                    # Conservative choice; -D_FORTIFY_SOURCE=2 may be unsafe in some cases.
                    "-D_FORTIFY_SOURCE=1",

                    # Disable assertions
                    "-DNDEBUG",

                    # Removal of unused code and data at link time (can this increase binary
                    # size in some cases?).
                    "-ffunction-sections",
                    "-fdata-sections",
                ],
            ),
            "%{opt_link_flags}": get_starlark_list(
                [] if is_mac else _add_linker_option_if_supported(
                    repo_ctx,
                    tools.cc,
                    "-Wl,--gc-sections",
                    "-gc-sections",
                ),
            ),
            "%{no_canonical_system_headers_flags_cc}": get_starlark_list(
                _get_no_canonical_prefixes_opt(repo_ctx, tools.cc)
            ),
            "%{no_canonical_system_headers_flags_dpcc}": get_starlark_list(
                _get_no_canonical_prefixes_opt(repo_ctx, tools.dpcc)
            ),
            "%{deterministic_compile_flags}": get_starlark_list(
                [
                    # Make C++ compilation deterministic. Use linkstamping instead of these
                    # compiler symbols.
                    "-Wno-builtin-macro-redefined",
                    "-D__DATE__=\\\"redacted\\\"",
                    "-D__TIMESTAMP__=\\\"redacted\\\"",
                    "-D__TIME__=\\\"redacted\\\"",
                ],
            ),
            "%{dbg_compile_flags}": get_starlark_list(
                [
                    "-g",

                    # Disable optimizations explicitly?
                    # Some compilers like Intel uses -O2 by default
                    # "-O0",
                ]
            ),
            "%{coverage_compile_flags}": coverage_compile_flags,
            "%{coverage_link_flags}": coverage_link_flags,
            "%{supports_start_end_lib}": "True" if gold_linker_path else "False",
        },
    )
