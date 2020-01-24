#pragma once

#include "onedal/decomposition/pca/common.hpp"

namespace dal {
namespace decomposition {
namespace pca {
namespace detail {

class params_impl : public base {
 public:
  std::int64_t components_count = -1;
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
