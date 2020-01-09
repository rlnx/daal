load("//build/bazel:rules.bzl", "daal_includes",
                                "daal_module")

cc_library(
  name = "public_common_headers",
  hdrs = glob(["**/*.h", "**/*.i", "**/*.cl"]),
  includes = daal_includes(),
  visibility = [ "//visibility:public" ]
)

daal_module(
  name = "daal_core",
  srcs = glob(["**/*.cpp"]),
  deps = [
    ":public_common_headers"
  ],
)
