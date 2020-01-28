/*******************************************************************************
* Copyright 2014-2019 Intel Corporation
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

#include "onedal/table.hpp"
#include "onedal/detail/object_wrapper.hpp"
#include "onedal/optimization/lbfgs/solver.hpp"

namespace dal {
namespace linear_model {
namespace logistic_regression {

namespace detail {
struct tag {};
class estimator_impl;
class model_impl;
} // namespace detail

namespace method {
struct solver {};
using by_default = solver;
} // namespace method

class estimator_base : public base {
  public:
    using tag_t = detail::tag;
    using float_t = float;
    using method_t = method::by_default;
    using solver_t = optimization::lbfgs::solver;

    estimator_base();

    double get_penalty_l1() const;

    void set_penalty_l1(double penalty_l1);

    double get_penalty_l2() const;

    void set_penalty_l2(double penalty_l2);

    bool get_intercept_flag() const;

    void set_intercept_flag(bool intercept_flag);

  protected:
    explicit estimator_base(const dal::detail::object_wrapper& solver);

    const dal::detail::object_wrapper &get_solver() const;

    void set_solver(const dal::detail::object_wrapper& solver);

  private:
    dal::detail::pimpl<detail::estimator_impl> impl_;
};

template <typename Float  = estimator_base::float_t,
          typename Method = estimator_base::method_t,
          typename Solver = estimator_base::solver_t>
class estimator : public estimator_base {
  public:
    using float_t  = Float;
    using method_t = Method;
    using solver_t = Solver;

    explicit estimator(const solver_t &solver)
        : estimator_base(dal::detail::wrap_object(solver)) {}

    auto& set_penalty_l1(double penalty_l1) {
        estimator_base::set_penalty_l1(penalty_l1);
        return *this;
    }

    auto& set_penalty_l2(double penalty_l2) {
        estimator_base::set_penalty_l2(penalty_l2);
        return *this;
    }

    auto& set_intercept_flag(bool intercept_flag) {
        estimator_base::set_intercept_flag(intercept_flag);
        return *this;
    }

    const solver_t& get_solver() const {
        return dal::detail::unwrap_object<solver_t>(estimator_base::get_solver());
    }

    estimator& set_solver(const solver_t& solver) {
        estimator_base::set_solver(dal::detail::wrap_object(solver));
        return *this;
    }
};

class model : public base {
public:
  table get_weights() const;

private:
  dal::detail::pimpl<detail::model_impl> impl_;
};

} // namespace logistic_regression
} // namespace linear_model
} // namespace dal
