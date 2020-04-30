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

#include <type_traits>

#include "onedal/detail/table_impl_wrapper.hpp"
#include "onedal/type_traits.hpp"

namespace dal {

class table {
    friend detail::pimpl_accessor;

protected:
    using pimpl = detail::pimpl<detail::table_impl_iface>;

public:
    table();
    table(const table&) = default;
    table(table&&);

    template <typename TableImpl,
              typename = std::enable_if_t<!std::is_base_of_v<table, std::decay_t<TableImpl>>>>
    table(TableImpl&& impl) {
        using impl_t = std::decay_t<TableImpl>;

        if constexpr (is_table_impl_v<impl_t>) {
            init_impl(new detail::table_impl_wrapper(std::forward<TableImpl>(impl)));
        } else {
            static_assert("implementation does not meet requirements");
        }
    }

    table& operator=(const table&) = default;
    table& operator=(table&&);

    bool has_data() const noexcept;
    std::int64_t get_column_count() const;
    std::int64_t get_row_count() const;
    const table_metadata& get_metadata() const;

protected:
    table(const pimpl& impl)
        : impl_(impl) {}

    void init_impl(pimpl::element_type* impl);

private:
    pimpl impl_;
};

class homogen_table : public table {
    friend detail::pimpl_accessor;

public:
    homogen_table() = default;

    template <typename DataType>
    homogen_table(std::int64_t row_count, std::int64_t column_count,
                  const DataType* data_pointer,
                  data_layout layout = data_layout::row_major);

    template <typename DataType>
    const DataType* get_data() const;

private:
    homogen_table(const pimpl& impl)
        : table(impl) {}
};

} // namespace dal
