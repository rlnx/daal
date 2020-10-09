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

#include "oneapi/dal/test/tables.hpp"

#include <cmath>
#include <limits>

namespace oneapi::dal::test {

template <typename Data>
index_2d find(const table& t, Data v) {
    return find_if<Data>(t, [&](Data x) {
        return x == v;
    });
}

template <typename Float>
index_2d find_nan(const table& t) {
    return find_if<Float>(t, [&](Float x) {
        return x != x;
    });
}

template <typename Float>
index_2d find_inf(const table& t) {
    constexpr auto inf = std::numeric_limits<Float>::infinity();
    return find_if<Float>(t, [&](Float x) {
        return std::fabs(x) == inf;
    });
}

#define INSTANTIATE(Data) template index_2d find<Data>(const table& x, Data v);

#define INSTANTIATE_FLOAT(Float)                       \
    template index_2d find_nan<Float>(const table& x); \
    template index_2d find_inf<Float>(const table& x);

INSTANTIATE(std::int32_t)
INSTANTIATE(float)
INSTANTIATE(double)

INSTANTIATE_FLOAT(float)
INSTANTIATE_FLOAT(double)

} // namespace oneapi::dal::test
