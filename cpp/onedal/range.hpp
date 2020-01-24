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
    std::int64_t start_idx;
    std::int64_t end_idx;
    std::int64_t step;

public:
    range(std::int64_t start,
          std::int64_t end,
          std::int64_t step = 1)
        : start_idx(start),
          end_idx(end),
          step(step)
    { }

    std::int64_t get_num_of_elements(std::int64_t end_of_parent) const noexcept {
        // TODO: handle error if (end_of_parent + end_idx) < 0
        auto final_row = (end_idx < 0) ? end_of_parent + end_idx + 1 : end_idx;

        return (final_row - start_idx - 1) / step + 1;
    }
};

class range2d {
public:
    range row_range;
    range col_range;
};

inline range intersect_borders(const range& r1, const range& r2, std::int64_t end_of_parent) {
    // TODO: do not work if steps are not equal to one
    // TODO: move to detail?
    auto r1_end = r1.end_idx > 0 ? r1.end_idx - end_of_parent - 1 : r1.end_idx;
    auto r2_end = r2.end_idx > 0 ? r2.end_idx - end_of_parent - 1 : r2.end_idx;

    const auto start = r1.start_idx > r2.start_idx ? r1.start_idx : r2.start_idx;
    const auto end = r1_end < r2_end ? r1_end : r2_end;
    return { start, end };
}

inline range2d row_range(const range& r) {
    return { r, {0, -1} };
}

inline range2d column_range(const range& r) {
    return { {0, -1}, r };
}

} // namespace dal
