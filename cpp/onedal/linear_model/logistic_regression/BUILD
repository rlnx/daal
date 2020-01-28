package(default_visibility = ["//visibility:public"])
load("//build/bazel:rules.bzl", "dal_module",
                                "dal_dpcpp_module")

dal_module(
  name = "common",
  hdrs = glob(["**/*.hpp"]),
  # srcs = [
  #   "common.cpp",
  # ],
  deps = [
    "//cpp/onedal:common",
  ],
)

dal_module(
  name = "logistic_regression",
  deps = [
    ":common"
  ],
)
