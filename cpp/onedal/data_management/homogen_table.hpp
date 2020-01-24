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

#include "onedal/data_management/table.hpp"
#include "onedal/data_management/data_format.hpp"

namespace dal {
namespace data_management {

namespace detail {
class homogen_table_data;
} // namespace detail

class homogen_table : public table {
public:
    using pimpl = dal::detail::pimpl<detail::homogen_table_data>;

    homogen_table(const homogen_table& t)
        : table(t.get_impl())
    { }

    template<typename DataType>
    homogen_table(const DataType* data, std::int64_t rows, std::int64_t cols, data_format df);

    detail::homogen_table_data* get_impl_ptr() const noexcept {
        return reinterpret_cast<detail::homogen_table_data*>(table::get_impl_ptr());
    }
};

} // namespace data_management
} // namespace dal
