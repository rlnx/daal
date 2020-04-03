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

#include "onedal/common_helpers.hpp"
#include "onedal/detail/table_impl.hpp"

namespace dal::detail {

class homogen_table_impl : public table_impl {
public:
    homogen_table_impl(std::int64_t N, std::int64_t p, feature_info finfo, data_layout layout)
        : table_impl(N, p, table_metadata{ p, finfo, layout }) {
            std::int64_t size = N * p * sizeof(get_data_type_size(finfo.dtype));
            data_ = shared<const byte_t> {
                new byte_t[size],
                [](const byte_t* b) { delete[] b; }
            };
        }

    template <typename DataType>
    homogen_table_impl(std::int64_t N, std::int64_t p, const DataType* data_pointer, data_layout layout)
        : table_impl(N, p,
                     table_metadata{ p, feature_info{ make_data_type<DataType>() }, layout }) {
            data_ = shared<const byte_t> {
                reinterpret_cast<const byte_t*>(data_pointer),
                empty_deleter<const byte_t>()
            };
        }

    template <typename DataType>
    const DataType* get_data_pointer() const noexcept {
        return reinterpret_cast<const DataType*>(data_.get());
    }

private:
    shared<const byte_t> data_;
};

} // namespace dal::detail
