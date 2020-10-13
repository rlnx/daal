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

#include <array>
#include <numeric>

#include "oneapi/dal/array.hpp"

namespace oneapi::dal::backend::ndarray {

enum class order {
    c,
    f
};

template <int Dim>
class shape {
    static_assert(Dim == 1 || Dim == 2);

public:
    shape() {
        dims_.fill(0L);
    }

    template <int D = Dim, typename = std::enable_if_t<D == 1>>
    shape(std::int64_t count) : dims_({count}) {}

    template <int D = Dim, typename = std::enable_if_t<D == 2>>
    shape(std::int64_t row_count, std::int64_t column_count)
            : dims_({row_count, column_count}) {}

    std::int64_t operator[](std::int64_t i) const {
        ONEDAL_ASSERT(i >= 0 && i < Dim);
        return dims_[i];
    }

    std::int64_t product() const {
        if constexpr (Dim == 1) {
            return dims_[0];
        }
        else if constexpr (Dim == 2) {
            // TODO: Validate overflow
            return dims_[0] * dims_[1];
        }
        else {
            ONEDAL_ASSERT(!"Unreachable");
            return -1;
        }
    }

    bool operator==(const shape& other) const {
        if constexpr (Dim == 1) {
            return dims_[0] == other.dims_[0];
        }
        else if constexpr (Dim == 2) {
            return dims_[0] == other.dims_[0] && dims_[1] == other.dims_[1];
        }
        else {
            ONEDAL_ASSERT(!"Unreachable");
            return false;
        }
    }

    bool operator!=(const shape& other) const {
        return !(*this == other);
    }

private:
    std::array<std::int64_t, Dim> dims_;
};

template <int Dim>
class array_base {
    static_assert(Dim == 1 || Dim == 2);

public:
    const shape<Dim>& get_shape() const {
        return shape_;
    }

    std::int64_t get_count() const {
        return shape_.product();
    }

    template <int D = Dim, typename = std::enable_if_t<D == 2>>
    std::int64_t get_row_count() const {
        return shape_[0];
    }

    template <int D = Dim, typename = std::enable_if_t<D == 2>>
    std::int64_t get_column_count() const {
        return shape_[1];
    }

protected:
    array_base() = default;

    explicit array_base(const shape<Dim>& s)
            : shape_(s) {}

private:
    shape<Dim> shape_;
};

template <typename T>
class array_container {
public:
    const array<T>& get_linear() const {
        return linear_;
    }

    array<T>& get_linear() {
        return linear_;
    }

    const T* get_data() const {
        return linear_.get_data();
    }

    T* get_mutable_data() const {
        return linear_.get_mutable_data();
    }

protected:
    array_container() = default;

    explicit array_container(const dal::array<T>& linear)
            : linear_(linear) {}

private:
    dal::array<T> linear_;
};

template <int Dim, order Order>
class array_index {
    static_assert(Dim == 1 || Dim == 2);

public:
    template <int D = Dim, typename = std::enable_if_t<D == 1>>
    std::int64_t get_linear_index(std::int64_t i) const {
        return i;
    }

    template <int D = Dim, typename = std::enable_if_t<D == 2>>
    std::int64_t get_linear_index(std::int64_t i, std::int64_t j) const {
        if constexpr (Order == order::c) {
            return i * strides_[0] + j;
        }
        else {
            return j * strides_[0] + i;
        }
    }

protected:
    array_index() {
        strides_.fill(0L);
    }

    template <int D = Dim, typename std::enable_if_t<D == 1, int>* = nullptr>
    explicit array_index(const shape<Dim>& s) {}

    template <int D = Dim, typename std::enable_if_t<D == 2, int>* = nullptr>
    explicit array_index(const shape<Dim>& s) {
        if constexpr (Order == order::c) {
            strides_ = { s[1] };
        }
        else {
            strides_ = { s[0] };
        }
    }

private:
    std::array<std::int64_t, Dim - 1> strides_;
};

template <typename T, int Dim, order Order = order::c>
class array : public array_base<Dim>,
              public array_index<Dim, Order>,
              public array_container<T> {
    static_assert(Dim == 1 || Dim == 2);
private:
    using base = array_base<Dim>;
    using index = array_index<Dim, Order>;
    using container = array_container<T>;

public:
    array() = default;

    explicit array(const dal::array<T>& linear, const shape<Dim>& s)
            : base(s), index(s), container(linear) {}

    template <typename... Indices>
    T get(Indices ...indices) const {
        return container::get_data()[index::get_linear_index(indices...)];
    }

    template <typename... Indices>
    T& set(Indices ...indices) {
        return container::get_mutable_data()[index::get_linear_index(indices...)];
    }
};

template <typename T, int D, order O>
auto transpose(const array<T, D, O>& x) {
    const auto linear = x.get_linear();
    const auto rshape = x.get_shape().reverse();
    if constexpr (O == order::c) {
        return array<T, D, order::f>(linear, rshape);
    }
    else {
        return array<T, D, order::c>(linear, rshape);
    }
}

} // namespace oneapi::dal::backend::ndarray
