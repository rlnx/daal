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

#include "onedal/linear_model/logistic_regression/estimator.hpp"

namespace dal {
namespace linear_model {
namespace logistic_regression {
namespace detail {

class train_input_impl : public base {
  public:
    explicit train_input_impl(const table& data,
                              const table& labels)
        : data(data), labels(labels) {}

    table data;
    table labels;
};

class train_result_impl : public base {
  public:
    model trained_model;
};

} // namespace detail
} // namespace logistic_regression
} // namespace linear_model
} // namespace dal
