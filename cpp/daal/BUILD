load("//build/bazel:rules.bzl", "daal_module",
                                "daal_includes")

cc_library(
  name = "public_common_headers",
  hdrs = glob(["**/*.h", "**/*.i", "**/*.cl"]),
  includes = daal_includes(),
  deps = [
    "//build/externals:tbb_headers",
    "//build/externals:mklfpk_headers",
  ],
  visibility = [ "//visibility:public" ],
)

daal_module(
  name = "daal_core_service",
  srcs = glob(["services/kernel/**/*.cpp"]),
  deps = [
    ":public_common_headers",
  ],
)

# daal_module(
#   name = "daal_core_algorithms",
#   srcs = glob(["algorithms/kernel/**/*.cpp"]),
#   deps = [
#     ":public_common_headers"
#   ],
# )

daal_module(
  name = "daal_core",
  deps = [
    ":daal_core_service",
    # ":daal_core_algorithms",
  ]
)
