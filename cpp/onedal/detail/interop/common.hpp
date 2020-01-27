#pragma once

#include "daal/include/services/env_detect.h"
#include "daal/include/data_management/data/homogen_numeric_table.h"

#include "onedal/backend/dispatcher.hpp"
#include "onedal/table.hpp"
#include "onedal/detail/table_impl.hpp"

namespace dal {
namespace detail {
namespace interop {

template <typename CpuType>
constexpr daal::CpuType get_daal_cpu_type();

template <>
constexpr daal::CpuType get_daal_cpu_type<backend::cpu_dispatch_default>() {
    return daal::CpuType::sse2;
}

template <>
constexpr daal::CpuType get_daal_cpu_type<backend::cpu_dispatch_avx>() {
    return daal::CpuType::avx;
}

template <>
constexpr daal::CpuType get_daal_cpu_type<backend::cpu_dispatch_avx2>() {
    return daal::CpuType::avx2;
}

template <>
constexpr daal::CpuType get_daal_cpu_type<backend::cpu_dispatch_avx512>() {
    return daal::CpuType::avx512;
}

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

    detail::table_impl_ptr result { new detail::table_homogen_impl(desc.getBlockPtr(),
        t.getNumberOfRows(), t.getNumberOfColumns()) };

    t.releaseBlockOfRows(desc);
    return result;
}

template <typename KernelType, typename... Args>
auto call_kernel(Args&&... args) {
    return KernelType().compute(std::forward<Args>(args)...);
}

} // namespace interop
} // namespace detail
} // namespace dal
