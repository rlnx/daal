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
#include "onedal/detail/deleters.hpp"

namespace dal {

namespace detail {
template <typename T> class array_impl;
} // namespace detail

template <typename T>
class array {
public:
    using pimpl = dal::detail::pimpl< detail::array_impl<T> >;

public:
    array(const pimpl& impl)
        : impl_(impl)
    { }

    array(T* data, std::int64_t size);

    template <typename Deleter>
    array(T* data, std::int64_t size, Deleter d)
        : array(data, size, new detail::deleter_container<Deleter, T>(d))
    { }

    T* get_pointer() const noexcept;
    std::int64_t get_size() const noexcept;

    const T& operator [](std::int64_t index) const {
        // TODO: is this method really important? Pointer access occurs every call
        // TODO: if no check of index, mark as noexcept
        return get_pointer()[index];
    }

private:
    using deleter_ptr = detail::shared<detail::deleter_iface<T>>;

private:
    array(T* data, std::int64_t size, const deleter_ptr& deleter);

private:
    pimpl impl_;
};

} // namespace dal
