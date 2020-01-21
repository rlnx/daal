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

#include "onedal/detail/common.hpp"
#include "onedal/data_management/detail/slicable.hpp"

namespace dal {
namespace data_management {
namespace detail {

class slice_impl {
public:
    using pimpl_data_owner = dal::detail::pimpl<detail::slicable>;

public:
    pimpl_data_owner data_owner;
    range rows;
    range cols;
};

} // namespace detail
} // namespace data_management
} // namespace dal
