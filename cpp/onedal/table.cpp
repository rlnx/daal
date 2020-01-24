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

int64_t table::get_num_rows() const noexcept {
    auto rows_total = _impl->data_container->get_num_rows();
    auto slice_rows = _impl->elements_to_access.x;

    return slice_rows.get_num_of_elements(rows_total);
}

int64_t table::get_num_cols() const noexcept {
    auto cols_total = _impl->data_container->get_num_cols();
    auto slice_cols = _impl->elements_to_access.y;

    return slice_cols.get_num_of_elements(cols_total);
}

template <typename T, access_mode Mode>
array<T> flatten(const table& t, const range2d& r) {
    auto* t_impl_ptr = t.get_impl_ptr();

    typename array<T>::pimpl a_impl {
        new detail::array_impl<T>(t_impl_ptr->data_container, r)
    };

    return a_impl;
}

template array<float> flatten<float, access_mode::read>(const table&, const range2d&);
template array<float> flatten<float, access_mode::write>(const table&, const range2d&);

template array<double> flatten<double, access_mode::read>(const table&, const range2d&);
template array<double> flatten<double, access_mode::write>(const table&, const range2d&);

template array<int32_t> flatten<int32_t, access_mode::read>(const table&, const range2d&);
template array<int32_t> flatten<int32_t, access_mode::write>(const table&, const range2d&);

} // namespace dal
