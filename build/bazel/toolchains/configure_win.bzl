load(
    "@bazel_tools//tools/cpp:lib_cc_configure.bzl",
    "auto_configure_fail",
    "auto_configure_warning",
    "get_starlark_list",
    "write_builtin_include_directory_paths",
)
load(
    "@onedal//build/bazel:utils.bzl",
    "get_starlark_dict",
    "unique",
)
load(
    "@onedal//build/bazel/toolchains:common.bzl",
    "get_toolchain_identifier",
    "get_default_compiler_options",
    "get_cpu_specific_options",
)

def _find_tool(repo_ctx, tool_name, mandatory=False):
    if ":\\" in tool_name:
        return tool_name
    tool_path = repo_ctx.which(tool_name + ".exe")
    if not tool_path:
        if mandatory:
            auto_configure_fail("Cannot find {}; try to correct your %PATH%".format(tool_name))
        else:
            tool_path = "tool_not_found_wrapper.bat"
            repo_ctx.template(
                tool_path,
                Label("@onedal//build/bazel/toolchains:tool_not_found.bat.tpl"),
                { "%{tool_name}": tool_name }
            )
    return str(tool_path)


def _find_tools(repo_ctx, reqs):
    # TODO: Use full compiler path from reqs
    cl_path = _find_tool(repo_ctx, "cl", mandatory=True)
    lib_path = repo_ctx.path(str(repo_ctx.path(cl_path).dirname) + "/lib.exe")
    link_path = repo_ctx.path(str(repo_ctx.path(cl_path).dirname) + "/link.exe")
    if not lib_path.exists or not link_path.exists:
        auto_configure_fail("Cannot find lib.exe or link.exe; Make sure MSVC toolchain is installed correctly")
    return struct(
        cc    = _find_tool(repo_ctx, reqs.compiler_id, mandatory=True),
        lib   = str(lib_path),
        link  = str(link_path),
        dpcc  = _find_tool(repo_ctx, reqs.compiler_dpcc_id, mandatory=False),
    )

def _preapre_builtin_include_directory_paths(repo_ctx, reqs, tools, cxx_opts):
    include_paths = ""
    if "INCLUDE" in repo_ctx.os.environ:
        include_paths = repo_ctx.os.environ["INCLUDE"]
    builtin_include_directories = []
    for path in include_paths.split(";"):
        if path:
            builtin_include_directories.append(path.replace("\\", "\\\\"))
    write_builtin_include_directory_paths(repo_ctx, tools.cc, unique(builtin_include_directories))
    return builtin_include_directories


def configure_cc_toolchain_win(repo_ctx, reqs):
    if reqs.os_id != "win":
        auto_configure_fail("Cannot configure Windows toolchain for '{}'".format(reqs.os_id))

    # Default compilations/link options
    cxx_opts  = [ "/std:c++17" ]
    link_opts = [ ]
    link_libs = [ ]

    # Paths to tools/compiler includes
    tools = _find_tools(repo_ctx, reqs)
    builtin_include_directories = _preapre_builtin_include_directory_paths(
        repo_ctx, reqs, tools, cxx_opts)

    repo_ctx.template(
        "BUILD",
        Label("@onedal//build/bazel/toolchains:BUILD_cc_toolchain_def_win.tpl"),
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
            "%{cc_path}": tools.cc,
            "%{dpcc_path}": tools.dpcc,
            "%{lib_path}": tools.lib,
            "%{link_path}": tools.link,
            "%{cxx_builtin_include_directories}": get_starlark_list(builtin_include_directories),
            "%{msvc_env_lib}": repo_ctx.os.environ["LIB"].replace("\\", "\\\\"),
            # "%{msvc_env_lib}": " ",
            "%{msvc_env_include}": repo_ctx.os.environ["INCLUDE"].replace("\\", "\\\\"),
            # "%{msvc_env_include}": " ",
            "%{msvc_env_path}": repo_ctx.os.environ["PATH"].replace("\\", "\\\\").replace("%", "%%"),
            # "%{msvc_env_path}": " ",

            "%{compile_flags_cc}": get_starlark_list(
                get_default_compiler_options(repo_ctx, reqs, tools.cc, is_dpcc=False)
            ),
            "%{compile_flags_dpcc}": get_starlark_list(
                get_default_compiler_options(repo_ctx, reqs, tools.dpcc, is_dpcc=True)
            ),
            "%{cxx_flags}": get_starlark_list(cxx_opts),
            "%{link_flags_cc}": get_starlark_list(
                []
            ),
            "%{link_flags_dpcc}": get_starlark_list(
                []
            ),
            "%{link_libs}": get_starlark_list(link_libs),
            "%{opt_compile_flags}": get_starlark_list(
                []
            ),
            "%{opt_link_flags}": get_starlark_list(
                []
            ),
            "%{dbg_compile_flags}": get_starlark_list(
                []
            ),
            "%{deterministic_compile_flags}": get_starlark_list(
                [
                    "/wd4117",
                    "/D__DATE__=\\\"redacted\\\"",
                    "/D__TIMESTAMP__=\\\"redacted\\\"",
                    "/D__TIME__=\\\"redacted\\\"",
                ],
            ),
            "%{cpu_flags}": get_starlark_dict(
                get_cpu_specific_options(reqs)
            )
        },
    )
