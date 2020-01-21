COMMON_COPTS = ["-std=c++17", "-Icpp"]

cc_binary(
  name = "pca",
  srcs = ["pca.cpp"],
  copts = COMMON_COPTS,
  deps = [
    "//cpp/onedal/decomposition/pca",
    "//cpp/onedal/data_management",
    "//cpp/daal:threading_tbb",
  ],
)
