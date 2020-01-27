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
#include "onedal/types_integral.hpp"

namespace dal {
namespace detail {

enum class type_rt {
    i32,
    f32,
    f64
};

template <typename DataType>
inline type_rt make_type_rt();

template <>
inline type_rt make_type_rt<std::int32_t>() {
    return type_rt::i32;
}

template <>
inline type_rt make_type_rt<float>() {
    return type_rt::f32;
}

template <>
inline type_rt make_type_rt<double>() {
    return type_rt::f64;
}

} // namespace detail
} // namespace dal
