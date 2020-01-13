load("//build/bazel:rules.bzl", "daal_module")

cc_library(
  name = "headers",
  hdrs = glob(["**/*.h", "**/*.i", "**/*.cl"]),
  visibility = [ "//visibility:public" ],
)

daal_module(
  name = "pca",
  srcs = glob(["**/*.cpp"]),
  deps = [
    "//cpp/daal:common_headers",
    ":headers",
  ],
  visibility = [ "//visibility:public" ],
)
