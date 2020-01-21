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
#include "onedal/integral_types.hpp"

namespace dal {

  // TODO: think of how to semantically distinguish
  // "simple" and "complex" types
class range {
public:
    static constexpr std::int32_t first_element_idx = 0;
    static constexpr std::int32_t last_element_idx = -1;

    std::int32_t start_idx;
    std::int32_t end_idx;
    std::int32_t step;

    range(std::int32_t start,
          std::int32_t end,
          std::int32_t step = 1)
        : start_idx(start),
          end_idx(end),
          step(step)
    { }
};

} // namespace dal
