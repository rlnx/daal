#pragma once

#include "onedal/decomposition/pca/common.hpp"

namespace dal {
namespace decomposition {
namespace pca {
namespace detail {

class train_input_impl : public base {
 public:
  train_input_impl(const table& data)
      : data(data) {}

  table data;
};

class train_result_impl : public base {
 public:
  shared<model_impl> trained_model;
  shared<dal::detail::table_impl> eigenvalues;
  shared<dal::detail::table_impl> eigenvectors;
  shared<dal::detail::table_impl> explained_variance;
};

} // namespace detail
} // namespace pca
} // namespace decomposition
} // namespace dal
