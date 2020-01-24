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

#include "onedal/data_management/data_format.hpp"
#include "onedal/data_management/detail/table_data.hpp"
#include "onedal/detail/type_rt.hpp"

namespace dal {
namespace data_management {
namespace detail {

class homogen_table_data : public table_data {
public:
    // TODO: figure out about using DataFormat object from public API here
    template <typename DataType>
    homogen_table_data(const DataType* data,
                       std::int64_t rows, std::int64_t cols,
                       data_format fmt)
        : table_data(rows, cols),
          _fmt(fmt),
          _type_rt(dal::detail::make_type_rt<DataType>()),
          _data_bytes(init_data(data, rows * cols * sizeof(DataType)))
    { }

    ~homogen_table_data() {
        delete[] _data_bytes;
        _data_bytes = nullptr;
    }

    dal::detail::type_rt get_type() const noexcept {
        return _type_rt;
    }

    data_format get_data_format() const noexcept {
        return _fmt;
    }

    virtual float* get_data_ptr(const slice&, float*) const override;
    virtual double* get_data_ptr(const slice&, double*) const override;
    virtual std::int32_t* get_data_ptr(const slice&, std::int32_t*) const override;

    virtual void release_data_ptr(const slice&, float*, bool need_copy_ptr) override;
    virtual void release_data_ptr(const slice&, double*, bool need_copy_ptr) override;
    virtual void release_data_ptr(const slice&, std::int32_t*, bool need_copy_ptr) override;

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
    DataType* get_slice_impl(const slice&) const;

    template <typename DataType>
    void release_slice_impl(const slice&, DataType*, bool need_copy_ptr);

private:
    data_format _fmt;
    dal::detail::type_rt _type_rt;

    char* _data_bytes;
    bool _is_data_copied;
};

} // namespace detail
} // namespace data_management
} // namespace dal
