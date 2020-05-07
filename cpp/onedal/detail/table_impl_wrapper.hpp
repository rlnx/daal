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

#include "onedal/detail/table_impl_iface.hpp"

namespace dal::detail {

template <typename TableImpl>
class table_impl_wrapper : public table_impl_iface,
                           public base {
public:
    table_impl_wrapper(TableImpl&& obj)
        : impl_(std::move(obj)) { }

    virtual std::int64_t get_column_count() const override {
        return impl_.get_column_count();
    }

    virtual std::int64_t get_row_count() const override {
        return impl_.get_row_count();
    }

    virtual const table_metadata& get_metadata() const override {
        return impl_.get_metadata();
    }

    virtual void pull_rows(array<float>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

    virtual void pull_rows(array<double>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

    virtual void pull_rows(array<std::int32_t>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

    virtual void push_back_rows(const array<float>& block, const range& r) override {
        impl_.push_back_rows(block, r);
    }

    virtual void push_back_rows(const array<double>& block, const range& r) override {
        impl_.push_back_rows(block, r);
    }

    virtual void push_back_rows(const array<std::int32_t>& block, const range& r) override {
        impl_.push_back_rows(block, r);
    }

    TableImpl& get() {
        return impl_;
    }

private:
    TableImpl impl_;
};

// TODO: avoid duplication inside wrappers?
template <typename Impl>
class homogen_table_impl_wrapper : public homogen_table_impl_iface,
                                   public base {
public:
    homogen_table_impl_wrapper(Impl&& obj)
        : impl_(std::move(obj)) { }

    virtual std::int64_t get_column_count() const override {
        return impl_.get_column_count();
    }

    virtual std::int64_t get_row_count() const override {
        return impl_.get_row_count();
    }

    virtual const table_metadata& get_metadata() const override {
        return impl_.get_metadata();
    }

    virtual void pull_rows(array<float>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

    virtual void pull_rows(array<double>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

    virtual void pull_rows(array<std::int32_t>& block, const range& r) const override {
        impl_.pull_rows(block, r);
    }

    virtual void push_back_rows(const array<float>& block, const range& r) override {
        impl_.push_back_rows(block, r);
    }

    virtual void push_back_rows(const array<double>& block, const range& r) override {
        impl_.push_back_rows(block, r);
    }

    virtual void push_back_rows(const array<std::int32_t>& block, const range& r) override {
        impl_.push_back_rows(block, r);
    }

    virtual const void* get_data() const override {
        return impl_.get_data();
    }

    Impl& get() {
        return impl_;
    }

private:
    Impl impl_;
};
    
} // namespace dal::detail
