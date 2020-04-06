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

#include <type_traits>

#include "onedal/common.hpp"
#include "onedal/detail/common_helpers.hpp"

namespace dal {
    template <typename T>
    inline data_type make_data_type() {
        return detail::make_data_type_impl<std::decay_t<T>>();
    }

    constexpr auto get_data_type_size(data_type t) {
        if (t == data_type::float32) {
            return sizeof(float);
        }
        else if (t == data_type::float64) {
            return sizeof(double);
        }
        else if (t == data_type::int32) {
            return sizeof(int32_t);
        }
        else if (t == data_type::int64) {
            return sizeof(int64_t);
        }
        else if (t == data_type::uint32) {
            return sizeof(uint32_t);
        }
        else if (t == data_type::uint64) {
            return sizeof(uint64_t);
        }
    }
} // namespace dal
