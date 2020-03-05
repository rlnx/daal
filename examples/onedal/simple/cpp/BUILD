# COMMON_COPTS = ["-std=c++17", "-Icpp"]
COMMON_COPTS = ["/std:c++17", "/Icpp"]

cc_binary(
  name = "pca",
  srcs = ["pca.cpp"],
  copts = COMMON_COPTS,
  deps = [
    "//cpp/onedal/decomposition/pca",
    "//cpp/daal:threading_tbb",
  ],
)

cc_binary(
  name = "logistic_regression",
  srcs = ["logistic_regression.cpp"],
  copts = COMMON_COPTS,
  deps = [
    "//cpp/onedal/linear_model/logistic_regression",
    "//cpp/daal:threading_tbb",
  ],
)
