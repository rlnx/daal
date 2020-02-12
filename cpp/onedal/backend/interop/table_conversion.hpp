#pragma once

#include "daal/include/data_management/data/homogen_numeric_table.h"

#include "onedal/table_homogen.hpp"

namespace dal {
namespace backend {
namespace interop {

template <typename T>
struct daal_array_owner {
    explicit daal_array_owner(const array<T>& arr)
        : array_(arr) {}

    void operator() (const void*) {
        array_.reset();
    }

    array<T> array_;
};

template <typename T>
inline auto allocate_daal_homogen_table(std::int64_t row_count,
                                        std::int64_t column_count) {
    return daal::data_management::HomogenNumericTable<T>::create(
        column_count, row_count, daal::data_management::NumericTable::doAllocate);
}

template <typename T>
inline auto convert_to_daal_homogen_table(const table& t) {
    const auto data = flatten<T, access_mode::read>(t);

    const auto daal_data = daal::services::SharedPtr<T>(
        data.get_pointer(), daal_array_owner<T>{data});

    return daal::data_management::HomogenNumericTable<T>::create(
        daal_data, t.get_column_count(), t.get_row_count());
}

template <typename T>
inline auto convert_to_onedal_homogen_table(const daal::data_management::NumericTablePtr& t) {
    daal::data_management::BlockDescriptor<T> desc;

    t->getBlockOfRows(0, t->getNumberOfRows(), daal::data_management::readOnly, desc);

    const auto deleter = [=](T*) mutable {
        t->releaseBlockOfRows(desc);
    };

    return table_homogen{desc.getBlockPtr(),
                         t->getNumberOfRows(),
                         t->getNumberOfColumns(),
                         deleter};
}

} // namespace interop
} // namespace backend
} // namespace dal
