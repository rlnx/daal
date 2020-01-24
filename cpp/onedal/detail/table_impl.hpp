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
#include "onedal/detail/table_data.hpp"

namespace dal {
namespace detail {

class table_impl {
public:
    table_data_ptr data_container;
    range2d elements_to_access;

    table_impl(const table_data_ptr& cnt, const range2d& s)
        : data_container(cnt),
          elements_to_access(s)
    { }

    table_impl(const table_data_ptr& cnt)
        : data_container(cnt),
          elements_to_access({ .x = {0, -1}, .y = {0, -1} })
    { }
};

} // namespace detail
} // namespace dal
