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

#pragma once

#include <string>
#include "oneapi/dal/test/common.hpp"
#include "oneapi/dal/table/common.hpp"
#include "oneapi/dal/table/row_accessor.hpp"
#include "oneapi/dal/table/detail/table_builder.hpp"

#define CHECK_TABLE_SHAPE(t, shape)                        \
    SECTION("ensure " #t "'s shape") {                     \
        REQUIRE(t.get_row_count() == _TS_GET_0(shape));    \
        REQUIRE(t.get_column_count() == _TS_GET_1(shape)); \
    }

#define CHECK_TABLE_NAN(Float, t)                                    \
    SECTION("ensure there is no NaN in " #t) {                       \
        const auto [i, j] = oneapi::dal::test::find_nan<Float>(t);   \
        if (i >= 0) {                                                \
            FAIL(fmt::format(#t " contains NaN at [{}, {}]", i, j)); \
        }                                                            \
    }

#define CHECK_TABLE_INF(Float, t)                                    \
    SECTION("ensure there is no Inf in " #t) {                       \
        const auto [i, j] = oneapi::dal::test::find_inf<Float>(t);   \
        if (i >= 0) {                                                \
            FAIL(fmt::format(#t " contains Inf at [{}, {}]", i, j)); \
        }                                                            \
    }

#define CHECK_TABLE(Float, t, shape) \
    CHECK_TABLE_SHAPE(t, shape)      \
    CHECK_TABLE_NAN(Float, t)        \
    CHECK_TABLE_INF(Float, t)

namespace oneapi::dal::test {

using index_2d = std::tuple<std::int64_t, std::int64_t>;

template <typename Data, typename Predicate>
inline index_2d find_if(const table& x, Predicate&& predicate) {
    const std::int64_t row_count = x.get_row_count();
    const std::int64_t column_count = x.get_column_count();

    const auto arr_x = row_accessor<const Data>{ x }.pull();
    for (std::int64_t i = 0; i < row_count; i++) {
        for (std::int64_t j = 0; j < column_count; j++) {
            if (predicate(arr_x[i * column_count + j])) {
                return { i, j };
            }
        }
    }
    return { -1, -1 };
}

template <typename Data>
index_2d find(const table& t, Data v);

template <typename Float>
index_2d find_inf(const table& t);

template <typename Float>
index_2d find_nan(const table& t);

} // namespace oneapi::dal::test
