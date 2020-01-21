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
#include "onedal/data_management/detail/slice_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {

int32_t table::get_num_rows() const noexcept {
    return _impl->get_num_rows();
}

int32_t table::get_num_cols() const noexcept {
    return _impl->get_num_cols();
}

slice table::row(int32_t idx) const {
    // TODO: check that index is correct.
    // How to organize error handling?
    return slice{ create_slice_impl({idx, idx+1}, {0, -1}) };
}

slice table::col(int32_t idx) const {
    // TODO: check that index is correct.
    // How to organize error handling?
    return slice{ create_slice_impl({0, -1}, {idx, idx+1}) };
}

slice table::rows(const range& r) const {
    // TODO: check that range is correct.
    // How to organize error handling?
    return slice{ create_slice_impl(r, {0, -1}) };
}
slice table::cols(const range& r) const {
    // TODO: check that range is correct.
    // How to organize error handling?
    return slice{ create_slice_impl({0, -1}, r) };
}

slice::pimpl table::create_slice_impl(const range& rows, const range& cols) const noexcept {
    return slice::pimpl {
        new detail::slice_impl {
            .data_owner = _impl,
            .rows = rows,
            .cols = cols
        }
    };
}

} // namespace data_management
} // namespace dal
