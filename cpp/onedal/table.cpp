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
#include "onedal/detail/table_impl.hpp"
#include "onedal/detail/array_impl.hpp"

using std::int32_t;
using std::int64_t;

namespace dal {

int64_t table::get_row_count() const noexcept {
    return impl_->get_num_rows();
}

int64_t table::get_column_count() const noexcept {
    return impl_->get_num_cols();
}

template <typename T, access_mode Mode>
array<T> flatten(const table& t, const range& rows, const range& columns) {
    auto t_impl = detail::get_impl_ptr(t);

    T* data = nullptr;
    data = t_impl->get_data_ptr({ rows, columns }, data);

    int64_t row_count = rows.get_num_of_elements(t_impl->get_num_rows());
    int64_t col_count = columns.get_num_of_elements(t_impl->get_num_cols());

    typename array<T>::pimpl a_impl {
        new detail::array_impl<T>(data, row_count*col_count,
        [t_impl, rows, columns](T* ptr) {
            t_impl->release_data_ptr({ rows, columns }, ptr, Mode == access_mode::write);
        })
    };

    return a_impl;
}

template array<float> flatten<float, access_mode::read>(const table&, const range& rows, const range& columns);
template array<float> flatten<float, access_mode::write>(const table&, const range& rows, const range& columns);

template array<double> flatten<double, access_mode::read>(const table&, const range& rows, const range& columns);
template array<double> flatten<double, access_mode::write>(const table&, const range& rows, const range& columns);

template array<int32_t> flatten<int32_t, access_mode::read>(const table&, const range& rows, const range& columns);
template array<int32_t> flatten<int32_t, access_mode::write>(const table&, const range& rows, const range& columns);

} // namespace dal
