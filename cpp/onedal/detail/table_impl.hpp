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
#include "onedal/detail/data_storage.hpp"
#include "onedal/table_metadata.hpp"

namespace dal::detail {

class table_impl : public base {
public:
    table_impl(std::int64_t N, std::int64_t p,
               const table_metadata& meta)
        : N_(N),
          p_(p),
          meta_(meta) { }

    std::int64_t get_feature_count() const noexcept {
        return p_;
    }

    std::int64_t get_observation_count() const noexcept {
        return N_;
    }

    const table_metadata& get_metadata() const noexcept {
        return meta_;
    }

private:
    std::int64_t N_;
    std::int64_t p_;

    table_metadata meta_;
};

template <typename TableImpl>
class table_impl_wrapper : public table_impl {
public:
    table_impl_wrapper(TableImpl&& obj)
        : table_impl(obj.get_feature_count(),
                     obj.get_observation_count(),
                     obj.get_metadata()) { }
};

} // namespace dal::detail
