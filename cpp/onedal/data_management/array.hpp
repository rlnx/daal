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
template <typename T> class array_impl;
} // namespace detail


template <typename T>
class array {
public:
    using pimpl = dal::detail::pimpl< detail::array_impl<T> >;

public:
    array(const pimpl& impl)
        : _impl(impl)
    { }

    T* get_host_ptr() const noexcept;

private:
    pimpl _impl;
};

} // namespace data_management
} // namespace dal
