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

#include "onedal/table.hpp"
#include "onedal/detail/common.hpp"

namespace dal {
namespace optimization {
namespace lbfgs {
namespace detail {

class solver_impl : public base {
  public:
    std::int64_t m = 0;
    std::int64_t l = 0;
    std::int64_t max_iteration_count = 0;
    std::int64_t correction_pair_batch_size = 0;
    table correction_pair_batch_indices;
    table batch_indices;
    table step_length_sequence;
    double accuracy_threshold = 0;
};

} // namespace detail
} // namespace lbfgs
} // namespace optimization
} // namespace dal
