load("//build/bazel:rules.bzl", "dal_module", "dal_cpu_kernel")

dal_cpu_kernel(
  name = "train_kernels",
  hdrs = glob(["*.hpp"]),
  srcs = glob(["train_*.cpp"]),
  deps = [
    "//cpp/onedal:common",
    "//cpp/onedal/decomposition/pca:headers",
    "//cpp/daal/algorithms/kernel/pca",
  ],
  visibility = ["//visibility:public"],
)
