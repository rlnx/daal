#include "onedal/data_management/homogen_table.hpp"
#include "onedal/data_management/detail/homogen_table_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {
namespace detail {

template HomogenTable create_table<float>(const float* data, int32_t rows, int32_t cols, DataFormat df);
template HomogenTable create_table<double>(const double* data, int32_t rows, int32_t cols, DataFormat df);

} // namespace detail


template<typename DataType>
HomogenTable::HomogenTable(const DataType* data, int32_t rows, int32_t cols, DataFormat df)
    : Table(Table::Pimpl(new detail::HomogenTableImpl(data, rows, cols, df)))
{ }

} // namespace data_management
} // namespace dal
