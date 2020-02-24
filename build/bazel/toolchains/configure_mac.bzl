load(
    "@onedal//build/bazel:utils.bzl",
    "unique"
)
load(
    "@onedal//build/bazel/toolchains:common.bzl",
    "TEST_CPP_FILE",
    "get_cxx_inc_directories",
    "add_compiler_option_if_supported",
    "add_linker_option_if_supported",
    "get_no_canonical_prefixes_opt",
    "get_toolchain_identifier",
)
load(
    "@bazel_tools//tools/cpp:lib_cc_configure.bzl",
    "auto_configure_fail",
    "auto_configure_warning",
    "get_starlark_list",
    "write_builtin_include_directory_paths",
)

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


def _find_tools(repo_ctx, reqs):
    # TODO: Use full compiler path from reqs
    compiler_name = reqs.compiler_id
    cpp_compiler_name = {
        "clang": "clang++",
    }[reqs.compiler_id]
    # { <key>: ( <tool_name>, <mandatory_flag> ) }
    tools_meta = {
        "ar":      ( "libtool",           True  ),
        "ld":      ( "ld",                True  ),
        "gcc":     ( compiler_name,       True  ),
        "cpp":     ( cpp_compiler_name,   True  ),
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
        paths = tool_paths,
    )


def _preapre_builtin_include_directory_paths(repo_ctx, reqs, tools, cxx_opts):
    builtin_include_directories = unique(
        get_cxx_inc_directories(repo_ctx, tools.cc, "-xc") +
        get_cxx_inc_directories(repo_ctx, tools.cc, "-xc++", cxx_opts) +
        get_cxx_inc_directories(
            repo_ctx,
            tools.cc,
            "-xc",
            get_no_canonical_prefixes_opt(repo_ctx, tools.cc),
        ) +
        get_cxx_inc_directories(
            repo_ctx,
            tools.cc,
            "-xc++",
            cxx_opts + get_no_canonical_prefixes_opt(repo_ctx, tools.cc),
        )
    )
    write_builtin_include_directory_paths(repo_ctx, tools.cc, builtin_include_directories)
    return builtin_include_directories



def configure_cc_toolchain_mac(repo_ctx, reqs):
    if reqs.os_id != "mac":
        auto_configure_fail("Cannot configure MacOs toolchain for '{}'".format(reqs.os_id))
    repo_ctx.file(TEST_CPP_FILE, "int main() { return 0; }")

    # Default compilations/link options
    cxx_opts  = [ "-std=c++17" ]
    link_opts = [ ]
    link_libs = [ "-lstdc++", "-lm" ]

    # Paths to tools/compiler includes
    tools = _find_tools(repo_ctx, reqs)
    builtin_include_directories = _preapre_builtin_include_directory_paths(
        repo_ctx, reqs, tools, cxx_opts)

    repo_ctx.template(
        "BUILD",
        Label("@onedal//build/bazel/toolchains:BUILD_cc_toolchain_def_mac.tpl"),
        {
            # Various IDs
            "%{cc_toolchain_identifier}": get_toolchain_identifier(reqs),
            "%{compiler}":                reqs.compiler_id + "-" + reqs.compiler_version,
            "%{abi_version}":             reqs.compiler_abi_version,
            "%{abi_libc_version}":        reqs.libc_abi_version,
            "%{target_libc}":             reqs.libc_version,
            "%{target_cpu}":              reqs.target_arch_id,
            "%{host_system_name}":        reqs.os_id + "-" + reqs.host_arch_id,
            "%{target_system_name}":      reqs.os_id + "-" + reqs.target_arch_id,

            "%{supports_param_files}": "1",
            "%{cc_compiler_deps}": get_starlark_list(
                [":builtin_include_directory_paths"]
            ),
            "%{tool_paths}": ",\n        ".join(
                ['"%s": "%s"' % (k, v) for k, v in tools.paths.items()],
            ),
            "%{cxx_builtin_include_directories}": get_starlark_list(builtin_include_directories),
            "%{compile_flags_cc}": get_starlark_list(
                [
                    "-U_FORTIFY_SOURCE",
                    "-fstack-protector",
                    "-Wall",
                ] +
                add_compiler_option_if_supported(repo_ctx, tools.cc, "-Wthread-safety") +
                add_compiler_option_if_supported(repo_ctx, tools.cc, "-Wself-assign") +
                # Disable problematic warnings.
                add_compiler_option_if_supported(repo_ctx, tools.cc, "-Wunused-but-set-parameter") +
                # has false positives
                add_compiler_option_if_supported(repo_ctx, tools.cc, "-Wno-free-nonheap-object") +
                # Enable coloring even if there's no attached terminal. Bazel removes the
                # escape sequences if --nocolor is specified.
                add_compiler_option_if_supported(repo_ctx, tools.cc, "-fcolor-diagnostics") +
                [
                    # Keep stack frames for debugging, even in opt mode.
                    "-fno-omit-frame-pointer",
                ],
            ),
            "%{cxx_flags}": get_starlark_list(cxx_opts),
            "%{link_flags_cc}": get_starlark_list(
                # add_linker_option_if_supported(
                #     repo_ctx,
                #     tools.cc,
                #     "-Wl,-no-as-needed",
                #     "-no-as-needed",
                # ) +
                # add_linker_option_if_supported(
                #     repo_ctx,
                #     tools.cc,
                #     "-Wl,-z,relro,-z,now",
                #     "-z",
                # ) +
                # add_compiler_option_if_supported(
                #     # Have gcc return the exit code from ld.
                #     repo_ctx,
                #     tools.cc,
                #     "-pass-exit-codes",
                # ) +
                link_opts
            ),
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
                add_linker_option_if_supported(
                    repo_ctx,
                    tools.cc,
                    "-Wl,--gc-sections",
                    "-gc-sections",
                ),
            ),
            "%{no_canonical_system_headers_flags_cc}": get_starlark_list(
                get_no_canonical_prefixes_opt(repo_ctx, tools.cc)
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
            "%{macosx_version_min}": "10.11",
        },
    )
