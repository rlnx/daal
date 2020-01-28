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

#include "onedal/execution_context.hpp"
#include "onedal/backend/dispatcher.hpp"
#include "onedal/linear_model/logistic_regression/detail/train_ops.hpp"
#include "onedal/linear_model/logistic_regression/backend/cpu/train_kernel.hpp"

namespace dal {
namespace linear_model {
namespace logistic_regression {
namespace detail {

template <typename Float, typename Method>
struct train_ops_dispatcher<default_execution_context, Float, Method> {
  train_result operator()(const default_execution_context& ctx,
                          const estimator_base& estimator,
                          const train_input& input) const {
    return dal::backend::dispatch(ctx, [&](auto cpu) {
        return train_result{};
    //   return backend::train_kernel<decltype(cpu), Float, Method>()(ctx, params, input);
    });
  }
};

#define INSTANTIATE(F, M) \
  template struct train_ops_dispatcher<default_execution_context, F, M>;

INSTANTIATE(float, method::solver)
INSTANTIATE(double, method::solver)

} // namespace detail
} // namespace logistic_regression
} // namespace linear_model
} // namespace dal
