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

#include "oneapi/dal/test/common.hpp"
#include "oneapi/dal/backend/ndarray/array.hpp"
#include "oneapi/dal/backend/ndarray/test/fixtures.hpp"

namespace oneapi::dal::backend::ndarray::test {

namespace nd = oneapi::dal::backend::ndarray;

TEST_CASE("can create 1d shape", "[ndarray][shape]") {
    const std::int64_t dim0 = 10;
    nd::shape<1> shape{dim0};
    REQUIRE(shape[0] == dim0);

    SECTION("ensure product") {
        CHECK(shape.product() == dim0);
    }
}

TEST_CASE("can create 2d shape", "[ndarray][shape]") {
    const std::int64_t dim0 = 10;
    const std::int64_t dim1 = 20;
    nd::shape<2> shape{dim0, dim1};
    REQUIRE(shape[0] == dim0);
    REQUIRE(shape[1] == dim1);

    SECTION("ensure product") {
        CHECK(shape.product() == dim0 * dim1);
    }
}

TEST_CASE("can compare 1d shapes", "[ndarray][shape]") {
    nd::shape<1> x{10};
    nd::shape<1> y{10};
    nd::shape<1> z{15};

    SECTION("ensure equal") {
        CHECK(x == y);
        CHECK(!(x != y));
    }

    SECTION("ensure not equal") {
        CHECK(x != z);
        CHECK(!(x == z));
    }
}

TEST_CASE("can compare 2d shapes", "[ndarray][shape]") {
    nd::shape<2> x{10, 20};
    nd::shape<2> y{10, 20};
    nd::shape<2> z{15, 30};

    SECTION("ensure equal") {
        CHECK(x == y);
        CHECK(!(x != y));
    }

    SECTION("ensure not equal") {
        CHECK(x != z);
        CHECK(!(x == z));
    }
}

TEST_CASE("can compare 2d shapes if some axis is equal", "[ndarray][shape]") {
    nd::shape<2> x{10, 20};
    nd::shape<2> y{10, 21};
    nd::shape<2> z{11, 20};

    CHECK(x != y);
    CHECK(x != z);
}

TEST_CASE("can create empty 1d array", "[ndarray][shape]") {
    SECTION("default order") {
        nd::array<float, 1> arr;
        CHECK(arr.get_count() == 0);
        CHECK(arr.get_shape() == nd::shape<1>{0});
    }

    SECTION("c order") {
        nd::array<float, 1, nd::order::c> arr;
        CHECK(arr.get_count() == 0);
        CHECK(arr.get_shape() == nd::shape<1>{0});
    }

    SECTION("f order") {
        nd::array<float, 1, nd::order::f> arr;
        CHECK(arr.get_count() == 0);
        CHECK(arr.get_shape() == nd::shape<1>{0});
    }
}

TEST_CASE("can create empty 2d array", "[ndarray][shape]") {
    SECTION("default order") {
        nd::array<float, 2> arr;
        CHECK(arr.get_count() == 0);
        CHECK(arr.get_row_count() == 0);
        CHECK(arr.get_column_count() == 0);
        CHECK(arr.get_shape() == nd::shape<2>{0, 0});
    }

    SECTION("c order") {
        nd::array<float, 2, nd::order::c> arr;
        CHECK(arr.get_count() == 0);
        CHECK(arr.get_row_count() == 0);
        CHECK(arr.get_column_count() == 0);
        CHECK(arr.get_shape() == nd::shape<2>{0, 0});
    }

    SECTION("f order") {
        nd::array<float, 2, nd::order::f> arr;
        CHECK(arr.get_count() == 0);
        CHECK(arr.get_row_count() == 0);
        CHECK(arr.get_column_count() == 0);
        CHECK(arr.get_shape() == nd::shape<2>{0, 0});
    }
}

TEST_CASE_METHOD(array1d_fixture, "can crate 1d array", "[ndarray][array]") {
    SECTION("ensure shape") {
        REQUIRE(arr_.get_count() == count_);
        REQUIRE(arr_.get_count() == arr_.get_shape()[0]);
    }

    SECTION("ensure data") {
        const float* data = arr_.get_data();
        for (std::int64_t i = 0; i < count_; i++) {
            CAPTURE(i);
            REQUIRE(data[i] == i);
            REQUIRE(arr_.get(i) == i);
        }
    }
}

TEMPLATE_TEST_CASE_METHOD_SIG(array2d_fixture,
                              "can crate 2d array",
                              "[ndarray][array]",
                              ((nd::order O), O), nd::order::c, nd::order::f) {
    using fx_t = array2d_fixture<O>;

    SECTION("ensure shape") {
        REQUIRE(fx_t::arr_.get_count() == fx_t::count_);
        REQUIRE(fx_t::arr_.get_row_count() == fx_t::row_count_);
        REQUIRE(fx_t::arr_.get_column_count() == fx_t::column_count_);
        REQUIRE(fx_t::arr_.get_shape() == nd::shape<2>{fx_t::row_count_, fx_t::column_count_});
    }

    SECTION("ensure data") {
        for (std::int64_t i = 0; i < fx_t::row_count_; i++) {
            for (std::int64_t j = 0; j < fx_t::column_count_; j++) {
                CAPTURE(i, j);
                REQUIRE(fx_t::arr_.get(i, j) == fx_t::get_reference(i, j));
            }
        }
    }
}

} // namespace oneapi::dal::backend::ndarray::test
