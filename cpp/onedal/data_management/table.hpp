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
#include "onedal/data_management/slice.hpp"

namespace dal {
namespace data_management {

namespace detail {
class table_impl;
} // namespace detail

class table : public base {
public:
    using pimpl = dal::detail::pimpl<detail::table_impl>;

public:
    table(const table& table)
        : _impl(table._impl)
    { }

    table(const pimpl& impl)
        : _impl(impl)
    { }

    std::int32_t get_num_rows() const noexcept;
    std::int32_t get_num_cols() const noexcept;

    // TODO: this methods is the same with row(), rows(),
    // col(), cols() in Slice class.
    // need to generalize them?
    slice row(std::int32_t idx) const;
    slice col(std::int32_t idx) const;
    slice rows(const range& r) const;
    slice cols(const range& r) const;

    detail::table_impl* get_impl_ptr() const noexcept {
        return _impl.get();
    }

    const pimpl& get_impl() const noexcept {
        return _impl;
    }
private:
    slice::pimpl create_slice_impl(const range& rows, const range& cols) const noexcept;

private:
    pimpl _impl;
};

} // namespace data_management
} // namespace dal
