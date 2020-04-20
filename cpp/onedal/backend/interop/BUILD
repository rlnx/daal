package(default_visibility = ["//visibility:public"])
load("//build/bazel:dal.bzl", "dal_module")

dal_module(
    name = "types",
    hdrs = ["common.hpp", "data_conversion.hpp"],
    deps = [
        "//cpp/daal:core",
        "//cpp/onedal:common_types"
    ]
)

dal_module(
    name = "tables",
    hdrs = ["table_conversion.hpp"],
    deps = [
        "//cpp/onedal:tables"
    ]
)
