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

#include "onedal/backend/convert.hpp"
#include "onedal/backend/homogen_table_impl.hpp"

#include <cstring>

namespace dal::backend {

using std::int32_t;

template <typename T>
void homogen_table_impl::pull_rows_impl(array<T>& block, const range& rows) const {
    // TODO: check range correctness
    // TODO: check array size if non-zero

    const int64_t N = get_row_count();
    const int64_t p = get_column_count();
    const int64_t block_size = rows.get_element_count(N)*p;
    const data_type block_dtype = make_data_type<T>();

    if (meta_.layout != data_layout::row_major) {
        throw std::runtime_error("unsupported data layout");
    }

    if (block_dtype == finfo_.dtype) {
        auto row_data = reinterpret_cast<const T*>(data_.get_data());
        auto row_start_pointer = row_data + rows.start_idx * p;
        block.reset_not_owning(row_start_pointer, block_size);
    } else {
        if (!block.is_data_owner() || block.get_capacity() < block_size) {
            block.reset(block_size);
        } else {
            block.resize(block_size);
        }

        auto type_size = get_data_type_size(finfo_.dtype);
        auto row_start_pointer = data_.get_data() + rows.start_idx * p * type_size; 
        backend::convert_vector(row_start_pointer, block.get_mutable_data(),
                                finfo_.dtype, block_dtype, block_size);
    }
}

template <typename T>
void homogen_table_impl::push_rows_impl(const array<T>& block, const range& rows) {
    // TODO: check range correctness
    // TODO: check array size if non-zero

    const int64_t N = get_row_count();
    const int64_t p = get_column_count();
    const int64_t block_size = rows.get_element_count(N)*p;
    const data_type block_dtype = make_data_type<T>();

    if (meta_.layout != data_layout::row_major) {
        throw std::runtime_error("unsupported data layout");
    }

    data_.unique();
    if (block_dtype == finfo_.dtype) {
        auto row_data = reinterpret_cast<T*>(data_.get_mutable_data());
        auto row_start_pointer = row_data + rows.start_idx * p;

        if (row_start_pointer == block.get_data()) {
            return;
        } else {
            std::memcpy(row_start_pointer, block.get_data(), block_size * sizeof(T));
        }
    } else {
        auto type_size = get_data_type_size(finfo_.dtype);
        auto row_start_pointer = data_.get_mutable_data() + rows.start_idx * p * type_size; 

        backend::convert_vector(block.get_data(), row_start_pointer,
                                block_dtype, finfo_.dtype, block_size);
    }
}

template void homogen_table_impl::pull_rows_impl(array<float>&, const range&) const;
template void homogen_table_impl::pull_rows_impl(array<double>&, const range&) const;
template void homogen_table_impl::pull_rows_impl(array<int32_t>&, const range&) const;

template void homogen_table_impl::push_rows_impl(const array<float>&, const range&);
template void homogen_table_impl::push_rows_impl(const array<double>&, const range&);
template void homogen_table_impl::push_rows_impl(const array<int32_t>&, const range&);

} // namespace dal::backend

