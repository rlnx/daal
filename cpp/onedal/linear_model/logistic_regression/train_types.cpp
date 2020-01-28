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

#include "onedal/linear_model/logistic_regression/train_types.hpp"
#include "onedal/linear_model/logistic_regression/detail/train_types_impl.hpp"

namespace dal {
namespace linear_model {
namespace logistic_regression {

using detail::train_input_impl;
using detail::train_result_impl;

train_input::train_input(const table& data,
                         const table& labels)
    : impl_(new train_input_impl{data, labels}) {}

void train_input::set_data_impl(const table& data) {
    impl_->data = data;
}

void train_input::set_labels_impl(const table& lables) {
    impl_->labels = lables;
}

train_result::train_result()
    : impl_(new train_result_impl{}) {}

model train_result::get_model() const {
    return impl_->trained_model;
}

} // namespace logistic_regression
} // namespace linear_model
} // namespace dal
