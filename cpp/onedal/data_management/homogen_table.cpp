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

#include "onedal/data_management/homogen_table.hpp"
#include "onedal/data_management/detail/homogen_table_impl.hpp"
#include "onedal/data_management/detail/table_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {

template<typename DataType>
homogen_table::homogen_table(const DataType* data, int32_t rows, int32_t cols, data_format df)
    : table(table::pimpl(new detail::table_impl {
        .data_container = detail::table_data_ptr { new detail::homogen_table_impl(data, rows, cols, df) }
    }))
{ }

template homogen_table::homogen_table<float>(const float* data, int32_t rows, int32_t cols, data_format df);
template homogen_table::homogen_table<double>(const double* data, int32_t rows, int32_t cols, data_format df);
template homogen_table::homogen_table<std::int32_t>(const std::int32_t* data, int32_t rows, int32_t cols, data_format df);

} // namespace data_management
} // namespace dal
