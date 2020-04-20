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
#include "onedal/detail/homogen_table_impl.hpp"

namespace dal::detail {

void homogen_table_impl::pull_rows(array<float>& block, const range& rows) const {
    // TODO: check range correctness
    // TODO: check array size if non-zero

    const std::int64_t N = get_row_count();
    const std::int64_t p = get_column_count();
    const std::int64_t block_size = rows.get_element_count(N)*p;
    const data_type block_dtype = make_data_type<float>();

    if (layout_ != data_layout::row_major) {
        throw std::runtime_error("unsupported data layout");
    }

    if (block_dtype == finfo_.dtype) {
        auto row_data = reinterpret_cast<float*>(data_.get());
        auto row_start_pointer = row_data + rows.start_idx * p;
        block.reset(row_start_pointer, block_size, empty_deleter<float>());
    } else {
        if (!block.is_data_owner() || block.get_capacity() < block_size) {
            block.reset(block_size);
        } else {
            block.resize(block_size);
        }

        backend::convert_vector(data_.get(), block.get_pointer(),
                                finfo_.dtype, block_dtype, block_size);
    }
}

} // namespace dal::detail

