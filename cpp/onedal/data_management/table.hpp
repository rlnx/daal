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
#include "onedal/access_mode.hpp"
#include "onedal/data_management/array.hpp"
#include "onedal/detail/common.hpp"

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

    std::int64_t get_num_rows() const noexcept;
    std::int64_t get_num_cols() const noexcept;

    detail::table_impl* get_impl_ptr() const noexcept {
        return _impl.get();
    }

    const pimpl& get_impl() const noexcept {
        return _impl;
    }

private:
    pimpl _impl;
};

template <typename T, access_mode Mode>
array<T> flatten(const table& t, const range2d& r);

} // namespace data_management
} // namespace dal
