load("//build/bazel:rules.bzl", "daal_module")

cc_library(
  name = "public_common_headers",
  hdrs = glob(["**/*.h"]),
  includes = [ 'include' ],
  visibility = [ "//visibility:public" ],
)

cc_library(
  name = "common_headers",
  hdrs = glob(["**/*.h", "**/*.i", "**/*.cl"]),
  includes = [ '.' ],
  deps = [
    ":public_common_headers",
    "//build/externals:tbb_headers",
    "//build/externals:mklfpk_headers",
  ],
  visibility = [ "//visibility:public" ],
)

daal_module(
  name = "daal_core_service",
  srcs = glob(["services/kernel/**/*.cpp"]),
  deps = [
    ":common_headers",
  ],
)

daal_module(
  name = "daal_core_algorithms",
  srcs = glob(["algorithms/kernel/**/*.cpp"]),
  deps = [
    ":common_headers",
    "//cpp/daal/algorithms/kernel/pca:pca",
  ],
)

daal_module(
  name = "daal_core",
  deps = [
    ":daal_core_service",
    ":daal_core_algorithms",
  ]
)
