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

#include "onedal/optimization/lbfgs/solver.hpp"
#include "onedal/optimization/lbfgs/detail/solver_impl.hpp"

namespace dal {
namespace optimization {
namespace lbfgs {

using detail::solver_impl;

solver_base::solver_base()
    : impl_(new solver_impl{}) {}

std::int64_t solver_base::get_m() const {
    return impl_->m;
}

void solver_base::set_m(std::int64_t m) {
    impl_->m = m;
}

std::int64_t solver_base::get_l() const {
    return impl_->l;
}

void solver_base::set_l(std::int64_t l) {
    impl_->l = l;
}

std::int64_t solver_base::get_max_iteration_count() const {
    return impl_->max_iteration_count;
}

void solver_base::set_max_iteration_count(std::int64_t max_iteration_count) {
    impl_->max_iteration_count = max_iteration_count;
}

std::int64_t solver_base::get_correction_pair_batch_size() const {
    return impl_->correction_pair_batch_size;
}

void solver_base::set_correction_pair_batch_size(std::int64_t correction_pair_batch_size) {
    impl_->correction_pair_batch_size = correction_pair_batch_size;
}

table solver_base::get_correction_pair_batch_indices() const {
    return impl_->correction_pair_batch_indices;
}

void solver_base::set_correction_pair_batch_indices(const table& correction_pair_batch_indices) {
    impl_->correction_pair_batch_indices = correction_pair_batch_indices;
}

table solver_base::get_batch_indices() const {
    return impl_->batch_indices;
}

void solver_base::set_batch_indices(const table& batch_indices) {
    impl_->batch_indices = batch_indices;
}

table solver_base::get_step_length_sequence() const {
    return impl_->step_length_sequence;
}

void solver_base::set_step_length_sequence(const table& step_length_sequence) {
    impl_->step_length_sequence = step_length_sequence;
}

double solver_base::get_accuracy_threshold() const {
    return impl_->accuracy_threshold;
}

void solver_base::set_accuracy_threshold(double accuracy_threshold) {
    impl_->accuracy_threshold = accuracy_threshold;
}

} // namespace lbfgs
} // namespace optimization
} // namespace dal
