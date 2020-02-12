#pragma once

#include "daal/include/data_management/data/homogen_numeric_table.h"

#include "onedal/table_homogen.hpp"

namespace dal {
namespace backend {
namespace interop {

template <typename T>
struct daal_array_owner {
    explicit daal_array_holder(const array<T>& arr)
        : array_(arr) {}

    void operator() (const void*) const {
        array_.reset();
    }

    array<T> array_;
};

template <typename Float>
inline auto allocate_daal_homogen_table(std::int64_t row_count,
                                        std::int64_t column_count) {
    return daal::data_management::HomogenNumericTable<Float>::create(
        column_count, row_count, daal::data_management::NumericTable::doAllocate);
}

inline constexpr auto get_daal_access_mode(access_mode access) {
    switch (access) {
        // TODO: Think about read/write mapping
        case access_mode::read: return daal::data_management::readOnly;
        case access_mode::write: return daal::data_management::readWrite;
    }
}

template <typename T, access_mode Access>
struct table_converter {
    auto to_daal_homogen_table(const table& t) {
        const auto data = flatten<T, Access>(t);

        const auto daal_data = daal::services::SharedPtr<T>(
            array.get_pointer(), daal_array_owner<T>{array});

        return daal::data_management::HomogenNumericTable<T>::create(
            daal_data, column_count, row_count);
    }

    auto to_onedal_homogen_table(const daal::data_management::NumericTablePtr& t) {
        daal::data_management::BlockDescriptor<T> desc;

        t->getBlockOfRows(0, t.getNumberOfRows(), get_daal_access_mode(Access), desc);

        const auto deleter = [=](T*) {
            t->releaseBlockOfRows(desc);
        };

        return table_homogen{desc.getBlockPtr(),
                             t.getNumberOfRows(),
                             t.getNumberOfRows(),
                             deleter};
    }
};

template <typename Float, access_mode Access>
inline auto convert_to_daal_homogen_table(const table& t) {
    return table_converter<Float, Access>().to_daal_homogen_table(t);
}

template <typename Float, access_mode Access>
inline auto convert_to_onedal_homogen_table(const daal::data_management::NumericTablePtr& t) {
    return table_converter<Float, Access>().to_onedal_homogen_table(t);
}

} // namespace interop
} // namespace backend
} // namespace dal
