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

#include "onedal/decomposition/pca/backend/cpu/train_kernel.hpp"

#include "onedal/backend/interop/common.hpp"
#include "onedal/backend/interop/table_conversion.hpp"
#include "onedal/table_homogen_impl.hpp"

#include "daal/src/algorithms/pca/pca_dense_correlation_batch_kernel.h"

using namespace dal::detail;
using namespace dal::decomposition::pca::detail;

namespace dal {
namespace decomposition {
namespace pca {
namespace backend {

namespace daal_pca = daal::algorithms::pca;
namespace daal_cov = daal::algorithms::covariance;
namespace daal_dm  = daal::data_management;
namespace interop = dal::backend::interop;

template <typename Float>
struct train_kernel_cpu<Float, method::cov> {
    train_result operator()(const dal::backend::context_cpu& ctx,
                            const descriptor_base& params,
                            const train_input& input) const {
        const table data = input.get_data();

        const std::int64_t row_count = data.get_row_count();
        const std::int64_t column_count = data.get_column_count();
        const std::int64_t component_count = params.get_component_count();
        const std::uint64_t results_to_compute = std::uint64_t(daal_pca::none);
        // TODO: results_to_compute = daal_pca::mean || daal_pca::variance || daal_pca::eigenvalue;

        const auto daal_data = interop::convert_to_daal_homogen_table<Float>(data);

        const auto eigenvectors = interop::allocate_daal_homogen_table<Float>(column_count, component_count);
        const auto eigenvalues  = interop::allocate_daal_homogen_table<Float>(1, component_count);
        const auto means        = interop::allocate_daal_homogen_table<Float>(1, component_count);
        const auto variances    = interop::allocate_daal_homogen_table<Float>(1, component_count);

        daal_cov::Batch<Float> covariance_alg;
        covariance_alg.input.set(daal_cov::data, daal_data);

        constexpr bool is_correlation = false;
        dal::backend::dispatch_by_cpu(ctx, [&](auto cpu) {
            constexpr auto cpu_type = interop::get_daal_cpu_type(cpu);
            using kernel_t = daal_pca::internal::PCACorrelationKernel<daal::batch, Float, cpu_type>;
            interop::call_kernel<kernel_t>(
                is_correlation,
                params.get_is_deterministic(),
                *daal_data,
                &covariance_alg,
                results_to_compute,
                *eigenvectors,
                *eigenvalues,
                *means,
                *variances);
        });

        const auto trained_model = model()
            .set_eigenvectors(interop::convert_to_onedal_homogen_table<Float>(eigenvectors));

        const auto result = train_result()
            .set_model(trained_model)
            .set_eigenvalues(interop::convert_to_onedal_homogen_table<Float>(eigenvalues));

        return result;
    }
};

template struct train_kernel_cpu<float, method::cov>;
template struct train_kernel_cpu<double, method::cov>;

} // namespace backend
} // namespace pca
} // namespace decomposition
} // namespace dal
