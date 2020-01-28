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
#include "onedal/memory.hpp"
#include "onedal/detail/common.hpp"

namespace dal {

template <typename T>
class array {
public:
    array(const shared<T>& data, std::int64_t size)
        : data_(data),
          size_(size)
    { }

    template <typename Deleter>
    array(T* data, std::int64_t size, Deleter d)
        : data_(data, d),
          size_(size)
    { }

    T* get_pointer() const noexcept {
        return data_.get();
    }

    std::int64_t get_size() const noexcept {
        return size_;
    }

    const T& operator [](std::int64_t index) const noexcept {
        return get_pointer()[index];
    }

private:
    shared<T> data_;
    std::int64_t size_;
};

} // namespace dal
