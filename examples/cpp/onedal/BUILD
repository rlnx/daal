COMMON_COPTS = ["-std=c++17", "-Icpp"]

cc_binary(
  name = "pca",
  srcs = ["pca.cpp"],
  copts = COMMON_COPTS,
  deps = [
    "//cpp/onedal:common",
    "//cpp/onedal/decomposition/pca:pca",
  ],
)
