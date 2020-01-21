#pragma once

#include "onedal/decomposition/pca/common.hpp"

using namespace dal::data_management;

namespace dal {
namespace decomposition {
namespace pca {
namespace detail {

class params_impl : public base {
 public:
  std::int32_t components_count = -1;
  bool is_deterministic = false;
};

class model_impl : public base {
 public:
  table::pimpl eigenvectors;
};

} // namespace detail
} // namespace pca
} // namespace decomposition
} // namespace dal
