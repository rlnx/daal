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

#include "onedal/linear_model/logistic_regression/train_types.hpp"

namespace dal {
namespace linear_model {
namespace logistic_regression {
namespace detail {

template <typename Context, typename... Options>
struct train_ops_dispatcher {
  train_result operator()(const Context&,
                          const estimator_base&,
                          const train_input&) const;
};

template <typename Estimator>
struct train_ops {
  using float_t = typename Estimator::float_t;
  using method_t = typename Estimator::method_t;
  using input_t = train_input;
  using result_t = train_result;
  using estimator_base_t = estimator_base;

  void validate(const Estimator& estimator, const train_input& input) const {

  }

  template <typename Context>
  auto operator()(const Context& ctx, const Estimator& estimator, const train_input& input) const {
    validate(estimator, input);
    return train_ops_dispatcher<Context, float_t, method_t>()(ctx, estimator, input);
  }
};

} // namespace detail
} // namespace dal
} // namespace linear_model
} // namespace logistic_regression
