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
    array()
        : data_(nullptr),
          size_(0),
          capacity_(0),
          is_owner_(false) {}

    explicit array(std::int64_t element_count, T element = {})
        : data_(new T[element_count], // TODO: Use custom oneDAL allocator
                [](T* obj){ delete[] obj; }),
          size_(element_count),
          capacity_(element_count),
          is_owner_(true) {

        auto* ptr = data_.get();
        for (std::int64_t i = 0; i < size_; i++) {
            ptr[i] = element;
        }
    }

    explicit array(const detail::shared<T>& data, std::int64_t size, bool is_owner = false)
        : data_(data),
          size_(size),
          capacity_(size),
          is_owner_(is_owner) {}

    template <typename Deleter>
    explicit array(T* data, std::int64_t size, Deleter&& deleter, bool is_owner = false)
        : data_(data, std::forward<Deleter>(deleter)),
          size_(size),
          capacity_(size),
          is_owner_(is_owner) {}

    template <typename U>
    array(const array<U>& other)
        : data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_),
          is_owner_(false) {}

    T* get_pointer() const {
        return data_.get();
    }

    std::int64_t get_size() const noexcept {
        return size_;
    }

    std::int64_t get_capacity() const noexcept {
        return capacity_;
    }

    bool is_data_owner() const noexcept {
        return is_owner_;
    }

    void reset() {
        data_.reset();
        size_ = 0;
        capacity_ = 0;
    }

    void reset(std::int64_t size) {
        data_.reset(new T[size], [](T* obj){ delete[] obj; });
        size_ = size;
        capacity_ = size;
        is_owner_ = true;
    }

    template <typename Deleter>
    void reset(T* data, std::int64_t size, Deleter&& deleter, bool is_owner = false) {
        // TODO: check input parameters
        data_.reset(data, std::forward<Deleter>(deleter));
        size_ = size;
        capacity_ = size;
        is_owner_ = is_owner;
    }

    void resize(std::int64_t size) {
        if (size <= 0) {
            reset();
        } else if (!is_data_owner() || get_capacity() < size) {
            T* new_data = new T[size];
            std::int64_t min_size = std::min(size, get_size());

            for (std::int64_t i = 0; i < min_size; i++) {
                new_data[i] = (*this)[i];
            }

            reset(new_data, size, [](T* obj){ delete[] obj; }, true);
        } else {
            size_ = size;
        }
    }

    const T& operator [](std::int64_t index) const {
        return get_pointer()[index];
    }

    T& operator [](std::int64_t index) {
        return get_pointer()[index];
    }

private:
    detail::shared<T> data_;
    std::int64_t size_;
    std::int64_t capacity_;
    bool is_owner_;
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
