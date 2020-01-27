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

#include "onedal/types_data.hpp"
#include "onedal/detail/common.hpp"

namespace dal {
namespace detail {

template <typename T>
class deleter_iface : public base {
public:
    virtual void operator()(T* ptr) = 0;
};

template <typename Deleter, typename T>
class deleter_container : public deleter_iface<T> {
public:
    deleter_container(const Deleter& impl)
        : _impl(impl)
    { }

    virtual void operator()(T* ptr) override {
        _impl(ptr);
    }
private:
    Deleter _impl;
};

template <typename T>
class empty_deleter : public deleter_iface<T> {
public:
    virtual void operator()(T* ptr) override
    { }
};

} // namespace detail
} // namespace dal
