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

#include "onedal/array.hpp"
#include "onedal/table.hpp"

namespace dal {

template <typename AccessType>
class row_accessor {
private:
    using storage_t = detail::get_dense_storage_iface_t<AccessType>;

public:
    using data_t = std::remove_const_t<AccessType>;
    static constexpr bool is_readonly = std::is_const_v<AccessType>;

    template <typename T = AccessType>
    row_accessor(const table& t, std::enable_if_t<sizeof(T) && is_readonly>* = nullptr)
        : storage_(detail::get_impl<storage_t>(t)) { }

    array<data_t> pull(const range& rows = {0, -1}) const {
        array<data_t> block;
        storage_.pull_rows(block, rows);
        return block;
    }

    data_t* pull(array<data_t>& block, const range& rows = {0, -1}) const {
        storage_.pull_rows(block, rows);
        return block.get_pointer();
    }

    template<typename T = AccessType>
    std::enable_if_t<sizeof(T) && !is_readonly> push(const array<data_t>& block,
                                                     const range& rows = {0, -1}) {
        storage_.push_rows(block, rows);
    }

private:
    storage_t& storage_;
};

} // namespace dal
