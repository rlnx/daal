load("@bazel_skylib//lib:paths.bzl", "paths")

DAAL_CPP_ALGORITHMS = [
  "adaboost",
  "adaboost/inner",
  "assocrules",
  "boosting/inner",
  "brownboost",
  "brownboost/inner",
  "cholesky",
  "classifier",
  "classifier/inner",
  "cordistance",
  "cosdistance",
  "covariance",
  "decision_tree",
  "decision_tree/inner",
  "distributions",
  "distributions/bernoulli",
  "distributions/normal",
  "distributions/uniform",
  "dtrees",
  "dtrees/forest",
  "dtrees/forest/classification",
  "dtrees/forest/classification/inner",
  "dtrees/forest/regression",
  "dtrees/gbt",
  "dtrees/gbt/regression",
  "dtrees/gbt/classification",
  "dtrees/regression",
  "dbscan",
  "engines",
  "engines/mcg59",
  "engines/mt19937",
  "engines/mt2203",
  "em",
  "implicit_als",
  "kernel_function",
  "kmeans/inner",
  "kmeans",
  "k_nearest_neighbors",
  "k_nearest_neighbors/inner",
  "lbfgs",
  "linear_model",
  "lasso_regression",
  "linear_regression",
  "logistic_regression",
  "logistic_regression/inner",
  "logitboost",
  "logitboos/inner",
  "low_order_moments",
  "multiclassclassifier",
  "multiclassclassifier/inner",
  "naivebayes",
  "normalization/minmax",
  "normalization/zscore",
  "normalization/zscore/inner",
  "objective_function",
  "objective_function/inner",
  "objective_function/cross_entropy_loss",
  "objective_function/cross_entropy_loss/inner",
  "objective_function/logistic_loss",
  "objective_function/logistic_loss/inner",
  "objective_function/mse",
  "objective_function/mse/inner",
  "optimization_solver",
  "optimization_solver/inner",
  "optimization_solver/sgd",
  "optimization_solver/sgd/inner",
  "optimization_solver/lbfgs",
  "optimization_solver/lbfgs/inner",
  "optimization_solver/adagrad",
  "optimization_solver/adagrad/inner",
  "optimization_solver/saga",
  "optimization_solver/saga/inner",
  "optimization_solver/coordinate_descent",
  "outlierdetection_multivariate",
  "outlierdetection_bacon",
  "outlierdetection_univariate",
  "pca",
  "pca/inner",
  "pca/metrics",
  "pca/transform",
  "pivoted_qr",
  "qr",
  "quantiles",
  "regression",
  "ridge_regression",
  "sgd",
  "sorting",
  "stump",
  "stump/inner",
  "svd",
  "svm",
  "svm/inner",
  "weak_learner/inner",
]

DAAL_CPP_PUBLIC_INCLUDE_DIRS = [
  "algorithms",
  "algorithms/association_rules",
  "algorithms/boosting",
  "algorithms/cholesky",
  "algorithms/classifier",
  "algorithms/covariance",
  "algorithms/decision_forest",
  "algorithms/decision_tree",
  "algorithms/distance",
  "algorithms/distributions",
  "algorithms/dbscan",
  "algorithms/engines",
  "algorithms/em",
  "algorithms/gradient_boosted_trees",
  "algorithms/implicit_als",
  "algorithms/kernel_function",
  "algorithms/kmeans",
  "algorithms/k_nearest_neighbors",
  "algorithms/lasso_regression",
  "algorithms/linear_model",
  "algorithms/linear_regression",
  "algorithms/logistic_regression",
  "algorithms/moments",
  "algorithms/multi_class_classifier",
  "algorithms/naive_bayes",
  "algorithms/normalization",
  "algorithms/optimization_solver",
  "algorithms/optimization_solver/adagrad",
  "algorithms/optimization_solver/iterative_solver",
  "algorithms/optimization_solver/lbfgs",
  "algorithms/optimization_solver/objective_function",
  "algorithms/optimization_solver/precomputed",
  "algorithms/optimization_solver/sgd",
  "algorithms/optimization_solver/saga",
  "algorithms/optimization_solver/coordinate_descent",
  "algorithms/outlier_detection",
  "algorithms/pca",
  "algorithms/pca/metrics",
  "algorithms/pca/transform",
  "algorithms/pivoted_qr",
  "algorithms/qr",
  "algorithms/quantiles",
  "algorithms/regression",
  "algorithms/ridge_regression",
  "algorithms/sorting",
  "algorithms/stump",
  "algorithms/svd",
  "algorithms/svm",
  "algorithms/tree_utils",
  "algorithms/weak_learner",
  "data_management/compression",
  "data_management/data_source",
  "data_management/data",
  "services",
]

DAAL_CPP_KERNEL_INCLUDES = [
  "algorithms/kernel",
  "algorithms/threading",
  "service/kernel",
  "service/kernel/compression",
  "service/kernel/data_management",
]

DAAL_CPP_EXTERNAL_INCLUDES = [
#  "externals",
  "externals/mklfpk/include",
  "externals/mklfpk/lnx/include",
  "externals/tbb/lnx/include/tbb",
  "externals/tbb/lnx/include",
]

DAAL_FP_TYPES = [
  "float",
  "double",
]

DAAL_CPUS = [
  "sse2",
  "ssse3",
  "sse42",
  "avx",
  "avx2",
  "avx512_mic",
  "avx512",
]


def daal_includes():
  includes  = [ ".", "include" ]
  # includes += [ paths.join("include", x) for x in DAAL_CPP_PUBLIC_INCLUDE_DIRS ]
  # includes += [ paths.join("algorithms/kernel", x) for x in DAAL_CPP_ALGORITHMS ]
  # includes += DAAL_CPP_KERNEL_INCLUDES
  includes += DAAL_CPP_EXTERNAL_INCLUDES
  return includes

def _daal_cpu_copts_gcc(cpu):
  march = {
    "sse2":       "nocona",
    "ssse3":      "nocona",
    "sse42":      "corei7",
    "avx":        "sandybridge",
    "avx2":       "haswell",
    "avx512_mic": "haswell",
    "avx512":     "haswell",
  }
  return [ "-march={}".format(march[cpu]) ]

def _daal_cpu_copts_clang(cpu):
  march = {
    "sse2":       "nocona",
    "ssse3":      "core2",
    "sse42":      "nehalem",
    "avx":        "sandybridge",
    "avx2":       "haswell",
    "avx512_mic": "knl",
    "avx512":     "skx",
  }
  return [ "-march={}".format(march[cpu]) ]

def _daal_common_copts_gcc():
  return [
    "-w",
    "-fwrapv",
    "-fno-strict-overflow",
    "-fno-delete-null-pointer-checks",
  ]

def _daal_common_defines_gcc():
  return [
    '__int64="long long"',
    '__int32="int"',
  ]

def _daal_cc_library(copts=[], local_defines=[], **kwargs):
  native.cc_library(
    copts = copts + _daal_common_copts_gcc(),
    local_defines = local_defines + _daal_common_defines_gcc(),
    **kwargs
  )

def _daal_fpt_cc_library(name, copts=[], local_defines=[], **kwargs):
  for fpt in DAAL_FP_TYPES:
    _daal_cc_library(
      name = name + "_" + fpt,
      local_defines = local_defines + [ "DAAL_FPTYPE={}".format(fpt) ],
      **kwargs
    )

def _daal_cpu_cc_library(name, copts=[], local_defines=[], **kwargs):
  for cpu in DAAL_CPUS:
    _daal_cc_library(
      name = name + "_" + cpu,
      copts = copts + _daal_cpu_copts_gcc(cpu),
      local_defines = local_defines + [ "DAAL_CPU={}".format(cpu) ],
      **kwargs
    )

def _daal_fpt_cpu_cc_library(name, copts=[], local_defines=[], **kwargs):
  for fpt in DAAL_FP_TYPES:
    for cpu in DAAL_CPUS:
      _daal_cc_library(
        name = name + "_" + fpt + "_" + cpu,
        copts = copts + _daal_cpu_copts_gcc(cpu),
        local_defines = local_defines + [ "DAAL_FPTYPE={}".format(fpt),
                                          "DAAL_CPU={}".format(cpu) ],
        **kwargs
      )

def daal_module(srcs=[], **kwargs):
  if not len(srcs):
    _daal_cc_library(**kwargs)
  fpt_files = []
  cpu_files = []
  normal_files = []
  fpt_cpu_files = []
  for src in srcs:
    is_cpu_file = "_cpu" in src
    is_fpt_file = "_fpt" in src
    if is_fpt_file and is_cpu_file:
      fpt_cpu_files.append(src)
    elif is_cpu_file:
      cpu_files.append(src)
    elif is_fpt_file:
      fpt_files.append(src)
    else:
      normal_files.append(src)
  if len(normal_files) > 0:
    _daal_cc_library(srcs=normal_files, **kwargs)
  if len(fpt_files) > 0:
    _daal_fpt_cc_library(srcs=fpt_files, **kwargs)
  if len(cpu_files) > 0:
    _daal_cpu_cc_library(srcs=cpu_files, **kwargs)
  if len(fpt_cpu_files) > 0:
    _daal_fpt_cpu_cc_library(srcs=fpt_cpu_files, **kwargs)
