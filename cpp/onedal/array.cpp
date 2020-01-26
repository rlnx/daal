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

#include "onedal/array.hpp"
#include "onedal/detail/array_impl.hpp"

using std::int32_t;
using std::int64_t;

namespace dal {

template <typename T>
array<T>::array(T* data, std::int64_t size)
    : _impl(new detail::array_impl<T>(data, size, deleter_ptr{ new detail::empty_deleter<T>() })) 
//TODO: check data size
{ }

template <typename T>
array<T>::array(T* data, std::int64_t size, const deleter_ptr& deleter)
    : _impl(new detail::array_impl<T>(data, size, deleter)) 
//TODO: check data size
{ }

template <typename T>
T* array<T>::get_pointer() const noexcept {
    return _impl->get_data_ptr();
}

template <typename T>
int64_t array<T>::get_size() const noexcept {
    return _impl->get_size();
}

template class array<float>;
template class array<double>;
template class array<int32_t>;

} // namespace dal
