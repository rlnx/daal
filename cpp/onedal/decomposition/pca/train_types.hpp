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

class train_input : public base {
 public:
  train_input(const data_management::table& data);

  data_management::table get_data() const;

  auto& set_data(const data_management::table& data) {
    set_data_impl(data);
    return *this;
  }

 private:
  void set_data_impl(const data_management::table& data);
  dal::detail::pimpl<detail::train_input_impl> impl_;
};

class train_result {
 public:
  using pimpl = dal::detail::pimpl<detail::train_result_impl>;

 public:
  train_result(const pimpl& impl);

  auto get_model() const -> model;
  data_management::table get_eigenvalues() const;
  data_management::table get_eigenvectors() const;
  data_management::table get_explained_variance() const;

 private:
  dal::detail::pimpl<detail::train_result_impl> impl_;
};

} // namespace pca
} // namespace decomposition
} // namespace dal
