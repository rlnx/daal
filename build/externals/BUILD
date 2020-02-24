cc_library(
  name = "mklfpk_headers",
  hdrs = glob(["mklfpk/include/**/*.h",
               "mklfpk/mac/include/**/*.h", ]),
  includes = [
    "mklfpk/include",
    "mklfpk/mac/include",
  ],
  visibility = [ "//visibility:public" ],
)

cc_library(
  name = "tbb_headers",
  hdrs = glob(["tbb/mac/include/tbb/**/*.h",
               "tbb/mac/include/**/*.h", ]),
  includes = [
    "tbb/mac/include/tbb",
    "tbb/mac/include",
  ],
  visibility = [ "//visibility:public" ],
)

cc_library(
  name = "tbb",
  srcs = [
    "tbb/mac/lib/libtbb.dylib",
    "tbb/mac/lib/libtbbmalloc.dylib",
  ],
  deps = [
    ":tbb_headers",
  ],
  linkopts = [
    "-lpthread",
  ],
  visibility = [ "//visibility:public" ],
)

cc_library(
  name = "mklfpk",
  srcs = [
    "mklfpk/mac/lib/intel64/libdaal_vmlipp_core.a",
    "mklfpk/mac/lib/intel64/libdaal_mkl_thread.a",
  ],
  deps = [
    ":mklfpk_headers",
    ":tbb",
  ],
  linkopts = [
    "-ldl",
  ],
  visibility = [ "//visibility:public" ],
)
