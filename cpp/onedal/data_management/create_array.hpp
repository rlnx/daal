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

//TODO: reimplement that not to include details here
#include "onedal/data_management/detail/create_array.hpp"

namespace dal {

template <typename T, typename... Args>
auto create_array(Args&&... args) {
    return data_management::detail::create_array<T>(std::forward<Args>(args)...);
}

} // namespace dal
