#pragma once

#include "onedal/decomposition/pca/common.hpp"

using namespace dal::data_management;

namespace dal {
namespace decomposition {
namespace pca {
namespace detail {

class train_input_impl : public Base {
 public:
  train_input_impl(const Table& data)
      : data(data) {}

  Table data;
};

class train_result_impl : public Base {
 public:
  model::Pimpl trained_model;
  Table::Pimpl eigenvalues;
  Table::Pimpl eigenvectors;
  Table::Pimpl explained_variance;
};

} // namespace detail
} // namespace pca
} // namespace decomposition
} // namespace dal
