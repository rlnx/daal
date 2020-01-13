load("//bazel/dal:rules.bzl", "dal_module", "dal_gpu_kernel")

dal_module(
  name = "headers",
  hdrs = glob(["*.hpp"]),
  deps = [
    "//cpp/dal/decomposition/pca:headers",
  ],
)

dal_cpu_kernel(
  name = "train_kernels",
  srcs = glob(["train_*.cpp"]),
  gpus = [ "default" ],
  deps = [ ":headers", ],
)
