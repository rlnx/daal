/*******************************************************************************
* Copyright 2021 Intel Corporation
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

#include "oneapi/dal/backend/primitives/lapack/eigen.hpp"

#include "oneapi/dal/test/engine/common.hpp"
#include "oneapi/dal/test/engine/math.hpp"
#include "oneapi/dal/test/engine/io.hpp"

namespace oneapi::dal::backend::primitives::test {

namespace te = dal::test::engine;
namespace la = te::linalg;

template <typename Float>
class sym_eigval_test {
public:
    std::int64_t generate_dim() const {
        return GENERATE(3, 28, 125, 256);
    }

    /// Generates symmetric positive-definite matrix with diagonal dominance.
    /// $\frac{1}{2}(A + A^T) + nE$, where $A$ is uniformly distributed matrix, $dim(A) = n$.
    la::matrix<Float> generate_symmetric_positive() {
        const std::int64_t dim = this->generate_dim();
        const auto a = la::generate_uniform<Float>({ dim, dim }, -1.0, 1.0, seed_);
        const auto at = la::transpose(a);
        const auto c = la::multiply(Float(0.5), la::add(a, at));
        return la::add(c, la::matrix<Float>::diag(dim, dim));
    }

    auto call_sym_eigval(const la::matrix<Float>& symmetric_matrix) {
        constexpr bool is_ascending = true;
        return call_sym_eigval_generic(symmetric_matrix, is_ascending);
    }

    auto call_sym_eigval_descending(const la::matrix<Float>& symmetric_matrix) {
        constexpr bool is_ascending = false;
        return call_sym_eigval_generic(symmetric_matrix, is_ascending);
    }

    auto call_sym_eigval_generic(const la::matrix<Float>& symmetric_matrix, bool is_ascending) {
        ONEDAL_ASSERT(symmetric_matrix.get_row_count() == symmetric_matrix.get_column_count());

        const std::int64_t dim = symmetric_matrix.get_row_count();
        const auto s_copy_flat = symmetric_matrix.copy().get_array();

        auto data_or_eigenvectors_nd = ndarray<Float, 2>::wrap_mutable(s_copy_flat, { dim, dim });
        auto eigenvalues_nd = ndarray<Float, 1>::empty(dim);
        if (is_ascending) {
            sym_eigval(data_or_eigenvectors_nd, eigenvalues_nd);
        }
        else {
            sym_eigval_descending(data_or_eigenvectors_nd, eigenvalues_nd);
        }

        const auto eigenvectors = la::matrix<Float>::wrap_nd(data_or_eigenvectors_nd);
        const auto eigenvalues = la::matrix<Float>::wrap_nd(eigenvalues_nd);
        return std::make_tuple(eigenvectors, eigenvalues);
    }

    void check_eigvals_definition(const la::matrix<Float>& s,
                                  const la::matrix<Float>& eigvecs,
                                  const la::matrix<Float>& eigvals) const {
        INFO("convert results to float64");
        const auto s_f64 = la::astype<double>(s);
        const auto eigvals_f64 = la::astype<double>(eigvals);
        const auto eigvecs_f64 = la::astype<double>(eigvecs);

        INFO("check eigenvectors and eigenvalues definition");
        for (std::int64_t i = 0; i < eigvecs.get_row_count(); i++) {
            const auto v = la::transpose(eigvecs_f64.get_row(i));
            const double w = eigvals_f64.get(i);
            CAPTURE(i, w);

            // Input matrix is positive-definite, so all eigenvalues must be positive
            REQUIRE(w > 0);

            const double tol = te::get_tolerance<float>(1e-4, 1e-10) * w;

            // Check condition: $S \times v_i = w_i \dot v_i$
            const double err = la::rel_error(la::dot(s_f64, v), la::multiply(w, v), tol);
            REQUIRE(err < tol);
        }
    }

    void check_eigvals_are_ascending(const la::matrix<Float>& eigvals) const {
        INFO("check eigenvalues order is ascending");
        la::enumerate_linear(eigvals, [&](std::int64_t i, Float x) {
            if (i > 0) {
                REQUIRE(eigvals.get(i - 1) <= x);
            }
        });
    }

    void check_eigvals_are_descending(const la::matrix<Float>& eigvals) const {
        INFO("check eigenvalues order is descending");
        la::enumerate_linear(eigvals, [&](std::int64_t i, Float x) {
            if (i > 0) {
                REQUIRE(eigvals.get(i - 1) >= x);
            }
        });
    }

private:
    static constexpr int seed_ = 7777;
};

#define SYM_EIGVAL_TEST(name) TEMPLATE_TEST_M(sym_eigval_test, name, "[sym_eigval]", float, double)

SYM_EIGVAL_TEST("check sym_eigval on symmetric positive-definite matrix") {
    const auto s = this->generate_symmetric_positive();

    const auto [eigenvectors, eigenvalues] = this->call_sym_eigval(s);

    this->check_eigvals_definition(s, eigenvectors, eigenvalues);
    this->check_eigvals_are_ascending(eigenvalues);
}

SYM_EIGVAL_TEST("check sym_eigval_descending on symmetric positive-definite matrix") {
    const auto s = this->generate_symmetric_positive();

    const auto [eigenvectors, eigenvalues] = this->call_sym_eigval_descending(s);

    this->check_eigvals_definition(s, eigenvectors, eigenvalues);
    this->check_eigvals_are_descending(eigenvalues);
}

} // namespace oneapi::dal::backend::primitives::test
