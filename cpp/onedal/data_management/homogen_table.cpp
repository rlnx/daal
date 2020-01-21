#include "onedal/data_management/homogen_table.hpp"
#include "onedal/data_management/detail/homogen_table_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {

template<typename DataType>
homogen_table::homogen_table(const DataType* data, int32_t rows, int32_t cols, data_format df)
    : table(table::pimpl(new detail::HomogenTableImpl(data, rows, cols, df)))
{ }

template homogen_table::HomogenTable<float>(const float* data, int32_t rows, int32_t cols, DataFormat df);
template homogen_table::HomogenTable<double>(const double* data, int32_t rows, int32_t cols, DataFormat df);
template homogen_table::HomogenTable<std::int32_t>(const std::int32_t* data, int32_t rows, int32_t cols, DataFormat df);

} // namespace data_management
} // namespace dal
