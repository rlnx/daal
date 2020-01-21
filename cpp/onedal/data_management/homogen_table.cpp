#include "onedal/data_management/homogen_table.hpp"
#include "onedal/data_management/detail/homogen_table_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {

template<typename DataType>
homogen_table::homogen_table(const DataType* data, int32_t rows, int32_t cols, data_format df)
    : table(table::pimpl(new detail::homogen_table_impl(data, rows, cols, df)))
{ }

template homogen_table::homogen_table<float>(const float* data, int32_t rows, int32_t cols, data_format df);
template homogen_table::homogen_table<double>(const double* data, int32_t rows, int32_t cols, data_format df);
template homogen_table::homogen_table<std::int32_t>(const std::int32_t* data, int32_t rows, int32_t cols, data_format df);

} // namespace data_management
} // namespace dal
