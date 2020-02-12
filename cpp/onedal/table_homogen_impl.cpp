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

#include "onedal/table_homogen_impl.hpp"

using std::int32_t;
using std::int64_t;

namespace dal {
namespace detail {

template <typename T>
bool table_homogen_impl::can_avoid_slice_copy(const range& rows,
                                              const range& columns) const {
    const int64_t c_count = get_column_count();
    const bool no_need_conversion = type_id_ == make_type_id<T>();
    const bool full_column_range = c_count == columns.get_element_count(c_count);
    return no_need_conversion && full_column_range;
}

template <typename T>
slice<T> table_homogen_impl::get_slice_template(const range& rows,
                                                const range& columns,
                                                access_mode mode) const {
    if (can_avoid_slice_copy<T>(rows, columns)) {
        const int64_t elements_offset = rows.start_idx * get_column_count();
        T* data_with_offset = (T*)(data_.get_pointer() + elements_offset * sizeof(T));
        return slice<T>(const_cast<table_homogen_impl*>(this),
                        data_with_offset, rows, columns, mode);
    }

    // TODO: Handle access_mode::write
    // TODO: Handle case where a copy is necessary
    return slice<T>(const_cast<table_homogen_impl*>(this));
}

template <typename T>
void table_homogen_impl::release_slice_template(const slice<T>& slice) {
    // TODO
}

#define DEFINE_GET_SLICE_IMPL(T) \
    slice<T> table_homogen_impl::get_slice_impl(type_tag<T>,              \
                                                const range& rows,        \
                                                const range& columns,     \
                                                access_mode mode) const { \
        return get_slice_template<T>(rows, columns, mode);                \
    }                                                                     \
    void table_homogen_impl::release_slice_impl(const slice<T> &slice) {  \
        return release_slice_template<T>(slice);                          \
    }

DEFINE_GET_SLICE_IMPL(float)
DEFINE_GET_SLICE_IMPL(double)
DEFINE_GET_SLICE_IMPL(std::int32_t)

} // namespace detail
} // namespace dal
