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
#include "onedal/detail/storage_iface.hpp"
#include "onedal/table_metadata.hpp"

namespace dal::detail {

class table_impl : public dense_storage_iface<storage_readonly> {
public:
    table_impl(std::int64_t N, std::int64_t p,
               const table_metadata& meta)
        : N_(N),
          p_(p),
          meta_(meta) { }

    std::int64_t get_column_count() const noexcept {
        return p_;
    }

    std::int64_t get_row_count() const noexcept {
        return N_;
    }

    const table_metadata& get_metadata() const noexcept {
        return meta_;
    }

private:
    std::int64_t N_;
    std::int64_t p_;

    table_metadata meta_;
};

template <typename TableImpl>
class table_impl_wrapper : public table_impl {
public:
    //TODO: do we want to have a copy-constructor for impl?

    table_impl_wrapper(TableImpl&& obj)
        : table_impl(obj.get_column_count(),
                     obj.get_row_count(),
                     obj.get_metadata()),
          impl_(std::move(obj)) { }

    virtual void pull_rows(array<float>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

    virtual void pull_rows(array<double>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

    virtual void pull_rows(array<std::int32_t>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

private:
    TableImpl impl_;
};

} // namespace dal::detail
