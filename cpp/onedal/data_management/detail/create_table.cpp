#include "onedal/data_management/homogen_table.hpp"

namespace dal {
namespace data_management {
namespace detail {

template <typename DataType>
HomogenTable create_table(const DataType* data, std::int32_t rows, std::int32_t cols, DataFormat df) {
    return HomogenTable(data, rows, cols, df);
}

template HomogenTable create_table<float>(const float* data, int32_t rows, int32_t cols, DataFormat df);
template HomogenTable create_table<double>(const double* data, int32_t rows, int32_t cols, DataFormat df);
template HomogenTable create_table<std::int32_t>(const std::int32_t* data, int32_t rows, int32_t cols, DataFormat df);

} // namespace detail
} // namespace data_management
} // namespace dal
