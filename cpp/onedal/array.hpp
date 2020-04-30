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

#include <variant>

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
        : data_owned_ptr_(nullptr),
          size_(0),
          capacity_(0) {}

    explicit array(std::int64_t element_count, T element = {})
        : data_owned_ptr_(new T[element_count], // TODO: Use custom oneDAL allocator
                [](T* obj){ delete[] obj; }),
          data_(data_owned_ptr_.get()),
          size_(element_count),
          capacity_(element_count) {
        for (std::int64_t i = 0; i < size_; i++) {
            (*this)[i] = element;
        }
    }

    template <typename Deleter>
    explicit array(T* data, std::int64_t size, Deleter&& deleter)
        : data_owned_ptr_(data, std::forward<Deleter>(deleter)),
          data_(data),
          size_(size),
          capacity_(size) {}

    template <typename U>
    array(const array<U>& other)
        : data_owned_ptr_(other.data_owned_ptr_),
          data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_) {}

    T* get_mutable_data() const {
        return std::get<T*>(data_);
    }

    const T* get_data() const {
        if (auto ptr_val = std::get_if<T*>(&data_)) {
            return *ptr_val;
        } else {
            return std::get<const T*>(data_);
        }
    }

    bool has_mutable_data() const {
        return std::holds_alternative<T*>(data_) && (get_mutable_data() != nullptr);
    }

    void unique() {
        if (is_data_owner() || size_ == 0) {
            return;
        } else {
            auto immutable_data = get_data();
            auto copy_data = new T[size_];

            for (std::int64_t i = 0; i < size_; i++) {
                copy_data[i] = immutable_data[i];
            }

            reset(copy_data, size_, [](auto obj){ delete[] obj; });
        }
    }

    std::int64_t get_size() const {
        return size_;
    }

    std::int64_t get_capacity() const {
        return capacity_;
    }

    bool is_data_owner() const {
        if (data_owned_ptr_ == nullptr) {
            return false;
        } else if (auto ptr_val = std::get_if<T*>(&data_)) {
            return *ptr_val == data_owned_ptr_.get();
        } else if (auto ptr_val = std::get_if<const T*>(&data_)) {
            return *ptr_val == data_owned_ptr_.get();
        } else {
            return false;
        }
    }

    void reset() {
        data_owned_ptr_.reset();
        data_ = std::variant<T*, const T*>();
        size_ = 0;
        capacity_ = 0;
    }

    void reset(std::int64_t size) {
        data_owned_ptr_.reset(new T[size], [](auto obj){ delete[] obj; });
        data_ = data_owned_ptr_.get();
        size_ = size;
        capacity_ = size;
    }

    template <typename Deleter>
    void reset(T* data, std::int64_t size, Deleter&& deleter) {
        // TODO: check input parameters
        data_owned_ptr_.reset(data, std::forward<Deleter>(deleter));
        data_ = data_owned_ptr_.get();
        size_ = size;
        capacity_ = size;
    }

    template <typename U = T*>
    void reset_not_owning(U data = nullptr, std::int64_t size = 0) {
        data_ = data;
        size_ = size;
    }

    void resize(std::int64_t size) {
        if (is_data_owner() == false) {
            throw std::runtime_error("cannot resize array with non-owning data");
        } else if (size <= 0) {
            reset_not_owning();
        } else if (get_capacity() < size) {
            T* new_data = new T[size];
            std::int64_t min_size = std::min(size, get_size());

            for (std::int64_t i = 0; i < min_size; i++) {
                new_data[i] = (*this)[i];
            }

            reset(new_data, size, [](T* obj){ delete[] obj; });
        } else {
            size_ = size;
        }
    }

    const T& operator [](std::int64_t index) const {
        return get_data()[index];
    }

    T& operator [](std::int64_t index) {
        return get_mutable_data()[index];
    }

private:
    detail::shared<T> data_owned_ptr_;
    std::variant<T*, const T*> data_;

    std::int64_t size_;
    std::int64_t capacity_;
};

template <typename T, typename U>
inline array<T> reinterpret_array_cast(const array<U>& arr) {
    // Intentionally do not use std::reinterpret_pointer_cast, libc++ missing it
    auto p = reinterpret_cast<T*>(arr.get_mutable_data());
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
