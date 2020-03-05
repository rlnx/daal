load("//build/bazel:daal.bzl", "daal_kernel_module")

daal_kernel_module(
  name = "low_order_moments",
  hdrs = glob(["**/*.h", "**/*.i", "**/*.cl"]),
  srcs = glob(["**/*.cpp"]),
  deps = [
    "//cpp/daal:services",
  ],
  visibility = [ "//visibility:public" ],
)
