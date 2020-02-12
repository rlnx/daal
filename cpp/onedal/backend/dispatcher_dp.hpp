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

#include "onedal/execution_context_dp.hpp"
#include "onedal/backend/dispatcher.hpp"

namespace dal {
namespace backend {

class context_gpu {};

template <typename CpuKernel, typename GpuKernel>
struct kernel_dispatcher<CpuKernel, GpuKernel> {
    template <typename ... Args>
    auto operator() (const data_parallel_execution_context& ctx, Args&& ...args) const {
        // TODO: Dispatch to GPU
        return CpuKernel()(context_cpu{}, std::forward<Args>(args)...);
    }
};

}  // namespace backend
}  // namespace dal
