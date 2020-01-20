#pragma once

#include "onedal/data_management/detail/create_table.hpp"

namespace dal {

template <typename... Args>
auto create_table(Args&&... args) {
    return data_management::detail::create_table(std::forward<Args>(args)...);
}

} // namespace dal
