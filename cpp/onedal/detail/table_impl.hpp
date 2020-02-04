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

#include "onedal/detail/common.hpp"

namespace dal {
namespace detail {

template <typename T>
class slice {
  public:
    slice(const range& rows, const range& columns, access_mode mode, T* data)
        : rows_(rows), columns_(columns), mode_(mode), data_(data) {}

    T* get_data() const {
        return data_;
    }

    access_mode get_access_mode() const {
        return mode_;
    }

    const range &get_rows() const {
        return rows_;
    }

    const range &get_columns() const {
        return columns_;
    }

  private:
    range rows_;
    range columns_;
    access_mode mode_;
    T* data_;
};

class table_impl : public base {
  public:
    explicit table_impl(std::int64_t row_count, std::int64_t column_count)
        : row_count_(row_count), column_count_(column_count) {}

    std::int64_t get_num_rows() const {
        return row_count_;
    }

    std::int64_t get_num_cols() const {
        return column_count_;
    }

    template <typename T>
    slice<T> get_slice(const range& rows, const range& columns, access_mode mode) const {
        return get_slice_impl(type_id<T>{}, rows, columns);
    }

    template <typename T>
    void release_slice(const slice<T>& slice) {
        release_slice_impl(slice, data);
    }

  private:
    #define DECLARE_GET_SLICE_IMPL(T) \
        virtual slice<T> get_slice_impl(type_id<T>, const range& rows, \
                                                    const range& columns) const = 0; \
        virtual void release_slice_impl(const slice<T>& info) = 0;

    DECLARE_GET_SLICE_IMPL(float)
    DECLARE_GET_SLICE_IMPL(double)
    DECLARE_GET_SLICE_IMPL(std::int32_t)

    #undef DECLARE_GET_SLICE_IMPL

    std::int64_t row_count_;
    std::int64_t column_count_;
};

using table_impl_ptr = dal::shared<table_impl>;

} // namespace detail
} // namespace dal
