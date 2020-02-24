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

#include "onedal/detail/common.hpp"

namespace dal {

template <typename T>
class array {
  template <typename U>
  friend class array;

  template <typename Y, typename U>
  friend array<Y> reinterpret_array_cast(const array<U>&);

  template <typename Y, typename U>
  friend array<Y> const_array_cast(const array<U>&);

  public:
    explicit array(std::int64_t element_count)
        : data_(new T[element_count]), // TODO: Use custom oneDAL allocator
          size_(element_count) {}

    explicit array(const detail::shared<T>& data, std::int64_t size)
        : data_(data),
          size_(size) {}

    template <typename Deleter>
    explicit array(T* data, std::int64_t size, Deleter&& deleter)
        : data_(data, deleter),
          size_(size) {}

    template <typename U>
    array(const array<U>& other)
        : data_(other.data_),
          size_(other.size_) {}

    T* get_pointer() const noexcept {
        return data_.get();
    }

    std::int64_t get_size() const noexcept {
        return size_;
    }

    void reset() {
        data_.reset();
        size_ = 0;
    }

    const T& operator [](std::int64_t index) const noexcept {
        return get_pointer()[index];
    }

    T& operator [](std::int64_t index) noexcept {
        return get_pointer()[index];
    }

private:
    detail::shared<T> data_;
    std::int64_t size_;
};

template <typename T, typename U>
inline array<T> reinterpret_array_cast(const array<U>& arr) {
    // Intentionally do not use std::reinterpret_pointer_cast, libc++ missing it
    auto p = reinterpret_cast<T*>(arr.get_pointer());
    return array<T>{std::shared_ptr<T>(arr.data_, p), arr.size_};
}

template <typename T, typename U>
inline array<T> const_array_cast(const array<U>& arr) {
    return array<T>{std::const_pointer_cast<T>(arr.data_), arr.size_};
}

template <typename T, typename U>
inline array<T> array_cast(const array<U>& arr) {
    return reinterpret_array_cast<T>(const_array_cast<std::remove_cv_t<U>>(arr));
}

} // namespace dal
