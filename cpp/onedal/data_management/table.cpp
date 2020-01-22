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

#include "onedal/data_management/table.hpp"
#include "onedal/data_management/detail/table_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {

int32_t table::get_num_rows() const noexcept {
    auto rows_total = _impl->data_container->get_num_rows();
    auto slice_rows = _impl->elements_to_access.rows;

    return slice_rows.get_num_of_elements(rows_total);
}

int32_t table::get_num_cols() const noexcept {
    auto cols_total = _impl->data_container->get_num_cols();
    auto slice_cols = _impl->elements_to_access.cols;

    return slice_cols.get_num_of_elements(cols_total);
}

table table::rows(int32_t idx) const {
    // TODO: check that index is correct.
    // How to organize error handling?
    return create_slice_impl({idx, idx+1}, {0, -1});
}

table table::rows(const range& r) const {
    // TODO: check that range is correct.
    // How to organize error handling?
    return create_slice_impl(r, {0, -1});
}

table table::cols(int32_t idx) const {
    // TODO: check that index is correct.
    // How to organize error handling?
    return create_slice_impl({0, -1}, {idx, idx+1});
}

table table::cols(const range& r) const {
    // TODO: check that range is correct.
    // How to organize error handling?
    return create_slice_impl({0, -1}, r);
}

table table::create_slice_impl(const range& rows, const range& cols) const {
    const auto slice = _impl->elements_to_access;
    const auto new_rows = intersect_borders(rows, slice.rows, get_num_rows());
    const auto new_cols = intersect_borders(cols, slice.cols, get_num_cols());

    return table::pimpl {
        new detail::table_impl {
            .data_container = _impl->data_container,
            .elements_to_access = { new_rows, new_cols }
        }
    };
}

} // namespace data_management
} // namespace dal
