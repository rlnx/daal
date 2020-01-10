cc_library(
  name = "mklfpk_headers",
  hdrs = glob(["mklfpk/include/**/*.h",
               "mklfpk/lnx/include/**/*.h", ]),
  includes = [
    "mklfpk/include",
    "mklfpk/lnx/include",
  ],
  visibility = [ "//visibility:public" ],
)

cc_library(
  name = "tbb_headers",
  hdrs = glob(["tbb/lnx/include/tbb/**/*.h",
               "tbb/lnx/include/**/*.h", ]),
  includes = [
    "tbb/lnx/include/tbb",
    "tbb/lnx/include",
  ],
  visibility = [ "//visibility:public" ],
)
