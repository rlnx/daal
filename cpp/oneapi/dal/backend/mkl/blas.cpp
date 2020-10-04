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

#include "oneapi/dal/backend/mkl/blas.hpp"
#include "oneapi/dal/backend/mkl/import.hpp"

namespace oneapi::dal::backend::mkl {

#define GEMM_PARAMETERS_FORTRAN(Float)                                                    \
    const char *transa, const char *transb, const std::int64_t *m, const std::int64_t *n, \
    const std::int64_t *k, const Float *alpha, const Float *a, const std::int64_t *lda,   \
    const Float *b, const std::int64_t *ldb, const Float *beta, const Float *c,           \
    const std::int64_t *ldc

#define GEMM_PARAMETERS_C(Float)                                                     \
    bool transa, bool transb, std::int64_t m, std::int64_t n, std::int64_t k,        \
    Float alpha, const Float *a, std::int64_t lda, const Float *b, std::int64_t ldb, \
    Float beta, Float *c, std::int64_t ldc

#define GEMM_ARGS(Float) transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc

template <typename Float, typename Cpu = void>
struct gemm_op {};

template <typename Cpu>
struct gemm_op<float, Cpu> {
    void operator()(GEMM_PARAMETERS_FORTRAN(float)) const {
        fpk_blas_sgemm<Cpu>(GEMM_ARGS(float));
    }
};

template <typename Cpu>
struct gemm_op<double, Cpu> {
    void operator()(GEMM_PARAMETERS_FORTRAN(double)) const {
        fpk_blas_dgemm<Cpu>(GEMM_ARGS(double));
    }
};

template <typename Float>
struct gemm_op<Float, void> {
    void operator()(const context_cpu &ctx, GEMM_PARAMETERS_FORTRAN(Float)) const {
        dispatch_by_cpu(ctx, [&](auto cpu) {
            gemm_op<Float, decltype(cpu)>{}(GEMM_ARGS(Float));
        });
    }

    void operator()(GEMM_PARAMETERS_FORTRAN(Float)) const {
        (*this)(context_cpu{}, GEMM_ARGS(Float));
    }
};

template <typename Float, typename Cpu>
void gemm(GEMM_PARAMETERS_C(Float)) {
    using op_t = gemm_op<Float, Cpu>;
    const char ta = transa ? 'T' : 'N';
    const char tb = transb ? 'T' : 'N';
    op_t{}(&ta, &tb, &m, &n, &k, &alpha, a, &lda, b, &ldb, &beta, c, &ldc);
}

template <typename Float>
void gemm(const context_cpu& ctx, GEMM_PARAMETERS_C(Float)) {
    using op_t = gemm_op<Float>;
    const char ta = transa ? 'T' : 'N';
    const char tb = transb ? 'T' : 'N';
    op_t{}(ctx, &ta, &tb, &m, &n, &k, &alpha, a, &lda, b, &ldb, &beta, c, &ldc);
}

#define INSTANTIATE_GEMM_CPU(Float, Cpu)                        \
    template<> void gemm<Float, Cpu>(GEMM_PARAMETERS_C(Float)); \

#define INSTANTIATE_GEMM_CTX(Float)                                            \
    template<> void gemm<Float>(const context_cpu&, GEMM_PARAMETERS_C(Float)); \

#define INSTANTIATE_GEMM(Float)                                                     \
    INSTANTIATE_GEMM_CTX(Float)                                                     \
    INSTANTIATE_GEMM_CPU(Float, void)                                               \
    INSTANTIATE_GEMM_CPU(Float, cpu_dispatch_default)                               \
    ONEDAL_IF_CPU_DISPATCH_SSSE3(INSTANTIATE_GEMM_CPU(Float, cpu_dispatch_ssse3))   \
    ONEDAL_IF_CPU_DISPATCH_SSE42(INSTANTIATE_GEMM_CPU(Float, cpu_dispatch_sse42))   \
    ONEDAL_IF_CPU_DISPATCH_AVX(INSTANTIATE_GEMM_CPU(Float, cpu_dispatch_avx))       \
    ONEDAL_IF_CPU_DISPATCH_AVX2(INSTANTIATE_GEMM_CPU(Float, cpu_dispatch_avx2))     \
    ONEDAL_IF_CPU_DISPATCH_AVX512(INSTANTIATE_GEMM_CPU(Float, cpu_dispatch_avx512))

INSTANTIATE_GEMM(float)
INSTANTIATE_GEMM(double)

} // namespace oneapi::dal::backend::mkl
