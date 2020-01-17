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
#include "onedal/decomposition/pca/backend/cpu/train_kernel.hpp"

namespace dal {
namespace decomposition {
namespace pca {
namespace backend {

namespace daal_pca = daal::algorithms::pca;

template <typename Float>
static train_result train_cov(const default_execution_context& ctx,
                              const params_base& params,
                              const train_input& input) {
  daal_pca::Batch<Float, daal_pca::correlationDense> alg;
  alg.parameter.nComponents = params.get_components_count();
  alg.parameter.isDeterministic = params.get_is_deterministic();

  // TODO: Wrap input.get_data() into NumericTable and pass to:
  //       alg.input.set(daal_pca::data, );

  return train_result();
};

template <typename Float>
struct train_kernel<Float, method::cov> {
  train_result operator()(const default_execution_context& ctx,
                          const params_base& params,
                          const train_input& input) const {
    return train_cov<Float>(ctx, params, input);
  }
};

template struct train_kernel<float, method::cov>;
template struct train_kernel<double, method::cov>;

} // namespace backend
} // namespace pca
} // namespace decomposition
} // namespace dal
