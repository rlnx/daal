load("//build/bazel:dal.bzl", "dal_module",
                              "dal_dpcpp_module",
                              "dal_test")

dal_module(
  name = "headers",
  hdrs = glob(["**/*.hpp"], exclude=["**/*_dp.hpp"]),
  visibility = ["//visibility:public"],
)

dal_dpcpp_module(
  name = "headers_dp",
  hdrs = glob(["**/*_dp.hpp"]),
  deps = [":headers"],
  visibility = ["//visibility:public"],
)

dal_module(
  name = "common",
  srcs = glob(["**/*.cpp"], exclude=["**/*_dp.cpp", "**/*_test.cpp"]),
  deps = [
    ":headers",
    "//cpp/onedal/backend:common",
  ],
  visibility = ["//visibility:public"],
)

dal_dpcpp_module(
  name = "common_dp",
  srcs = glob(["**/*_dp.cpp"], exclude=["**/*_test.cpp"]),
  deps = [
    ":headers_dp",
  ],
  visibility = ["//visibility:public"],
)

dal_module(
  name = "dal_core",
  deps = [
    ":common",
    "//cpp/onedal/decomposition/pca:pca",
  ],
  visibility = ["//visibility:public"],
)

dal_dpcpp_module(
  name = "dal_dpcpp",
  deps = [
    ":common_dp",
    "//cpp/onedal/decomposition/pca:pca_dp",
  ],
  visibility = ["//visibility:public"],
)

dal_test(
  name = "common_test",
  srcs = glob(["*_test.cpp"]),
  deps = [
    ":common",
  ],
  visibility = ["//visibility:public"],
)
