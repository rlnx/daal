package(default_visibility = ["//visibility:public"])

load("@onedal//build/bazel/toolchains:cc_toolchain_config_mac.bzl", "cc_toolchain_config")

filegroup(
    name = "empty",
    srcs = [],
)

cc_toolchain_config(
    name = "%{cc_toolchain_identifier}",
    cpu = "%{target_cpu}",
    compiler = "%{compiler}",
    toolchain_identifier = "%{cc_toolchain_identifier}",
    host_system_name = "%{host_system_name}",
    target_system_name = "%{target_system_name}",
    target_libc = "%{target_libc}",
    abi_version = "%{abi_version}",
    abi_libc_version = "%{abi_libc_version}",
    cxx_builtin_include_directories = [%{cxx_builtin_include_directories}],
    tool_paths = {%{tool_paths}},
    compile_flags_cc = [%{compile_flags_cc}],
    opt_compile_flags = [%{opt_compile_flags}],
    dbg_compile_flags = [%{dbg_compile_flags}],
    cxx_flags = [%{cxx_flags}],
    link_flags_cc = [%{link_flags_cc}],
    link_libs = [%{link_libs}],
    opt_link_flags = [%{opt_link_flags}],
    no_canonical_system_headers_flags_cc = [%{no_canonical_system_headers_flags_cc}],
    deterministic_compile_flags = [%{deterministic_compile_flags}],
)

cc_toolchain(
    name = "cc_toolchain",
    toolchain_identifier = "%{cc_toolchain_identifier}",
    toolchain_config = ":%{cc_toolchain_identifier}",
    all_files = ":empty",
    ar_files = ":empty",
    as_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = %{supports_param_files},
)

toolchain(
    name = "cc_toolchain_mac",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:macos",
    ],
    target_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:macos",
    ],
    toolchain = ":cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)
