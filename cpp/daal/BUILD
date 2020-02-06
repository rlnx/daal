package(default_visibility = ["//visibility:public"])
load("//build/bazel:rules.bzl", "daal_module",
                                "daal_kernel_module",
                                "daal_shared_lib")

daal_module(
  name = "public_headers",
  hdrs = glob(["include/**/*.h"]),
  includes = ["include"],
)

daal_module(
  name = "common_headers",
  hdrs = glob(["**/*.h", "**/*.i", "**/*.cl"], exclude=["include/**"]),
  includes = ["."],
  deps = [
    ":public_headers",
    "//build/externals:tbb_headers",
    "//build/externals:mklfpk_headers",
  ],
)

daal_kernel_module(
  name = "services",
  srcs = glob([
    "src/services/**/*.cpp",
    "src/algorithms/*.cpp",
  ]) + [
    "src/externals/service_atomic_int.cpp",
    "src/externals/service_memory.cpp",
    "src/externals/service_profiler.cpp",
    "src/externals/service_service.cpp",
  ],
  deps = [
    ":common_headers",
  ],
)

daal_module(
  name = "threading_tbb",
  srcs = glob([
    "src/threading/*.cpp"
  ]),
  local_defines = ["__DO_TBB_LAYER__"],
  deps = [
    ":common_headers",
    "//build/externals:tbb",
    "//build/externals:mklfpk",
  ],
)

daal_module(
  name = "threading_seq",
  srcs = glob([
    "src/threading/*.cpp"
  ]),
  local_defines = ["__DO_SEQ_LAYER__"],
  deps = [
    ":common_headers",
    "//build/externals:tbb",
    "//build/externals:mklfpk",
  ],
)

daal_module(
  name = "algorithms",
  deps = [
    ":services",
    "//cpp/daal/src/algorithms/covariance",
    "//cpp/daal/src/algorithms/pca",
  ],
)

daal_module(
  name = "core",
  deps = [
    ":services",
    ":algorithms",
  ],
)

daal_shared_lib(
  name = "daal_core",
  deps = [
    ":core",
  ],
)

daal_shared_lib(
  name = "daal_thread",
  deps = [
    ":threading_tbb",
  ],
)

daal_shared_lib(
  name = "daal_sequential",
  deps = [
    ":threading_seq",
  ],
)

filegroup(
  name = "daal",
  srcs = [
    ":daal_core",
    ":daal_thread",
    ":daal_sequential",
  ],
)
