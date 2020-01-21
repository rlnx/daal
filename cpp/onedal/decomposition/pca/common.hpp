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

#include "onedal/detail/common.hpp"
#include "onedal/data_management/table.hpp"

namespace dal {

namespace decomposition {
namespace pca {

namespace detail {
struct tag {};
class params_impl;
class model_impl;
} // namespace detail

namespace method {
struct cov {};
struct svd {};
using by_default = cov;
} // namespace method

class params_base : public Base {
 public:
  using tag_t = detail::tag;
  using float_t = float;
  using method_t = method::by_default;

  params_base();

  std::int32_t get_components_count() const;
  bool get_is_deterministic() const;

  void set_components_count(std::int32_t value);
  void set_is_deterministic(bool value);

 private:
  dal::detail::Pimpl<detail::params_impl> impl_;
};

template <typename Float  = params_base::float_t,
          typename Method = params_base::method_t>
class params : public params_base {
 public:
  using float_t  = Float;
  using method_t = Method;

  auto& set_components_count(int32_t value) {
    params_base::set_components_count(value);
    return *this;
  }

  auto& set_is_deterministic(bool value) {
    params_base::set_is_deterministic(value);
    return *this;
  }
};

class model : public Base {
public:
  using Pimpl = dal::detail::Pimpl<detail::model_impl>;

public:
  model(const Pimpl& impl);

  data_management::Table get_eigenvectors() const;

private:
  dal::detail::Pimpl<detail::model_impl> impl_;
};

} // namespace pca
} // namespace decomposition
} // namespace dal
