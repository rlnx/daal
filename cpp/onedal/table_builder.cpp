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

#include "onedal/backend/homogen_table_impl.hpp"
#include "onedal/table_builder.hpp"

namespace dal {
namespace backend {

class table_builder_impl {
    using dense_rw_storage = detail::table_builder_impl_iface::dense_rw_storage;

public:
    table_builder_impl(table&& t)
        : table_impl_(detail::pimpl_accessor().get_pimpl(std::move(t))) {}

    auto build_table() {
        return detail::pimpl_accessor().make_from_pimpl<table>(table_impl_);
    }

    auto get_storage() -> dense_rw_storage& {
        return *table_impl_;
    }

private:
    detail::pimpl<detail::table_impl_iface> table_impl_;
};

class homogen_table_builder_impl {
    using dense_rw_storage = detail::table_builder_impl_iface::dense_rw_storage;
    using table_impl = detail::table_impl_iface;

public:
    homogen_table_builder_impl(homogen_table&& t)
        : table_impl_(detail::pimpl_accessor().get_pimpl(std::move(t))) {}

    auto build_table() {
        return detail::pimpl_accessor().make_from_pimpl<homogen_table>(table_impl_);
    }

    auto get_storage() -> dense_rw_storage& {
        return *table_impl_;
    }

private:
    detail::pimpl<table_impl> table_impl_;
};

} // namespace backend

table_builder::table_builder(table&& t)
    : table_builder(backend::table_builder_impl{ std::move(t) }) {}

} // namespace dal
