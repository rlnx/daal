#pragma once

#include "onedal/decomposition/pca/common.hpp"

using namespace dal::data_management;

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
  model::pimpl trained_model;
  table::pimpl eigenvalues;
  table::pimpl eigenvectors;
  table::pimpl explained_variance;
};

} // namespace detail
} // namespace pca
} // namespace decomposition
} // namespace dal
