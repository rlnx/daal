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

#pragma once

#include "onedal/memory.hpp"

namespace dal {
namespace detail {

template <typename T>
using pimpl = shared<T>;

struct pimpl_accessor {
  template <typename Object>
  auto& operator()(Object&& object) const {
    return object.impl_;
  }
};

template <typename Impl, typename Object>
Impl& get_impl(Object&& object) {
  return static_cast<Impl&>(pimpl_accessor()(object).get());
}

template <typename Object>
auto& get_impl_ptr(const Object& object) {
  return pimpl_accessor()(object);
}

}  // namespace detail
}  // namespace dal
