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

#include "onedal/decomposition/pca/backend/cpu/train_kernel.hpp"

namespace dal {
namespace decomposition {
namespace pca {
namespace backend {

template <typename Cpu, typename Float>
static train_result train_svd(const default_execution_context& ctx,
                              const params_base& params,
                              const train_input& input) {
  return train_result();
};

template <typename Cpu, typename Float>
struct train_kernel<Cpu, Float, method::svd> {
  train_result operator()(const default_execution_context& ctx,
                          const params_base& params,
                          const train_input& input) const {
    return train_svd<Cpu, Float>(ctx, params, input);
  }
};

template struct train_kernel<DAL_CPU_ID_, float, method::svd>;
template struct train_kernel<DAL_CPU_ID_, double, method::svd>;

} // namespace backend
} // namespace pca
} // namespace decomposition
} // namespace dal
