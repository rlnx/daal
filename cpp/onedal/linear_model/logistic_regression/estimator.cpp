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

#include "onedal/linear_model/logistic_regression/estimator.hpp"
#include "onedal/linear_model/logistic_regression/detail/estimator_impl.hpp"

namespace dal {
namespace linear_model {
namespace logistic_regression {

using detail::estimator_impl;

estimator_base::estimator_base()
    : impl_(new estimator_impl{dal::detail::wrap_object(estimator_base::solver_t{})}) {}

estimator_base::estimator_base(const dal::detail::object_wrapper& solver)
    : impl_(new estimator_impl{solver}) {}

const dal::detail::object_wrapper& estimator_base::get_solver() const {
    return impl_->solver;
}

void estimator_base::set_solver(const dal::detail::object_wrapper& solver) {
    impl_->solver = solver;
}

} // namespace logistic_regression
} // namespace linear_model
} // namespace dal
