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
#include "onedal/data_management/detail/slice_impl.hpp"

namespace dal {
namespace data_management {
namespace detail {

class slicable : public base {
public:
    virtual std::int32_t get_num_rows() const noexcept = 0;
    virtual std::int32_t get_num_cols() const noexcept = 0;

    virtual float* get_slice_data(slice_impl&, float*) const = 0;
    virtual double* get_slice_data(slice_impl&, double*) const = 0;
    virtual std::int32_t* get_slice_data(slice_impl&, std::int32_t*) const = 0;

    virtual void release_slice_data(slice_impl&, float*) = 0;
    virtual void release_slice_data(slice_impl&, double*) = 0;
    virtual void release_slice_data(slice_impl&, std::int32_t*) = 0;
};

} // namespace detail
} // namespace data_management
} // namespace dal
