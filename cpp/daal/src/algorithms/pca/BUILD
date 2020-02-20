load("//build/bazel:daal.bzl", "daal_kernel_module")

daal_kernel_module(
  name = "pca",
  hdrs = glob(["**/*.h", "**/*.i", "**/*.cl"]),
  srcs = glob(["**/*.cpp"]),
  deps = [
    "//cpp/daal:services",
    "//cpp/daal/src/algorithms/covariance",
  ],
  visibility = [ "//visibility:public" ],
)
