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

#include "onedal/common.hpp"
#include "onedal/detail/common.hpp"
#include "onedal/data_management/detail/slicable.hpp"
#include "onedal/data_management/detail/slice_impl.hpp"

namespace dal {
namespace data_management {
namespace detail {

// TODO: this is particular array impl for slices,
// rename it
template <typename T>
class array_impl {
public:
    using slice_ptr = dal::detail::pimpl<slice_impl>;

public:
    array_impl(const slice_ptr& slice)
        : _slice(slice) {
        _data = _slice->data_owner->get_slice_data(*_slice, _data);
    }

    ~array_impl() {
        _slice->data_owner->release_slice_data(*_slice, _data);
    }

    T* get_data_ptr() const noexcept {
        return _data;
    }

private:
    slice_ptr _slice;
    T* _data;
};

} // namespace detail
} // namespace data_management
} // namespace dal
