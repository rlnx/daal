#pragma once

#include "daal/include/services/env_detect.h"

#include "onedal/backend/dispatcher.hpp"

namespace dal {
namespace backend {
namespace interop {

template <typename CpuType>
constexpr daal::CpuType get_daal_cpu_type();

template <>
constexpr daal::CpuType get_daal_cpu_type<cpu_dispatch_default>() {
    return daal::CpuType::sse2;
}

template <>
constexpr daal::CpuType get_daal_cpu_type<cpu_dispatch_avx>() {
    return daal::CpuType::avx;
}

template <>
constexpr daal::CpuType get_daal_cpu_type<cpu_dispatch_avx2>() {
    return daal::CpuType::avx2;
}

template <>
constexpr daal::CpuType get_daal_cpu_type<cpu_dispatch_avx512>() {
    return daal::CpuType::avx512;
}

template <typename KernelType, typename... Args>
auto call_kernel(Args&&... args) {
    return KernelType().compute(std::forward<Args>(args)...);
}

} // namespace interop
} // namespace detail
} // namespace dal
