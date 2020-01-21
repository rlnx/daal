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
    return _impl->get_num_rows();
}

int32_t table::get_num_cols() const noexcept {
    return _impl->get_num_cols();
}

slice table::row(int32_t idx) const {
    // TODO: check that index is correct.
    // How to organize error handling?
    return slice(_impl, range{idx, idx+1}, range{0, -1});
}

slice table::col(int32_t idx) const {
    // TODO: check that index is correct.
    // How to organize error handling?
    return slice(_impl, range{0, -1}, range {idx, idx+1});
}

slice table::rows(const range& r) const {
    // TODO: check that range is correct.
    // How to organize error handling?
    return slice(_impl, r, range{0, -1});
}
slice table::cols(const range& r) const {
    // TODO: check that range is correct.
    // How to organize error handling?
    return slice(_impl, range{0, -1}, r);
}

} // namespace data_management
} // namespace dal
