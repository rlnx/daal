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
class slice_impl;
} // namespace detail

class slice : public base {
public:
    using pimpl = dal::detail::pimpl<detail::slice_impl>;

public:
    slice(const pimpl& impl)
        : _impl(impl)
    { }

    std::int32_t get_num_rows() const noexcept;
    std::int32_t get_num_cols() const noexcept;
    const range& get_rows_range() const noexcept;
    const range& get_cols_range() const noexcept;

    // TODO: too big exposure of details
    // need to make SliceImpl?
    const pimpl& get_impl() const noexcept {
        return _impl;
    }

    slice& row(std::int32_t idx);
    slice& col(std::int32_t idx);
    slice& rows(range r);
    slice& cols(range r);

private:
    pimpl _impl;
};

} // namespace data_management
} // namespace dal
