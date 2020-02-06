package(default_visibility = ["//visibility:public"])
load("//build/bazel:rules.bzl", "dal_module",
                                "dal_cpu_kernel")

dal_module(
    name = "interop",
    hdrs = glob(["**/*.hpp"]),
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//cpp/daal/src:public_headers",
        "//cpp/onedal:common"
    ]
)
