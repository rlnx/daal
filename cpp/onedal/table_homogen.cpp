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

#include "onedal/table_homogen.hpp"
#include "onedal/detail/table_homogen_impl.hpp"

using std::int32_t;
using std::int64_t;

using namespace dal::detail;

namespace dal {

template<typename T>
table_homogen::table_homogen(T* data, int64_t rows, int64_t cols)
    : table(dal::detail::make_from_pointer<table>(shared<table_impl> {
        new table_homogen_impl(data, rows, cols) }))
{ }


template table_homogen::table_homogen(const float* data, int64_t rows, int64_t cols);
template table_homogen::table_homogen(const double* data, int64_t rows, int64_t cols);
template table_homogen::table_homogen(const int32_t* data, int64_t rows, int64_t cols);

template table_homogen make_table<float>(const float* data, int64_t rows, int64_t cols);
template table_homogen make_table<double>(const double* data, int64_t rows, int64_t cols);
template table_homogen make_table<int32_t>(const int32_t* data, int64_t rows, int64_t cols);

} // namespace dal
