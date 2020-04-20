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

#include "onedal/array.hpp"

#include <utility>

namespace dal::detail {

struct storage_readonly { };
struct storage_writeonly { };
struct storage_readwrite { };

template <typename StorageType>
class dense_storage_iface { };

template <>
class dense_storage_iface<storage_readonly> : public base {
public:
    virtual void pull_rows(array<float>&, const range&) const = 0;
    virtual void pull_rows(array<double>&, const range&) const = 0;
    virtual void pull_rows(array<std::int32_t>&, const range&) const = 0;
};

template <>
class dense_storage_iface<storage_writeonly> : public base {
public:
    virtual void push_back_rows(const array<float>&, const range&) = 0;
    virtual void push_back_rows(const array<double>&, const range&) = 0;
    virtual void push_back_rows(const array<std::int32_t>&, const range&) = 0;
};

template <>
class dense_storage_iface<storage_readwrite> : public dense_storage_iface<storage_readonly>,
                                               public dense_storage_iface<storage_writeonly> { };

template <typename AccessType>
struct get_dense_storage_iface {
    using type = dense_storage_iface<storage_readwrite>;
};

template <typename AccessType>
struct get_dense_storage_iface<const AccessType> {
    using type = dense_storage_iface<storage_readonly>;
};

template <typename AccessType>
using get_dense_storage_iface_t = typename get_dense_storage_iface<AccessType>::type;

} // namespace dal::detail
