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

#include "onedal/decomposition/pca/train_types.hpp"
#include "onedal/decomposition/pca/detail/train_types_impl.hpp"

namespace dal {
namespace decomposition {
namespace pca {

using detail::train_input_impl;
using detail::train_result_impl;

train_input::train_input(const table& data)
    : impl_(new train_input_impl(data)) {}

auto train_input::get_data() const -> table {
  return impl_->data;
}

void train_input::set_data_impl(const table& data) {
  impl_->data = data;
}

train_result::train_result(const pimpl& impl)
    : impl_(impl) {}

auto train_result::get_model() const -> model {
  return impl_->trained_model;
}

auto train_result::get_eigenvalues() const -> table {
  return impl_->eigenvalues;
}

auto train_result::get_eigenvectors() const -> table {
  return impl_->eigenvectors;
}

auto train_result::get_explained_variance() const -> table {
  return impl_->explained_variance;
}

} // namespace pca
} // namespace decomposition
} // namespace dal
