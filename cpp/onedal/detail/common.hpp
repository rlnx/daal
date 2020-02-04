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

#include <memory>
#include "onedal/common.hpp"

namespace dal {
namespace detail {

template <typename T>
struct type_id {};

enum class type_rt {
    i32,
    f32,
    f64
};

template <typename DataType>
inline type_rt make_type_rt();

template <>
inline type_rt make_type_rt<std::int32_t>() {
    return type_rt::i32;
}

template <>
inline type_rt make_type_rt<float>() {
    return type_rt::f32;
}

template <>
inline type_rt make_type_rt<double>() {
    return type_rt::f64;
}

template <typename T>
using shared = std::shared_ptr<T>;

template <typename T>
using pimpl = shared<T>;

struct pimpl_accessor {
    template <typename Object>
    auto& get_pimpl(Object&& object) const {
        return object.impl_;
    }

    template <typename Object>
    auto make_from_pimpl(typename Object::pimpl const& impl) {
        return Object{impl};
    }

    template <typename Object>
    auto make_from_pointer(typename Object::pimpl::element_type* pointer) {
        using pimpl_t = typename Object::pimpl;
        return Object{pimpl_t(pointer)};
    }
};

template <typename Impl, typename Object>
Impl& get_impl(Object&& object) {
    return static_cast<Impl&>(pimpl_accessor().get_pimpl(object).get());
}

template <typename Object>
auto& get_impl_ptr(const Object& object) {
    return pimpl_accessor().get_pimpl(object);
}

template <typename Object, typename Pimpl>
Object make_from_pimpl(const Pimpl& impl) {
    return pimpl_accessor().template make_from_pimpl<Object>(impl);
}

template <typename Object, typename Pimpl>
Object make_from_pointer(const Pimpl& impl) {
    return pimpl_accessor().template make_from_pointer<Object>(impl);
}

}  // namespace detail
}  // namespace dal
