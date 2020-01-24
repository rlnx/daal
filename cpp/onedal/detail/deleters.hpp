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
namespace detail {

class deleter_iface : public base {
public:
    virtual void operator()(void * ptr) = 0;
};

// TODO: custom operator() for concrete type from T
template <typename T>
class deleter_container : public deleter_iface {
public:
    deleter_container(const T& impl)
        : _impl(impl)
    { }

    virtual void operator()(void* ptr) override {
        _impl(ptr);
    }
private:
    T _impl;
};

} // namespace detail
} // namespace dal
