#pragma once

#include "onedal/data_management/array.hpp"
#include "onedal/data_management/slice.hpp"

namespace dal {

template <typename T, typename... Args>
auto create_array(Args&&... args) {
    return data_management::detail::create_array<T>(std::forward<Args>(args)...);
}

} // namespace dal
