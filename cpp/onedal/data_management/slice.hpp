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

namespace dal {
namespace data_management {

namespace detail {
class slicable;
} // namespace detail

// TODO: think of how to semantically distinguish
// "simple" and "complex" types
class slice : public base {
public:
    using pimpl_data_owner = dal::detail::pimpl<detail::slicable>;

public:
    slice(const pimpl_data_owner& impl, range rows_range, range cols_range)
        : _data_owner(impl),
          _rows(rows_range),
          _cols(cols_range)
    { } // TODO: check that ranges are correct

    std::int32_t get_num_rows() const noexcept;
    std::int32_t get_num_cols() const noexcept;

    const range& get_rows_range() const noexcept {
        return _rows;
    }

    const range& get_cols_range() const noexcept {
        return _cols;
    }

    // TODO: too big exposure of details
    // need to make SliceImpl?
    pimpl_data_owner get_data_owner_impl() const noexcept {
        return _data_owner;
    }

    slice& row(std::int32_t idx);
    slice& col(std::int32_t idx);
    slice& rows(range r);
    slice& cols(range r);

private:
    pimpl_data_owner _data_owner;
    range _rows;
    range _cols;
};

} // namespace data_management
} // namespace dal
