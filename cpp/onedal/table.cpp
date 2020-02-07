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

#include "onedal/table.hpp"

#include "onedal/array.hpp"
#include "onedal/table_impl.hpp"

namespace dal {

table::table(detail::table_impl* impl)
    : impl_(impl) {}

std::int64_t table::get_row_count() const noexcept {
    return impl_->get_row_count();
}

std::int64_t table::get_column_count() const noexcept {
    return impl_->get_column_count();
}

template <typename T>
struct slice_array_deleter {
    explicit slice_array_deleter(const detail::slice<T>& slice)
        : slice(slice) {}

    void operator() (T*) const {
        slice.get_table()->release_slice(slice);
    }

    detail::slice<T> slice;
};

template <typename T, access_mode Mode>
array<T> flatten(const table& t, const range& rows, const range& columns) {
    auto& table_impl = detail::get_impl<detail::table_impl>(t);
    auto slice = table_impl.get_slice<T>(rows, columns, Mode);
    return array<T>{slice.get_data(), slice.get_element_count(), slice_array_deleter<T>(slice)};
}

#define INSTANTIATE(T, M) \
    template array<T> flatten<T, M>(const table&, const range&, const range&);

INSTANTIATE(float, access_mode::read)
INSTANTIATE(float, access_mode::write)

INSTANTIATE(double, access_mode::read)
INSTANTIATE(double, access_mode::write)

INSTANTIATE(std::int32_t, access_mode::read)
INSTANTIATE(std::int32_t, access_mode::write)

} // namespace dal
