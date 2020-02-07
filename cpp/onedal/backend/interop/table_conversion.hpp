#pragma once

#include "daal/include/data_management/data/homogen_numeric_table.h"

#include "onedal/table.hpp"
#include "onedal/table_homogen_impl.hpp"

namespace dal {
namespace backend {
namespace interop {

template <typename Float>
inline auto make_daal_homogen_table(std::int64_t row_count, std::int64_t column_count) {
    return daal::data_management::HomogenNumericTable<Float>::create(column_count, row_count,
                daal::data_management::NumericTable::doAllocate);
}

template <typename Float>
inline auto to_daal_homogen_table(const table& t) {
    const auto array = flatten<Float, access_mode::read>(t);
    return daal::data_management::HomogenNumericTable<Float>::create(array.get_pointer(),
                                                                     t.get_column_count(),
                                                                     t.get_row_count());
}

template <typename Float>
inline auto from_daal_table(daal::data_management::NumericTable& t) {
    daal::data_management::BlockDescriptor<Float> desc;
    t.getBlockOfRows(0, t.getNumberOfRows(), daal::data_management::readOnly, desc);

    dal::detail::table_impl_ptr result {
        new dal::detail::table_homogen_impl(desc.getBlockPtr(),
                                            t.getNumberOfRows(),
                                            t.getNumberOfColumns())
    };

    t.releaseBlockOfRows(desc);
    return result;
}

} // namespace interop
} // namespace backend
} // namespace dal
