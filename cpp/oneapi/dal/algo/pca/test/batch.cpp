/*******************************************************************************
* Copyright 2020 Intel Corporation
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

#include "oneapi/dal/algo/pca/train.hpp"
#include "oneapi/dal/test/tables.hpp"
#include "oneapi/dal/test/matchers.hpp"
#include "oneapi/dal/test/dataframes.hpp"
#include "oneapi/dal/backend/linalg.hpp"
#include "oneapi/dal/table/row_accessor.hpp"

namespace oneapi::dal::pca::test {

namespace ts = dal::test;
namespace la = dal::backend::linalg;

ALGO_TEST_CASE("pca general flow", (float, double), (method::cov)) {
    DECLARE_TEST_POLICY(policy);

    const ts::dataframe data =
        GENERATE_DATAFRAME(ts::dataframe_builder(1000, 100).fill_uniform(-2, 5),
                           ts::dataframe_builder(100, 10).fill_uniform(-0.2, 1.5));
    const std::string table_type = GENERATE("homogen");
    const std::int64_t component_count = GENERATE(2, 3, 5);

    const table x = data.get_table<Float>(policy, table_type);

    const auto pca_desc = descriptor<Float, Method>{} //
                              .set_component_count(component_count) //
                              .set_deterministic(false);

    train_result result;
    SECTION("training") {
        result = ts::train(policy, pca_desc, x);
        const auto mdl = result.get_model();
        const auto means = result.get_means();
        const auto variances = result.get_variances();
        const auto eigenvalues = result.get_eigenvalues();
        const auto eigenvectors = result.get_eigenvectors();

        CHECK_TABLE(Float, means, (1, x.get_column_count()));
        CHECK_TABLE(Float, variances, (1, x.get_column_count()));
        CHECK_TABLE(Float, eigenvalues, (1, component_count));
        CHECK_TABLE(Float, eigenvectors, (component_count, x.get_column_count()));

        SECTION("ensure model and result eigenvectors are the same") {
            REQUIRE(mdl.get_eigenvectors() == ts::all_exact<Float>{ eigenvectors });
        }

        SECTION("ensure means are correct") {
            const auto expected = data.column_means();
            REQUIRE(means == ts::all_close<Float>{ expected, ts::eps<Float>(1e-4, 1e-10) });
        }

        SECTION("ensure variances are correct") {
            const auto expected = data.column_variances();
            REQUIRE(variances == ts::all_close<Float>{ expected, ts::eps<Float>(1e-4, 1e-10) });
        }

        SECTION("ensure eigenvalues order is descending") {
            const auto W = la::matrix<double>::wrap(eigenvalues);
            W.enumerate_linear([&](std::int64_t j, double w) {
                if (j > 0) {
                    CHECK(W.get(0, j - 1) > W.get(0, j));
                }
            });
        }

        SECTION("ensure eigenvectors matrix is orthogonal") {
            const auto V = la::matrix<double>::wrap(eigenvectors);
            const auto E = la::matrix<double>::eye(V.get_row_count());
            const auto VxVT = la::dot(V, V.T());
            CHECK(VxVT == ts::all_close<double>{ E, ts::eps<Float>(1e-4, 1e-10) });
        }
    }

    SECTION("inference") {
        // TODO
    }
}

} // namespace oneapi::dal::pca::test
