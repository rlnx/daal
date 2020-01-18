#pragma once

#include "onedal/data_management/homogen_table.hpp"

namespace dal {
namespace data_management {

namespace detail {

template <typename DataType>
HomogenTable create_table(const DataType* data,
                          uint32_t rows, uint32_t cols,
                          DataFormat df = DataFormat::rowmajor);

} // namespace detail


template <typename... Args>
auto create_table(Args&&... args) {
    return detail::create_table(std::forward<Args>(args)...);
}

} // namespace data_management
} // namespace dal
