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
#include "onedal/data_management/detail/slicable.hpp"
#include "onedal/detail/common.hpp"

namespace dal {
namespace data_management {
namespace detail {

template <typename T>
class array_impl {
public:
    using data_owner_ptr = dal::detail::pimpl<slicable>;

public:
    // TODO: warning! Usage of public class range
    array_impl(const data_owner_ptr& dataOwner, range rows, range cols)
        : _dataOwner(dataOwner),
          _rows(rows),
          _cols(cols) {
        _data = _dataOwner->get_slice(_data, _rows, _cols);
    }

    ~array_impl() {
        _dataOwner->release_slice(_data, _rows, _cols);
    }

    T* get_data_ptr() const noexcept {
        return _data;
    }

private:
    data_owner_ptr _dataOwner;

    range _rows;
    range _cols;

    T* _data;
};

} // namespace detail
} // namespace data_management
} // namespace dal
