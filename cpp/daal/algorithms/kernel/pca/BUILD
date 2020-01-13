load("//build/bazel:rules.bzl", "daal_module")

cc_library(
  name = "pca_headers",
  hdrs = glob(["**/*.h", "**/*.i", "**/*.cl"]),
  visibility = [ "//visibility:public" ],
)

daal_module(
  name = "pca",
  srcs = glob(["**/*.cpp"]),
  deps = [
    "//cpp/daal:public_common_headers",
    ":pca_headers",
  ],
  visibility = [ "//visibility:public" ],
)
