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
#include "onedal/detail/common.hpp"

namespace dal {
namespace optimization {
namespace lbfgs {

namespace detail {
class solver_impl;
} // namespace detail

class solver_base : public base {
  public:
    solver_base();

    std::int64_t get_m() const;

    void set_m(std::int64_t);

    std::int64_t get_l() const;

    void set_l(std::int64_t);

    std::int64_t get_max_iteration_count() const;

    void set_max_iteration_count(std::int64_t);

    std::int64_t get_correction_pair_batch_size() const;

    void set_correction_pair_batch_size(std::int64_t);

    table get_correction_pair_batch_indices() const;

    void set_correction_pair_batch_indices(const table&);

    table get_batch_indices() const;

    void set_batch_indices(const table&);

    table get_step_length_sequence() const;

    void set_step_length_sequence(const table&);

    double get_accuracy_threshold() const;

    void set_accuracy_threshold(double);

  private:
    dal::detail::pimpl<detail::solver_impl> impl_;
};

class solver : public solver_base {
  public:
    auto& set_m(std::int64_t m) {
        solver_base::set_m(m);
        return *this;
    }

    auto& set_l(std::int64_t l) {
        solver_base::set_l(l);
        return *this;
    }

    auto& set_max_iteration_count(std::int64_t max_iteration_count) {
        solver_base::set_max_iteration_count(max_iteration_count);
        return *this;
    }

    auto& set_correction_pair_batch_size(std::int64_t correction_pair_batch_size) {
        solver_base::set_correction_pair_batch_size(correction_pair_batch_size);
        return *this;
    }

    auto& set_correction_pair_batch_indices(const table& correction_pair_batch_indices) {
        solver_base::set_correction_pair_batch_indices(correction_pair_batch_indices);
        return *this;
    }

    auto& set_batch_indices(const table& batch_indices) {
        solver_base::set_batch_indices(batch_indices);
        return *this;
    }

    auto& set_step_length_sequence(const table& step_length_sequence) {
        solver_base::set_step_length_sequence(step_length_sequence);
        return *this;
    }

    auto& set_accuracy_threshold(double accuracy_threshold) {
        solver_base::set_accuracy_threshold(accuracy_threshold);
        return *this;
    }
};

} // namespace lbfgs
} // namespace optimization
} // namespace dal
