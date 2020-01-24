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
#include "onedal/detail/common.hpp"

namespace dal {
namespace data_management {
namespace detail {

class slice {
public:
    range rows;
    range cols;
};

class table_data : public base {
public:
    table_data(std::int64_t rows, std::int64_t cols)
        : _rows(rows)
        , _cols(cols)
    { }

    virtual std::int64_t get_num_rows() const noexcept {
        return _rows;
    }

    virtual std::int64_t get_num_cols() const noexcept {
        return _cols;
    }

    virtual float* get_data_ptr(const slice&, float*) const = 0;
    virtual double* get_data_ptr(const slice&, double*) const = 0;
    virtual std::int32_t* get_data_ptr(const slice&, std::int32_t*) const = 0;

    virtual void release_data_ptr(const slice&, float*, bool need_copy_ptr) = 0;
    virtual void release_data_ptr(const slice&, double*, bool need_copy_ptr) = 0;
    virtual void release_data_ptr(const slice&, std::int32_t*, bool need_copy_ptr) = 0;

private:
    std::int64_t _rows;
    std::int64_t _cols;
};

using table_data_ptr = dal::detail::shared<table_data>;

} // namespace detail
} // namespace data_management
} // namespace dal
