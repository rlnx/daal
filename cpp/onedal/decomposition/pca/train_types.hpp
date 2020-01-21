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

#include "onedal/decomposition/pca/common.hpp"

namespace dal {
namespace decomposition {
namespace pca {

namespace detail {
class train_input_impl;
class train_result_impl;
} // namespace detail

class train_input : public Base {
 public:
  train_input(const table& data);

  auto get_data() const -> table;

  auto& set_data(const table& data) {
    set_data_impl(data);
    return *this;
  }

 private:
  void set_data_impl(const table& data);
  dal::detail::Pimpl<detail::train_input_impl> impl_;
};

class train_result {
 public:
  train_result();

  auto get_model() const -> model;
  auto get_eigenvalues() const -> table;
  auto get_eigenvectors() const -> table;
  auto get_explained_variance() const -> table;

 private:
  dal::detail::Pimpl<detail::train_result_impl> impl_;
};

} // namespace pca
} // namespace decomposition
} // namespace dal
