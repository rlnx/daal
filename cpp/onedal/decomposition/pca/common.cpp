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

#include "onedal/decomposition/pca/common.hpp"

namespace dal {
namespace decomposition {
namespace pca {

class detail::params_impl : public base {
 public:
  std::int32_t components_count = -1;
  bool is_deterministic = false;
};

class detail::model_impl : public base {
 public:
  table eigenvectors;
};

using detail::params_impl;
using detail::model_impl;

params_base::params_base()
  : impl_(new params_impl()) { }

std::int32_t params_base::get_components_count() const {
  return impl_->components_count;
}

bool params_base::get_is_deterministic() const {
  return impl_->is_deterministic;
}

void params_base::set_components_count(std::int32_t value) {
  impl_->components_count = value;
}

void params_base::set_is_deterministic(bool value) {
  impl_->is_deterministic = value;
}

model::model()
  : impl_(new model_impl()) { }

table model::get_eigenvectors() const {
  return impl_->eigenvectors;
}

} // namespace pca
} // namespace decomposition
} // namespace dal
