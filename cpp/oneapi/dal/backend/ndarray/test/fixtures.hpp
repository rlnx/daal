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

#include "oneapi/dal/backend/ndarray/array.hpp"

namespace oneapi::dal::backend::ndarray::test {

namespace nd = oneapi::dal::backend::ndarray;

class array1d_fixture {
public:
    array1d_fixture() {
        const auto linear = dal::array<float>::empty(count_);
        float* mutable_data = linear.get_mutable_data();
        for (std::int64_t i = 0; i < count_; i++) {
            mutable_data[i] = float(i);
        }
        arr_ = nd::array<float, 1>{linear, shape<1>{count_}};
    }

    nd::array<float, 1> arr_;
    const std::int64_t count_ = 10;
};

template <nd::order O>
class array2d_fixture {
public:
    array2d_fixture() {
        const auto linear = dal::array<float>::empty(count_);
        float* mutable_data = linear.get_mutable_data();
        for (std::int64_t i = 0; i < row_count_; i++) {
            for (std::int64_t j = 0; j < column_count_; j++) {
                if constexpr (O == nd::order::c) {
                    mutable_data[i * column_count_ + j] = get_reference(i, j);
                }
                else {
                    mutable_data[j * row_count_ + i] = get_reference(i, j);
                }
            }
        }
        arr_ = nd::array<float, 2, O>{linear, shape<2>{row_count_, column_count_}};
    }

    float get_reference(std::int64_t i, std::int64_t j) {
        return i * column_count_ + j;
    }

    nd::array<float, 2, O> arr_;
    const std::int64_t row_count_ = 7;
    const std::int64_t column_count_ = 5;
    const std::int64_t count_ = row_count_ * column_count_;
};

} // namespace oneapi::dal::backend::ndarray::test
