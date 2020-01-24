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

#include "onedal/detail/deleters.hpp"
#include "onedal/detail/table_impl.hpp"

namespace dal {
namespace detail {

template <typename T>
class array_impl {
public:
    template <typename Deleter>
    array_impl(T* data, std::int64_t size, Deleter d)
        : _data(data),
          _size(size),
          _deleter(shared<deleter_iface>(new deleter_container<Deleter>(d)))
    { }

    ~array_impl() {
        _deleter->operator()((void*)_data);
        _data = nullptr;
    }

    T* get_data_ptr() const noexcept {
        return _data;
    }

    std::int64_t get_size() const noexcept {
        return _size;
    }

private:
    T* _data;
    std::int64_t _size;
    shared<deleter_iface> _deleter;
};

} // namespace detail
} // namespace dal
