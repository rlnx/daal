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
#include "onedal/data_management/detail/slicable.hpp"

namespace dal {
namespace data_management {
namespace detail {

class table_impl : public slicable {
public:
    table_impl(std::int32_t rows, std::int32_t cols)
        : _rows(rows)
        , _cols(cols)
    { }

    virtual std::int32_t get_num_rows() const noexcept override {
        return _rows;
    }

    virtual std::int32_t get_num_cols() const noexcept override {
        return _cols;
    }

private:
    std::int32_t _rows;
    std::int32_t _cols;
};

} // namespace detail
} // namespace data_management
} // namespace dal
