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

#include "onedal/detail/table_homogen_impl.hpp"

#include "onedal/decomposition/pca/backend/cpu/train_kernel.hpp"
#include "onedal/decomposition/pca/detail/train_types_impl.hpp"
#include "onedal/decomposition/pca/detail/common_impl.hpp"

namespace dal {
namespace decomposition {
namespace pca {
namespace backend {

namespace daal_pca = daal::algorithms::pca;
namespace daal_dm  = daal::data_management;

template <typename Cpu, typename Float>
struct train_kernel<Cpu, Float, method::cov> {
  train_result operator()(const default_execution_context& ctx,
                          const params_base& params,
                          const train_input& input) const {
    daal_pca::Batch<Float, daal_pca::correlationDense> alg;
    alg.parameter.nComponents = params.get_components_count();
    alg.parameter.isDeterministic = params.get_is_deterministic();
    // TODO: algorithm.parameter.resultsToCompute = pca::mean | pca::variance | pca::eigenvalue;

    auto data = input.get_data();
    auto array = dal::flatten<Float, dal::access_mode::read>(data);
    auto inputNt = daal_dm::HomogenNumericTable<Float>::create(array.get_pointer(),
                                                               data.get_column_count(),
                                                               data.get_row_count());
    alg.input.set(daal_pca::data, inputNt);
    alg.compute();

    model::pimpl model_impl { new detail::model_impl() };
    train_result::pimpl result_impl { new detail::train_result_impl() };

    daal_pca::ResultPtr result = alg.getResult();
    {
        auto eigenvectors = result->get(daal_pca::eigenvectors);

        daal_dm::BlockDescriptor<Float> desc;
        eigenvectors->getBlockOfRows(0, eigenvectors->getNumberOfRows(), daal_dm::readOnly, desc);

        table::pimpl eigenvectors_table {
                dal::detail::table_impl_ptr {
                    new dal::detail::table_homogen_impl(desc.getBlockPtr(),
                                                        eigenvectors->getNumberOfColumns(),
                                                        eigenvectors->getNumberOfRows())
                }
        };

        model_impl->eigenvectors = eigenvectors_table;
    }

    result_impl->trained_model = model_impl;
    return train_result(result_impl);
  }
};

template struct train_kernel<DAL_CPU_ID_, float, method::cov>;
template struct train_kernel<DAL_CPU_ID_, double, method::cov>;

} // namespace backend
} // namespace pca
} // namespace decomposition
} // namespace dal
