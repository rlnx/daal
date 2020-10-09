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

namespace oneapi::dal::test {

template <typename Float>
inline void kahan_add(Float input, Float& sum_accumulator, Float& compensation_accumulator) {
    const Float y = input - compensation_accumulator;
    const Float t = sum_accumulator + y;
    compensation_accumulator = (t - sum_accumulator) - y;
    sum_accumulator = t;
}

} // namespace oneapi::dal::test
