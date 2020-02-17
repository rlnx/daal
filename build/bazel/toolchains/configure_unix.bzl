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

# OSX add " (framework directory)" at the end of line, strip it.
_MAC_FRAMEWORK_SUFFIX = " (framework directory)"
_MAC_FRAMEWORK_SUFFIX_LEN = len(_MAC_FRAMEWORK_SUFFIX)

def _cxx_inc_convert(path):
    """Convert path returned by cc -E xc++ in a complete path."""
    path = path.strip()
    if path.endswith(_MAC_FRAMEWORK_SUFFIX):
        path = path[:-_MAC_FRAMEWORK_SUFFIX_LEN].strip()
    return path


def get_cxx_inc_directories(repo_ctx, cc, lang_flag, additional_flags = []):
    """Compute the list of default C++ include directories."""
    result = repo_ctx.execute([cc, "-E", lang_flag, "-", "-v"] + additional_flags)
    index1 = result.stderr.find(_INC_DIR_MARKER_BEGIN)
    if index1 == -1:
        return []
    index1 = result.stderr.find("\n", index1)
    if index1 == -1:
        return []
    index2 = result.stderr.rfind("\n ")
    if index2 == -1 or index2 < index1:
        return []
    index2 = result.stderr.find("\n", index2 + 1)
    if index2 == -1:
        inc_dirs = result.stderr[index1 + 1:]
    else:
        inc_dirs = result.stderr[index1 + 1:index2].strip()
    return [ _cxx_inc_convert(p) for p in inc_dirs.split("\n") ]


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
            tool_path = "/usr/bin/" + tool_name
    return str(tool_path)


def _wrap_compilers(repo_ctx, reqs, compiler, cpp_compiler):
    if reqs.os_id == "mac" and reqs.compiler_id == "clang":
        wrapper_src = Label("@bazel_tools//tools/cpp:osx_cc_wrapper.sh.tpl")
        # TODO: Extra environment variables may be required for
        #       some compilers, no need for gcc/clang.
        env = ""
        repo_ctx.template(
            "compiler_wrapper.sh",
            wrapper_src,
            {
                "%{cc}": compiler,
                "%{env}": env,
            }
        )
        repo_ctx.template(
            "compiler_cpp_wrapper.sh",
            wrapper_src,
            {
                "%{cc}": cpp_compiler,
                "%{env}": env,
            }
        )
        compiler = "compiler_wrapper.sh"
        compiler_cpp = "compiler_cpp_wrapper.sh"
    return compiler, cpp_compiler


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
    # { <key>: ( <tool_name>, <mandatory_flag> ) }
    tools_meta = {
        "ar":      ( ar_tool_name,      True  ),
        "ld":      ( "ld",              True  ),
        "gcc":     ( compiler_name,     True  ),
        "cpp":     ( cpp_compiler_name, True  ),
        "dwp":     ( "dwp",             False ),
        "gcov":    ( "gcov",            True  ),
        "nm":      ( "nm",              True  ),
        "objcopy": ( "objcopy",         False ),
        "objdump": ( "objdump",         True  ),
        "strip":   ( "strip",           True  ),
    }
    tool_paths = {
        key: _find_tool(repo_ctx, *meta)
        for key, meta in tools_meta.items()
    }
    wrapped_compiler_path, wrapped_compiler_cpp_path = _wrap_compilers(
        repo_ctx, reqs, tool_paths["gcc"], tool_paths["cpp"])
    original_compiler_path = tool_paths["gcc"]
    tool_paths["gcc"] = wrapped_compiler_path
    tool_paths["cpp"] = wrapped_compiler_cpp_path
    return original_compiler_path, tool_paths


def _preapre_builtin_include_directory_paths(repo_ctx, reqs, cc_path, cxx_opts):
    builtin_include_directories = unique(
        get_cxx_inc_directories(repo_ctx, cc_path, "-xc") +
        get_cxx_inc_directories(repo_ctx, cc_path, "-xc++", cxx_opts) +
        get_cxx_inc_directories(
            repo_ctx,
            cc_path,
            "-xc",
            _get_no_canonical_prefixes_opt(repo_ctx, cc_path),
        ) +
        get_cxx_inc_directories(
            repo_ctx,
            cc_path,
            "-xc++",
            cxx_opts + _get_no_canonical_prefixes_opt(repo_ctx, cc_path),
        ),
    )
    write_builtin_include_directory_paths(repo_ctx, cc_path, builtin_include_directories)
    return builtin_include_directories


def _add_compiler_deps(reqs, cc_path, tool_paths):
    is_compiler_was_wrapped = tool_paths["gcc"] != cc_path
    deps = []
    if is_compiler_was_wrapped:
        deps.append(tool_paths["gcc"])
        deps.append(tool_paths["cpp"])
    return deps


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


def configure_cc_toolchain_unix(repo_ctx, reqs):
    if reqs.os_id not in ["lnx", "mac"]:
        auto_configure_fail("Cannot configure Unix toolchain on '{}', " +
                            "only Linux and Mac supported".format(reqs.os_id))
    is_mac = reqs.os_id == "mac"
    repo_ctx.file(_TEST_CPP_FILE, "int main() { return 0; }")

    # Default compilations/link options
    cxx_opts  = [ "-std=c++17" ]
    link_opts = [ "-lstdc++", "-lm" ]
    link_libs = [ ]

    # Paths to tools/compiler includes
    cc_path, tool_paths = _find_tools(repo_ctx, reqs)
    gold_linker_path = _find_gold_linker_path(repo_ctx, cc_path)
    builtin_include_directories = _preapre_builtin_include_directory_paths(
        repo_ctx, reqs, cc_path, cxx_opts)

    # Addition compile/link flags
    bin_search_flag = _get_bin_search_flag(repo_ctx, cc_path)
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
                _add_compiler_deps(reqs, cc_path, tool_paths)
            ),
            "%{tool_paths}": ",\n        ".join(
                ['"%s": "%s"' % (k, v) for k, v in tool_paths.items()],
            ),
            "%{cxx_builtin_include_directories}": get_starlark_list(builtin_include_directories),
            "%{compile_flags}": get_starlark_list(
                [
                    "-U_FORTIFY_SOURCE",
                    # "-fstack-protector",
                    "-Wall",
                ] + (
                    _add_compiler_option_if_supported(repo_ctx, cc_path, "-Wthread-safety") +
                    _add_compiler_option_if_supported(repo_ctx, cc_path, "-Wself-assign")
                ) + (
                    # Disable problematic warnings.
                    _add_compiler_option_if_supported(repo_ctx, cc_path, "-Wunused-but-set-parameter") +
                    # has false positives
                    _add_compiler_option_if_supported(repo_ctx, cc_path, "-Wno-free-nonheap-object") +
                    # Enable coloring even if there's no attached terminal. Bazel removes the
                    # escape sequences if --nocolor is specified.
                    _add_compiler_option_if_supported(repo_ctx, cc_path, "-fcolor-diagnostics")
                ) + [
                    # Keep stack frames for debugging, even in opt mode.
                    "-fno-omit-frame-pointer",
                ],
            ),
            "%{cxx_flags}": get_starlark_list(cxx_opts),
            "%{link_flags}": get_starlark_list((
                ["-fuse-ld=" + gold_linker_path] if gold_linker_path else []
            ) + _add_linker_option_if_supported(
                repo_ctx,
                cc_path,
                "-Wl,-no-as-needed",
                "-no-as-needed",
            ) + _add_linker_option_if_supported(
                repo_ctx,
                cc_path,
                "-Wl,-z,relro,-z,now",
                "-z",
            ) + (
                [
                    "-undefined",
                    "dynamic_lookup",
                    "-headerpad_max_install_names",
                ] if is_mac else bin_search_flag + [
                    # Gold linker only? Can we enable this by default?
                    # "-Wl,--warn-execstack",
                    # "-Wl,--detect-odr-violations"
                ] + _add_compiler_option_if_supported(
                    # Have gcc return the exit code from ld.
                    repo_ctx,
                    cc_path,
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
                    cc_path,
                    "-Wl,--gc-sections",
                    "-gc-sections",
                ),
            ),
            "%{unfiltered_compile_flags}": get_starlark_list(
                _get_no_canonical_prefixes_opt(repo_ctx, cc_path) + [
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
