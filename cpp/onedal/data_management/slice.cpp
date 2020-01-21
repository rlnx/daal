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

#include "onedal/data_management/slice.hpp"
#include "onedal/data_management/detail/slicable.hpp"
#include "onedal/data_management/detail/slice_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {

int32_t slice::get_num_rows() const noexcept {
    auto rows_total = _impl->data_owner->get_num_rows();
    //TODO: range need shortcut to select "negative" indexes
    auto final_row = _impl->rows.end_idx == _impl->rows.last_element_idx ? 
        rows_total : _impl->rows.end_idx;

    return (final_row - _impl->rows.start_idx - 1) / _impl->rows.step + 1;
}

int32_t slice::get_num_cols() const noexcept {
    auto cols_total = _impl->data_owner->get_num_cols();
    auto final_col = _impl->cols.end_idx == _impl->cols.last_element_idx ? 
        cols_total : _impl->cols.end_idx;

    return (final_col - _impl->cols.start_idx - 1) / _impl->cols.step + 1;
}

const range& slice::get_rows_range() const noexcept {
    return _impl->rows;
}

const range& slice::get_cols_range() const noexcept {
    return _impl->cols;
}

slice& slice::row(std::int32_t idx) {
    // TODO: check that index is correct
    _impl->rows = range{idx, idx+1};
    return *this;
}

slice& slice::col(std::int32_t idx) {
    // TODO: check that index is correct
    _impl->cols = range{idx, idx+1};
    return *this;
}

slice& slice::rows(range r) {
    // TODO: check that range is correct
    _impl->rows = r;
    return *this;
}

slice& slice::cols(range r) {
    // TODO: check that range is correct
    _impl->cols = r;
    return *this;
}

} // namespace data_management
} // namespace dal
