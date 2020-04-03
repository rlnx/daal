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

namespace dal::detail {

template <typename T> inline data_type make_data_type_impl();

template<> inline data_type make_data_type_impl<std::int32_t>() {
    return data_type::int32;
}

template<> inline data_type make_data_type_impl<std::int64_t>() {
    return data_type::int64;
}

template<> inline data_type make_data_type_impl<std::uint32_t>() {
    return data_type::uint32;
}

template<> inline data_type make_data_type_impl<std::uint64_t>() {
    return data_type::uint64;
}

template<> inline data_type make_data_type_impl<float>() {
    return data_type::float32;
}

template<> inline data_type make_data_type_impl<double>() {
    return data_type::float64;
}

} // namespace dal::detail
