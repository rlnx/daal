package(default_visibility = ["//visibility:public"])
load("//build/bazel:rules.bzl", "dal_module",
                                "dal_dpcpp_module")

dal_module(
  name = "estimator",
  hdrs = glob(["**/*.hpp"]),
  srcs = [
    "estimator.cpp",
    "train_types.cpp",
  ],
  deps = [
    "//cpp/onedal:common",
  ],
)

dal_module(
  name = "train_ops",
  srcs = [
    "detail/train_ops.cpp",
  ],
  deps = [
    ":estimator",
  ],
)

dal_module(
  name = "logistic_regression",
  deps = [
    ":train_ops",
  ],
)
