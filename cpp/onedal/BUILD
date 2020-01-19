load("//build/bazel:rules.bzl", "dal_module",
                                "dal_dpcpp_module")

cc_library(
    name = "dm",
    srcs = glob(["data_management/**/*.cpp"]),
    hdrs = glob(["data_management/**/*.hpp"]) + 
    ["common.hpp", "range.hpp", "integral_types.hpp"] + 
    glob(["detail/*.hpp"]),
    copts = ["--std=c++17"],
    includes = [".."],
    visibility = ["//visibility:public"],
)

cc_binary(
  name = "dm_test",
  srcs = ["dm_test.cpp"],
  copts = ["--std=c++17"],
  deps = [":dm"],
)

dal_module(
  name = "headers",
  hdrs = glob(["**/*.hpp"], exclude=["**/*_dp.hpp"]),
  visibility = ["//visibility:public"],
)

dal_dpcpp_module(
  name = "headers_dp",
  hdrs = glob(["**/*_dp.hpp"]),
  deps = [
    ":headers",
  ],
  visibility = ["//visibility:public"],
)

dal_module(
  name = "common",
  srcs = glob(["**/*.cpp"], exclude=["**/*_dp.cpp"]),
  deps = [
    ":headers",
  ],
  visibility = ["//visibility:public"],
)

dal_dpcpp_module(
  name = "common_dp",
  srcs = glob(["**/*_dp.cpp"]),
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
