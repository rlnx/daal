package(default_visibility = ["//visibility:public"])
load("//build/bazel:dal.bzl", "dal_module")

dal_module(
    name = "headers",
    hdrs = glob(["**/*.hpp"], exclude=["**/*_dp.hpp"]),
    deps = [
        "//cpp/onedal:headers",
    ],
)

dal_module(
    name = "headers_dp",
    hdrs = glob(["**/*_dp.hpp"]),
    deps = [":headers"],
)

dal_module(
    name = "common",
    srcs = glob(["**/*.cpp"], exclude = ["**/*_dp.cpp"]),
    deps = [
        ":headers",
        "//cpp/onedal/backend/interop",
    ],
)
