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

#include <windows.h>
#include "oneapi/dal/backend/dispatcher.hpp"

extern HMODULE load_onedal_thread_dll();
extern HMODULE load_onedal_sequential_dll();

static HMODULE threading_layer_dll_handle = nullptr;

static void check_threading_layer() {
    if (!threading_layer_dll_handle) {
        throw oneapi::dal::internal_error{
            "Cannot load threading layer DLL. "
            "Make sure 'onedal_thread.dll' is present in PATH"};
    }
}

static void try_load_threading_layer() {
    // TODO: Currently we support only threaded DLL loading
    if (!threading_layer_dll_handle) {
        threading_layer_dll_handle = load_onedal_thread_dll();
        check_threading_layer();
    }
    return threading_layer_dll_handle;
}

static FARPROC get_function_pointer(const char* function_name) {
    check_threading_layer();

    FARPROC function_pointer = GetProcAddress(threading_layer_dll_handle, function_name);
    if (!function_pointer) {
        throw oneapi::dal::internal_error{
            "Cannot load function from threading layer DLL. Symbol " +
            std::string{function_name} + " is not found"};
    }

    return function_pointer;
}

#define DEFINE_DLL_FUNCTION_PROXY_ISA(ReturnType, prefix, isa, name, parameters, args) \
    using prefix##isa##_##name##_ptr_t = ReturnType (*) parameters;                    \
    static prefix##isa##_##name##_ptr_t prefix##isa##_##name##_ptr = nullptr;          \
    ReturnType prefix##isa##_##name parameters                                         \
    {                                                                                  \
        if (!prefix##isa##_##name##_ptr) {                                             \
            try_load_threading_layer();                                                \
            auto f_ptr = get_function_pointer(#prefix #isa "_" #name);                 \
            prefix##isa##_##name##_ptr = (prefix##isa##_##name##_ptr_t)f_ptr;          \
        }                                                                              \
        return prefix##isa##_##name##_ptr args;                                        \
    }

#define DEFINE_DLL_FUNCTION_PROXY(ReturnType, prefix, name, parameters, args)              \
    DEFINE_DLL_FUNCTION_PROXY_ISA(ReturnType, prefix, sse2, name, parameters, args)        \
    ONEDAL_IF_CPU_DISPATCH_SSSE3(                                                          \
        DEFINE_DLL_FUNCTION_PROXY_ISA(ReturnType, prefix, ssse3, name, parameters, args))  \
    ONEDAL_IF_CPU_DISPATCH_SSE42(                                                          \
        DEFINE_DLL_FUNCTION_PROXY_ISA(ReturnType, prefix, sse42, name, parameters, args))  \
    ONEDAL_IF_CPU_DISPATCH_AVX(                                                            \
        DEFINE_DLL_FUNCTION_PROXY_ISA(ReturnType, prefix, avx, name, parameters, args))    \
    ONEDAL_IF_CPU_DISPATCH_AVX2(                                                           \
        DEFINE_DLL_FUNCTION_PROXY_ISA(ReturnType, prefix, avx2, name, parameters, args))   \
    ONEDAL_IF_CPU_DISPATCH_AVX512(                                                         \
        DEFINE_DLL_FUNCTION_PROXY_ISA(ReturnType, prefix, avx512, name, parameters, args))

#define GEMM_PARAMETERS(Float)                                                             \
    (const char *transa, const char *transb, const std::int64_t *m, const std::int64_t *n, \
     const std::int64_t *k, const Float *alpha, const Float *a, const std::int64_t *lda,   \
     const Float *b, const std::int64_t *ldb, const Float *beta, const Float *c,           \
     const std::int64_t *ldc)

#define GEMM_ARGS(Float) (transa, transb, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc)

DEFINE_DLL_FUNCTION_PROXY(void, fpk_blas_, sgemm, GEMM_PARAMETERS(float), GEMM_ARGS(float))
DEFINE_DLL_FUNCTION_PROXY(void, fpk_blas_, dgemm, GEMM_PARAMETERS(double), GEMM_ARGS(double))
