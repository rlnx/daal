#pragma once

#include "daal/include/services/env_detect.h"

#include "onedal/backend/dispatcher.hpp"

namespace dal {
namespace backend {
namespace interop {

// template <typename DispatchId>
// constexpr daal::CpuType get_daal_cpu_type(DispatchId);

// template <>
// constexpr daal::CpuType get_daal_cpu_type<cpu_dispatch_default>(cpu_dispatch_default) {
//     return daal::CpuType::sse2;
// }

// template <>
// constexpr daal::CpuType get_daal_cpu_type<cpu_dispatch_avx>(cpu_dispatch_avx) {
//     return daal::CpuType::avx;
// }

// template <>
// constexpr daal::CpuType get_daal_cpu_type<cpu_dispatch_avx2>(cpu_dispatch_avx2) {
//     return daal::CpuType::avx2;
// }

// template <>
// constexpr daal::CpuType get_daal_cpu_type<cpu_dispatch_avx512>(cpu_dispatch_avx512) {
//     return daal::CpuType::avx512;
// }

template <typename Float, template <typename, daal::CpuType> typename CpuKernel, typename... Args>
inline auto call_daal_kernel(const context_cpu& ctx, Args&&... args) {
    return dal::backend::dispatch_by_cpu(ctx, [&](auto cpu) {
        constexpr daal::CpuType daal_cpu_type = daal::CpuType::avx512;
        // constexpr daal::CpuType daal_cpu_type = get_daal_cpu_type(cpu);
        return CpuKernel<Float, daal_cpu_type>().compute(args...);
    });
}

} // namespace interop
} // namespace backend
} // namespace dal
