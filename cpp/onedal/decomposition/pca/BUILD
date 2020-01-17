load("//build/bazel:rules.bzl", "dal_module",
                                "dal_dpcpp_module")

dal_module(
  name = "headers",
  hdrs = glob(["**/*.hpp"]),
  visibility = ["//visibility:public"],
)

dal_module(
  name = "common_types",
  srcs = [ "common.cpp", ],
  deps = [
    ":headers",
    "//cpp/onedal:common",
  ],
)

dal_module(
  name = "train_types",
  srcs = [ "train_types.cpp"],
  deps = [
    ":common_types",
  ],
)

dal_module(
  name = "train_ops",
  deps = [
    ":train_types",
    "//cpp/onedal/decomposition/pca/backend/cpu:train_kernels",
  ],
  visibility = ["//visibility:public"],
)

dal_dpcpp_module(
  name = "train_ops_dp",
  deps = [
    ":headers",
    "//cpp/onedal:common_dp",
    # "//cpp/onedal/decomposition/pca/backend/gpu:train_kernels",
  ],
  visibility = ["//visibility:public"],
)

dal_module(
  name = "pca",
  deps = [
    ":train_ops"
  ],
  visibility = ["//visibility:public"],
)

dal_dpcpp_module(
  name = "pca_dp",
  deps = [
    ":train_ops_dp"
  ],
  visibility = ["//visibility:public"],
)
