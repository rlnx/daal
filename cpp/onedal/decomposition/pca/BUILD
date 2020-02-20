package(default_visibility = ["//visibility:public"])
load("//build/bazel:dal.bzl", "dal_module",
                              "dal_dpcpp_module")

dal_module(
  name = "common",
  hdrs = glob(["**/*.hpp"]),
  srcs = [
    "common.cpp",
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
    ":common",
    "//cpp/onedal/decomposition/pca/backend/cpu:train_kernels",
  ],
)

dal_dpcpp_module(
  name = "train_ops_dp",
  srcs = [
    "detail/train_ops_dp.cpp",
  ],
  deps = [
    ":common",
    "//cpp/onedal:common_dp",
    "//cpp/onedal/decomposition/pca/backend/cpu:train_kernels",
    "//cpp/onedal/decomposition/pca/backend/gpu:train_kernels",
  ],
)

dal_module(
  name = "pca",
  deps = [
    ":train_ops"
  ],
)

dal_dpcpp_module(
  name = "pca_dp",
  deps = [
    ":train_ops_dp"
  ],
)
