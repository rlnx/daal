/*******************************************************************************
 * Copyright 2014-2019 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#pragma once

#include "onedal/execution_context.hpp"

#if !defined(DAL_CPU_ID_)
#define DAL_CPU_ID_ dal::backend::cpu_dispatch_default
#endif

#if !defined(DAL_GPU_ID_)
#define DAL_GPU_ID_ dal::backend::gpu_dispatch_default
#endif

namespace dal {
namespace backend {

struct cpu_dispatch_default {};
struct cpu_dispatch_avx {};
struct cpu_dispatch_avx2 {};
struct cpu_dispatch_avx512 {};
struct gpu_dispatch_default {};

inline bool is_cpu_extension_enabled(cpu_extensions mask, cpu_extensions flag) noexcept {
  return ((uint64_t)mask & (uint64_t)flag) > 0;
}

template <typename Op>
inline auto dispatch(const default_execution_context& ctx, const Op& op) {
  const cpu_extensions mask = ctx.get_enabled_cpu_extensions();
  if (is_cpu_extension_enabled(mask, cpu_extensions::avx512)) {
    return op(cpu_dispatch_avx512());
  }
  else if (is_cpu_extension_enabled(mask, cpu_extensions::avx2)) {
    return op(cpu_dispatch_avx2());
  }
  else if (is_cpu_extension_enabled(mask, cpu_extensions::avx)) {
    return op(cpu_dispatch_avx());
  }
  return op(cpu_dispatch_default());
}

template <typename Op>
inline auto dispatch(const data_parallel_execution_context& ctx, const Op& op) {
  static_assert("Not implemeneted");
}

}  // namespace backend
}  // namespace dal
