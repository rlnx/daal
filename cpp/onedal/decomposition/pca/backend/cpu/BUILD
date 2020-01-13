load("//build/bazel:rules.bzl", "dal_module", "dal_cpu_kernel")

dal_module(
  name = "headers",
  hdrs = glob(["*.hpp"]),
  visibility = ["//visibility:public"],
)

dal_cpu_kernel(
  name = "train_kernels",
  srcs = glob(["train_*.cpp"]),
  deps = [
    ":headers",
    "//cpp/onedal:common_headers",
    "//cpp/onedal/decomposition/pca:headers",
    "//cpp/daal:public_common_headers",
  ],
  visibility = ["//visibility:public"],
)
