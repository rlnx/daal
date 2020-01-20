#pragma once

#include "onedal/data_management/array.hpp"
#include "onedal/data_management/slice.hpp"

namespace dal {
namespace data_management {
namespace detail {

template <typename T>
Array<T> create_array(const Slice&);

} // namespace detail  
} // namespace data_management
} // namespace dal
