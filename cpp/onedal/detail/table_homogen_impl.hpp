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

#include <cstring>

#include "onedal/types_data.hpp"
#include "onedal/detail/table_impl.hpp"
#include "onedal/detail/type_rt.hpp"

namespace dal {
namespace detail {

class table_homogen_impl : public table_impl {
public:
    template <typename DataType>
    table_homogen_impl(const DataType* data,
                       std::int64_t rows, std::int64_t cols)
        : table_impl(rows, cols),
          type_rt_(dal::detail::make_type_rt<DataType>()),
          data_bytes_(init_data(data, rows * cols * sizeof(DataType)))
    { }

    ~table_homogen_impl() {
        delete[] data_bytes_;
        data_bytes_ = nullptr;
    }

    dal::detail::type_rt get_type() const noexcept {
        return type_rt_;
    }

    virtual float*        get_data_ptr(const table_range&, float*) const override;
    virtual double*       get_data_ptr(const table_range&, double*) const override;
    virtual std::int32_t* get_data_ptr(const table_range&, std::int32_t*) const override;

    virtual void release_data_ptr(const table_range&, float*,        bool need_copy_ptr) override;
    virtual void release_data_ptr(const table_range&, double*,       bool need_copy_ptr) override;
    virtual void release_data_ptr(const table_range&, std::int32_t*, bool need_copy_ptr) override;

private:
    template <typename DataType>
    static char* init_data(const DataType* data, std::int64_t size_bytes) {
        // TODO: use internal allocators
        char* bytes = new char[size_bytes];
        // TODO: use internal memcpy routine
        std::memcpy(bytes, data, size_bytes);
        return bytes;
    }

    template <typename DataType>
    DataType* get_slice_impl(const table_range&) const;

    template <typename DataType>
    void release_slice_impl(const table_range&, DataType*, bool need_copy_ptr);

private:
    dal::detail::type_rt type_rt_;

    char* data_bytes_;
    bool is_data_copied_;
};

} // namespace detail
} // namespace dal
