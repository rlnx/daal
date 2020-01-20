#pragma once

#include "onedal/integral_types.hpp"
#include "onedal/data_management/homogen_table.hpp"

namespace dal {
namespace data_management {

namespace detail {

template <typename DataType>
HomogenTable create_table(const DataType* data,
                          std::int32_t rows, std::int32_t cols,
                          DataFormat df = DataFormat::rowmajor);

} // namespace detail
} // namespace data_management
} // namespace dal