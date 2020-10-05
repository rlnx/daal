package(default_visibility = ["//visibility:public"])
load("@onedal//dev/bazel:dal.bzl",
    "dal_module",
    "dal_test_suite",
)

dal_module(
    name = "mkl",
    auto = True,
    auto_exclude = ["*_win_dll.cpp"],
    dal_deps = [
        "@onedal//cpp/oneapi/dal:common",
    ],
)

dal_test_suite(
    name = "tests",
    dpc = False,
)
