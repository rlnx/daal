package(default_visibility = ["//visibility:public"])
load("//build/bazel:dal.bzl", "dal_module")

dal_module(
    name = "convert",
    hdrs = ["convert.hpp",],
    srcs = ["convert.cpp",],
    deps = [
        "//cpp/daal:public_headers",
        "//cpp/onedal/backend/interop:types"
    ],
)
