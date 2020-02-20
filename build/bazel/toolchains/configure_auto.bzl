load("@onedal//build/bazel/toolchains:configure_unix.bzl", "configure_cc_toolchain_unix")

def _detect_requirements(repo_ctx):
    return struct(
        # TODO: Detect OS, os_ids = [lnx, mac, win]
        os_id = "lnx",

        # TODO: Detect bitness
        host_arch_id = "intel64",
        target_arch_id = "intel64",

        # TODO: Detect compiler, use $env{CC}
        compiler_id = "gcc",

        # TODO: Detect compiler version
        compiler_version = "9.1.0",

        # TODO: Detect DPC++ compiler, use $env{DPCC}
        compiler_dpcpp_id = "clang++",

        # TODO: Detect compiler version
        compiler_dpcpp_version = "2021.1",

        # TODO: Decide whether it required to detect libc/ABI version
        compiler_abi_version = "local",
        libc_version = "local",
        libc_abi_version = "local",
    )

def _configure_cc_toolchain(repo_ctx, reqs):
    configure_cc_toolchain_os = {
        "lnx": configure_cc_toolchain_unix,
        "mac": configure_cc_toolchain_unix,
    }[reqs.os_id]
    return configure_cc_toolchain_os(repo_ctx, reqs)

def _onedal_cc_toolchain_impl(repo_ctx):
    reqs = _detect_requirements(repo_ctx)
    _configure_cc_toolchain(repo_ctx, reqs)

onedal_cc_toolchain = repository_rule(
    implementation = _onedal_cc_toolchain_impl,
    environ = [
        "CC",
    ],
)

def declare_onedal_cc_toolchain(name):
    onedal_cc_toolchain(
        name = name,
    )
    native.register_toolchains(
        "@{}//:all".format(name)
    )
