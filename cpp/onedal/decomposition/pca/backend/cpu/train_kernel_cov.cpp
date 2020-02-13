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

namespace dal {
namespace decomposition {
namespace pca {
namespace backend {

using std::int64_t;
using dal::backend::context_cpu;

namespace daal_pca = daal::algorithms::pca;
namespace daal_cov = daal::algorithms::covariance;
namespace interop  = dal::backend::interop;

template <typename Float, daal::CpuType Cpu>
using daal_pca_cor_kernel_t = daal_pca::internal::PCACorrelationKernel<daal::batch, Float, Cpu>;

template <typename Float>
static void call_daal_kernel(const context_cpu& ctx,
                             const descriptor_base& desc,
                             const table& data,
                             const table& eigenvectors,
                             const table& eigenvalues,
                             const table& means,
                             const table& variances) {
    const auto daal_data         = interop::convert_to_daal_homogen_table<Float>(data);
    const auto daal_eigenvectors = interop::convert_to_daal_homogen_table<Float>(eigenvectors);
    const auto daal_eigenvalues  = interop::convert_to_daal_homogen_table<Float>(eigenvalues);
    const auto daal_means        = interop::convert_to_daal_homogen_table<Float>(means);
    const auto daal_variances    = interop::convert_to_daal_homogen_table<Float>(variances);

    daal_cov::Batch<Float, daal_cov::defaultDense> covariance_alg;
    covariance_alg.input.set(daal_cov::data, daal_data);

    constexpr bool is_correlation = false;
    constexpr uint64_t results_to_compute = int64_t(daal_pca::mean ||
                                                    daal_pca::variance ||
                                                    daal_pca::eigenvalue);

    interop::call_daal_kernel<Float, daal_pca_cor_kernel_t>(
        ctx,
        is_correlation,
        desc.get_is_deterministic(),
        *daal_data,
        &covariance_alg,
        results_to_compute,
        *daal_eigenvectors,
        *daal_eigenvalues,
        *daal_means,
        *daal_variances);
}

template <typename Float>
static train_result train(const context_cpu& ctx,
                          const descriptor_base& desc,
                          const train_input& input) {
    const auto data = input.get_data();

    const int64_t row_count = data.get_row_count();
    const int64_t column_count = data.get_column_count();
    const int64_t component_count = desc.get_component_count();

    const auto eigenvectors = allocate_table<Float>(column_count, component_count);
    const auto eigenvalues  = allocate_table<Float>(1, component_count);
    const auto means        = allocate_table<Float>(1, component_count);
    const auto variances    = allocate_table<Float>(1, component_count);

    call_daal_kernel<Float>(ctx, desc, data, eigenvectors, eigenvalues, means, variances);

    return train_result()
        .set_model(model().set_eigenvectors(eigenvectors))
        .set_eigenvalues(eigenvalues);
}

template <typename Float>
struct train_kernel_cpu<Float, method::cov> {
    train_result operator()(const context_cpu& ctx,
                            const descriptor_base& desc,
                            const train_input& input) const {
        return train<Float>(ctx, desc, input);
    }
};

template struct train_kernel_cpu<float, method::cov>;
template struct train_kernel_cpu<double, method::cov>;

} // namespace backend
} // namespace pca
} // namespace decomposition
} // namespace dal
