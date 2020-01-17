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

#include "onedal/decomposition/pca/train_types.hpp"

namespace dal {
namespace decomposition {
namespace pca {

namespace backend {
template <typename... Options>
struct train_kernel {
  auto operator()(const default_execution_context&,
                  const params_base&,
                  const train_input&) const -> train_result;

  auto operator()(const data_parallel_execution_context&,
                  const params_base&,
                  const train_input&) const -> train_result;
};
} // namespace backend

namespace detail {

template <typename Params>
struct train_ops {
  using float_t = typename Params::float_t;
  using method_t = typename Params::method_t;
  using input_t = train_input;
  using result_t = train_result;
  using params_base_t = params_base;

  void validate(const Params& params, const train_input& input) const {

  }

  template <typename Context>
  auto operator()(const Context& ctx, const Params& params, const train_input& input) const {
    validate(params, input);
    return backend::train_kernel<float_t, method_t>()(ctx, params, input);
  }
};

} // namespace detail
} // namespace dal
} // namespace decomposition
} // namespace pca
