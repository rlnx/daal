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
    template <typename DataType>
    homogen_table_impl(std::int64_t N, std::int64_t p, const DataType* data_pointer, data_layout layout)
        : table_impl(N, p,
                     table_metadata{ p, feature_info{ make_data_type<DataType>() }, layout }),
          finfo_(feature_info{ make_data_type<DataType>() }),
          layout_(layout) {
        data_ = shared<byte_t> {
            const_cast<byte_t*>(reinterpret_cast<const byte_t*>(data_pointer)),
            empty_deleter<byte_t>()
        };
    }

    template <typename DataType>
    const DataType* get_data_pointer() const {
        return reinterpret_cast<const DataType*>(data_.get());
    }

    virtual void pull_rows(array<float>& block, const range& rows) const override;
    virtual void pull_rows(array<double>& block, const range& r) const override { }
    virtual void pull_rows(array<std::int32_t>& block, const range& r) const override { }

private:
    shared<byte_t> data_;
    feature_info finfo_;
    data_layout layout_;
};

} // namespace dal::detail
