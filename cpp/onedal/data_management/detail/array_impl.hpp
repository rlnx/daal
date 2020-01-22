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

#include "onedal/common.hpp"
#include "onedal/detail/common.hpp"
#include "onedal/data_management/detail/table_data.hpp"

namespace dal {
namespace data_management {
namespace detail {

// TODO: this is particular array impl for slices,
// rename it
template <typename T>
class array_impl {
public:
    array_impl(const table_data_ptr& data_origin, const slice& slice)
        : _data_origin(data_origin),
          _slice(slice) {
        _data = _data_origin->get_data_ptr(_slice, _data);
    }

    ~array_impl() {
        _data_origin->release_data_ptr(_slice, _data);
    }

    T* get_data_ptr() const noexcept {
        return _data;
    }

    std::int32_t get_size() const noexcept {
        std::int32_t num_rows = _slice.rows.get_num_of_elements(_data_origin->get_num_rows());
        std::int32_t num_cols = _slice.cols.get_num_of_elements(_data_origin->get_num_cols());

        return num_rows * num_cols;
    }

private:
    table_data_ptr _data_origin;
    slice _slice;
    T* _data;
};

} // namespace detail
} // namespace data_management
} // namespace dal
