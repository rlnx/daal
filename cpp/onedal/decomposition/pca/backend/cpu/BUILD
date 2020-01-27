package(default_visibility = ["//visibility:public"])
load("//build/bazel:rules.bzl", "dal_module",
                                "dal_cpu_kernel")

dal_cpu_kernel(
  name = "train_kernels",
  hdrs = glob(["*.hpp"]),
  srcs = glob(["train_*.cpp"]),
  deps = [
    "//cpp/onedal/detail/interop",
    "//cpp/onedal/decomposition/pca:common",
    "//cpp/daal/algorithms/kernel/pca",
  ],
)
