package(default_visibility = ["//visibility:public"])
load("//build/bazel:rules.bzl", "dal_module",
                                "dal_gpu_kernel")

dal_gpu_kernel(
  name = "train_kernels",
  hdrs = glob(["*.hpp"]),
  srcs = glob(["train_*.cpp"]),
  deps = [
    "//cpp/onedal/decomposition/pca:common",
    "//cpp/daal/algorithms/kernel/pca",
  ],
)
