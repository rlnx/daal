cc_library(
    name = "mklfpk_headers",
    hdrs = glob([
        "mklfpk/include/**/*.h",
        "mklfpk/lnx/include/**/*.h",
        # "mklfpk/mac/include/**/*.h",
    ]),
    includes = [
        "mklfpk/include",
        "mklfpk/lnx/include",
        # "mklfpk/mac/include",
    ],
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "tbb_headers",
    hdrs = glob([
        "tbb/lnx/include/tbb/**/*.h",
        "tbb/lnx/include/**/*.h",
        # "tbb/mac/include/tbb/**/*.h",
        # "tbb/mac/include/**/*.h",
    ]),
    includes = [
        "tbb/lnx/include/tbb",
        "tbb/lnx/include",
        # "tbb/mac/include/tbb",
        # "tbb/mac/include",
    ],
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "tbb",
    srcs = [
        "tbb/lnx/lib/intel64/gcc4.8/libtbb.so.2",
        "tbb/lnx/lib/intel64/gcc4.8/libtbbmalloc.so.2",
        # "tbb/mac/lib/libtbb.dylib",
        # "tbb/mac/lib/libtbbmalloc.dylib",
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
        "mklfpk/lnx/lib/intel64/libdaal_vmlipp_core.a",
        "mklfpk/lnx/lib/intel64/libdaal_mkl_thread.a",
        # "mklfpk/mac/lib/intel64/libdaal_vmlipp_core.a",
        # "mklfpk/mac/lib/intel64/libdaal_mkl_thread.a",
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
