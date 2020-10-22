/*******************************************************************************
* Copyright 2020 Intel Corporation
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

#include "oneapi/dal/algo/knn/train_types.hpp"
#include "oneapi/dal/detail/common.hpp"

namespace oneapi::dal::knn {

namespace detail {
template <typename Task>
class train_input_impl : public base {
public:
    train_input_impl(const table& data, const table& labels) : data(data), labels(labels) {}

    table data;
    table labels;
};

template <typename Task>
class train_result_impl : public base {
public:
    model<Task> trained_model;
};
} // namespace detail

using detail::train_input_impl;
using detail::train_result_impl;

template <typename Task>
train_input<Task>::train_input(const table& data, const table& labels)
        : impl_(new train_input_impl<Task>(data, labels)) {}

template <typename Task>
const table& train_input<Task>::get_data() const {
    return impl_->data;
}

template <typename Task>
const table& train_input<Task>::get_labels() const {
    return impl_->labels;
}

template <typename Task>
void train_input<Task>::set_data_impl(const table& value) {
    impl_->data = value;
}

template <typename Task>
void train_input<Task>::set_labels_impl(const table& value) {
    impl_->labels = value;
}

template <typename Task>
train_result<Task>::train_result() : impl_(new train_result_impl{}) {}

template <typename Task>
const model<Task>& train_result<Task>::get_model() const {
    return impl_->trained_model;
}

template <typename Task>
void train_result<Task>::set_model_impl(const model<Task>& value) {
    impl_->trained_model = value;
}

template class ONEDAL_EXPORT train_input<task::classification>;
template class ONEDAL_EXPORT train_result<task::classification>;

} // namespace oneapi::dal::knn
