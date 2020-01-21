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

#include "onedal/data_management/detail/create_array.hpp"
#include "onedal/data_management/detail/array_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {
namespace detail {

// TODO: use internal allocator instead of new()
template <typename T>
array<T> create_array(const slice& slice) {
    using ArrayPimpl = typename array<T>::pimpl;
    return array<T>(ArrayPimpl(new ArrayImpl<T>(slice.get_data_owner_impl(),
                                                slice.get_rows_range(),
                                                slice.get_cols_range())));
}

template array<float> create_array(const slice&);
template array<double> create_array(const slice&);
template array<int32_t> create_array(const slice&);

} // namespace detail
} // namespace data_management
} // namespace dal
