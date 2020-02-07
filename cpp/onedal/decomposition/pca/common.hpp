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
#include "onedal/table.hpp"

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

class descriptor_base : public base {
 public:
  using tag_t = detail::tag;
  using float_t = float;
  using method_t = method::by_default;

  descriptor_base();

  std::int64_t get_components_count() const;
  bool get_is_deterministic() const;

  void set_components_count(std::int64_t value);
  void set_is_deterministic(bool value);

 private:
  dal::detail::pimpl<detail::params_impl> impl_;
};

template <typename Float  = descriptor_base::float_t,
          typename Method = descriptor_base::method_t>
class descriptor : public descriptor_base {
 public:
  using float_t  = Float;
  using method_t = Method;

  auto& set_components_count(int64_t value) {
    descriptor_base::set_components_count(value);
    return *this;
  }

  auto& set_is_deterministic(bool value) {
    descriptor_base::set_is_deterministic(value);
    return *this;
  }
};

class model : public base {
public:
  table get_eigenvectors() const;

private:
  using pimpl = dal::detail::pimpl<detail::model_impl>;

private:
  model(const pimpl& impl)
      : impl_(impl) { }

private:
  dal::detail::pimpl<detail::model_impl> impl_;

private:
  friend dal::detail::pimpl_accessor;
};

} // namespace pca
} // namespace decomposition
} // namespace dal
