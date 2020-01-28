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

#include "daal/include/algorithms/pca/pca_batch.h"
#include "daal/algorithms/kernel/pca/pca_dense_correlation_batch_kernel.h"

#include "onedal/detail/table_homogen_impl.hpp"
#include "onedal/detail/interop/common.hpp"

#include "onedal/decomposition/pca/backend/cpu/train_kernel.hpp"
#include "onedal/decomposition/pca/detail/train_types_impl.hpp"
#include "onedal/decomposition/pca/detail/common_impl.hpp"

using namespace dal::detail;
using namespace dal::decomposition::pca::detail;

namespace dal {
namespace decomposition {
namespace pca {
namespace backend {

namespace daal_pca = daal::algorithms::pca;
namespace daal_cov = daal::algorithms::covariance;
namespace daal_dm  = daal::data_management;

template <typename Cpu, typename Float>
struct train_kernel<Cpu, Float, method::cov> {
    train_result operator()(const default_execution_context& ctx,
                            const params_base& params,
                            const train_input& input) const {
        const auto data = input.get_data();

        const std::int64_t col_count = data.get_column_count();
        const std::int64_t row_count = data.get_row_count();
        const std::int64_t components_count = params.get_components_count();

        const auto data_table = interop::to_daal_homogen_table<Float>(data);

        constexpr bool is_correlation = false;
        constexpr auto cpu_type = interop::get_daal_cpu_type<Cpu>();
        std::uint64_t results_to_compute = std::uint64_t(daal_pca::none);
        // results_to_compute = daal_pca::mean || daal_pca::variance || daal_pca::eigenvalue;

        const auto eigenvectors = interop::make_daal_homogen_table<Float>(col_count, components_count);
        const auto eigenvalues  = interop::make_daal_homogen_table<Float>(1, components_count);
        const auto means        = interop::make_daal_homogen_table<Float>(1, components_count);
        const auto variances    = interop::make_daal_homogen_table<Float>(1, components_count);

        daal_cov::Batch<Float> covariance_alg;
        covariance_alg.input.set(daal_cov::data, data_table);

        using kernel_t = daal_pca::internal::PCACorrelationKernel<daal::batch, Float, cpu_type>;
        interop::call_kernel<kernel_t>(is_correlation, params.get_is_deterministic(), *data_table,
                                       &covariance_alg, results_to_compute,
                                       *eigenvectors, *eigenvalues, *means, *variances);

        shared<model_impl> model { new model_impl() };
        model->eigenvectors = interop::from_daal_table<Float>(*eigenvectors);

        shared<train_result_impl> result { new train_result_impl() };
        result->trained_model = model;

        return result;
    }
};

template struct train_kernel<DAL_CPU_ID_, float, method::cov>;
template struct train_kernel<DAL_CPU_ID_, double, method::cov>;

} // namespace backend
} // namespace pca
} // namespace decomposition
} // namespace dal
