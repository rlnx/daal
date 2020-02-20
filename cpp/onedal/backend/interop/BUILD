package(default_visibility = ["//visibility:public"])
load("//build/bazel:dal.bzl", "dal_module")

dal_module(
    name = "interop",
    hdrs = glob(["**/*.hpp"]),
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//cpp/daal:public_headers",
        "//cpp/onedal:common"
    ]
)
