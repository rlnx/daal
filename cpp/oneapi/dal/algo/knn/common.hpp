/*******************************************************************************
* Copyright 2020-2021 Intel Corporation
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

#include "oneapi/dal/detail/common.hpp"
#include "oneapi/dal/table/common.hpp"

namespace oneapi::dal::knn {

namespace task {
namespace v1 {
/// Tag-type that parameterizes entities used for solving
/// :capterm:`classification problem <classification>`.
struct classification {};

/// Alias tag-type for classification task.
using by_default = classification;
} // namespace v1

using v1::classification;
using v1::by_default;

} // namespace task

namespace method {
namespace v1 {
/// Tag-type that denotes :ref:`k-d tree <knn_t_math_kd_tree>` computational method.
struct kd_tree {};

/// Tag-type that denotes :ref:`brute-force <knn_t_math_brute_force>` computational
/// method.
struct brute_force {};

/// Alias tag-type for :ref:`brute-force <knn_t_math_brute_force>` computational
/// method.
using by_default = brute_force;
} // namespace v1

using v1::kd_tree;
using v1::brute_force;
using v1::by_default;

} // namespace method

namespace detail {
namespace v1 {
struct descriptor_tag {};
template <typename Task>
class descriptor_impl;

template <typename Task>
class model_impl;

template <typename Float>
constexpr bool is_valid_float_v = dal::detail::is_one_of_v<Float, float, double>;

template <typename Method>
constexpr bool is_valid_method_v =
    dal::detail::is_one_of_v<Method, method::kd_tree, method::brute_force>;

template <typename Task>
constexpr bool is_valid_task_v = dal::detail::is_one_of_v<Task, task::classification>;

template <typename Task = task::by_default>
class descriptor_base : public base {
    static_assert(is_valid_task_v<Task>);

public:
    using tag_t = descriptor_tag;
    using float_t = float;
    using method_t = method::by_default;
    using task_t = Task;

    descriptor_base();

    std::int64_t get_class_count() const;
    std::int64_t get_neighbor_count() const;

protected:
    void set_class_count_impl(std::int64_t value);
    void set_neighbor_count_impl(std::int64_t value);

private:
    dal::detail::pimpl<descriptor_impl<Task>> impl_;
};

} // namespace v1

using v1::descriptor_tag;
using v1::descriptor_impl;
using v1::model_impl;
using v1::descriptor_base;

using v1::is_valid_float_v;
using v1::is_valid_method_v;
using v1::is_valid_task_v;

} // namespace detail

namespace v1 {

/// @tparam Float  The floating-point type that the algorithm uses for
///                intermediate computations. Can be :expr:`float` or
///                :expr:`double`.
/// @tparam Method Tag-type that specifies an implementation of algorithm. Can
///                be :expr:`method::brute_force` or :expr:`method::kd_tree`.
/// @tparam Task   Tag-type that specifies type of the problem to solve. Can
///                be :expr:`task::classification`.
template <typename Float = float,
          typename Method = method::by_default,
          typename Task = task::by_default>
class descriptor : public detail::descriptor_base<Task> {
    static_assert(detail::is_valid_float_v<Float>);
    static_assert(detail::is_valid_method_v<Method>);
    static_assert(detail::is_valid_task_v<Task>);

    using base_t = detail::descriptor_base<Task>;

public:
    using float_t = Float;
    using method_t = Method;
    using task_t = Task;

    /// Creates a new instance of the class with the given :literal:`class_count`
    /// and :literal:`neighbor_count` property values
    explicit descriptor(std::int64_t class_count, std::int64_t neighbor_count) {
        set_class_count(class_count);
        set_neighbor_count(neighbor_count);
    }

    /// The number of classes c
    /// @invariant :expr:`class_count > 1`
    std::int64_t get_class_count() const {
        return base_t::get_class_count();
    }

    auto& set_class_count(std::int64_t value) {
        base_t::set_class_count_impl(value);
        return *this;
    }

    /// The number of neighbors k
    /// @invariant :expr:`neighbor_count > 0`
    std::int64_t get_neighbor_count() const {
        return base_t::get_neighbor_count();
    }

    auto& set_neighbor_count(std::int64_t value) {
        base_t::set_neighbor_count_impl(value);
        return *this;
    }
};

/// @tparam Task Tag-type that specifies type of the problem to solve. Can
///              be :expr:`task::classification`.
template <typename Task = task::by_default>
class model : public base {
    static_assert(detail::is_valid_task_v<Task>);
    friend dal::detail::pimpl_accessor;

public:
    /// Creates a new instance of the class with the default property values.
    model();

private:
    explicit model(const std::shared_ptr<detail::model_impl<Task>>& impl);
    dal::detail::pimpl<detail::model_impl<Task>> impl_;
};

} // namespace v1

using v1::descriptor;
using v1::model;

} // namespace oneapi::dal::knn
