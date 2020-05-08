package(default_visibility = ["//visibility:public"])
load("//build/bazel:dal.bzl", "dal_module")

dal_module(
    name = "interop",
    hdrs = glob(["*.hpp"]),
    deps = [
        "//cpp/daal:core",
        "//cpp/onedal:headers",
    ]
)
