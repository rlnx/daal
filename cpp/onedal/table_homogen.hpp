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

#include "onedal/table.hpp"

namespace dal {

namespace detail {
class table_homogen_impl;
} // namespace detail

class table_homogen : public table {
  public:
    template <typename T, typename Deleter = detail::empty_deleter<T>>
    explicit table_homogen(T* data,
                           std::int64_t row_count,
                           std::int64_t column_count,
                           Deleter&& deleter = Deleter())
        : table_homogen(array<T>{data, row_count * column_count, deleter},
                        row_count, column_count) {}

    template <typename T>
    explicit table_homogen(const array<T>& data,
                           std::int64_t row_count,
                           std::int64_t column_count)
        : table_homogen(array_cast<byte_t>(data),
                        detail::make_type_id<T>(),
                        row_count, column_count) {}

  private:
    explicit table_homogen(const array<byte_t>& data,
                           detail::type_id type_id,
                           std::int64_t row_count,
                           std::int64_t column_count);
};

template <typename T>
table_homogen allocate_table(std::int64_t row_count, std::int64_t column_count) {
    return table_homogen(array<T>(row_count * column_count), row_count, column_count);
}

} // namespace dal
