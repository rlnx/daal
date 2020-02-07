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

#include "onedal/table_impl.hpp"

namespace dal {
namespace detail {

template <typename T>
struct empty_deleter {
    void operator() (T*) const {}
};

class table_homogen_impl : public table_impl {
  public:
    template <typename T>
    explicit table_homogen_impl(T* data,
                                std::int64_t row_count,
                                std::int64_t column_count)
        : table_impl(row_count, column_count),
          type_id_(make_type_id<T>()),
          data_((char *)data, empty_deleter<char>()) {}

  protected:
    #define DECLARE_GET_SLICE_IMPL(T) \
        slice<T> get_slice_impl(type_tag<T>, const range& rows, \
                                             const range& columns, \
                                             access_mode mode) const override; \
        void release_slice_impl(const slice<T>& slice) override;

    DECLARE_GET_SLICE_IMPL(float)
    DECLARE_GET_SLICE_IMPL(double)
    DECLARE_GET_SLICE_IMPL(std::int32_t)

    #undef DECLARE_GET_SLICE_IMPL

  private:
    template <typename T>
    slice<T> get_slice_template(const range& rows,
                                const range& columns,
                                access_mode mode) const;

    template <typename T>
    void release_slice_template(const slice<T>& slice);

    template <typename T>
    bool can_avoid_slice_copy(const range& rows,
                              const range& columns) const;

    type_id type_id_;
    shared<char> data_;
};

} // namespace detail
} // namespace dal
