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

#include "onedal/table.hpp"

namespace dal {

namespace detail {
class table_homogen_impl;
} // namespace detail

class table_homogen : public table {
public:
    using pimpl = dal::detail::pimpl<detail::table_homogen_impl>;

    template<typename DataType>
    table_homogen(const DataType* data, std::int64_t rows, std::int64_t cols);

private:
    friend detail::pimpl_accessor;
};

template <typename DataType>
table_homogen make_table(const DataType* data,
                         std::int64_t rows, std::int64_t cols);

} // namespace dal
