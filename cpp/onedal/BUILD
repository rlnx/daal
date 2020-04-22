load("//build/bazel:dal.bzl", "dal_module",
                              "dal_dpcpp_module",
                              "dal_test")

table_impl_headers = glob(["detail/*table_impl.hpp"])
table_impl_srcs = glob(["detail/*table_impl.cpp"])

dal_module(
  name = "common_types",
  hdrs = [
    "array.hpp",
    "common_helpers.hpp",
    "common.hpp",
    "execution_context.hpp",
    "table_metadata.hpp",
  ] + glob(["detail/*.hpp"], exclude=table_impl_headers),
  srcs = [
    "execution_context.cpp"
  ],
  visibility = ["//visibility:public"],
)

dal_module(
  name = "tables",
  hdrs = [
    "accessor.hpp",
    "table.hpp"
  ] + table_impl_headers,
  srcs = [
    "table.cpp"
  ] + table_impl_srcs,
  deps = [
    ":common_types",
    "//cpp/onedal/backend:convert",
  ],
  visibility = ["//visibility:public"],
)

# TODO: no difference between public and private headers
dal_module(
  name = "common",
  deps = [
    ":common_types",
    ":tables"
  ],
  visibility = ["//visibility:public"],
)

dal_dpcpp_module(
  name = "common_dp",
  hdrs = ["execution_context_dp.hpp"],
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
