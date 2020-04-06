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

#include "onedal/detail/common.hpp"
#include "onedal/detail/data_storage.hpp"
#include "onedal/detail/table_impl.hpp"
#include "onedal/detail/type_traits.hpp"
#include "onedal/table_metadata.hpp"

namespace dal {

template <typename T>
struct is_table_impl {
    INSTANTIATE_HAS_METHOD(get_feature_count,     std::int64_t,          () const);
    INSTANTIATE_HAS_METHOD(get_observation_count, std::int64_t,          () const);
    INSTANTIATE_HAS_METHOD(get_metadata,          const table_metadata&, () const);

    static constexpr bool value = has_method_get_feature_count_v<T> &&
                                  has_method_get_observation_count_v<T> &&
                                  has_method_get_metadata_v<T>;
};

template <typename T>
inline constexpr bool is_table_impl_v = is_table_impl<T>::value;

class table {
    friend detail::pimpl_accessor;

public:
    table() = default;
    table(const table&) = default;
    table(table&&) = default;

    template <typename TableImpl,
              typename = std::enable_if_t<is_table_impl_v<std::decay_t<TableImpl>>> >
    table(TableImpl&& impl)
        : table(new detail::table_impl_wrapper(std::forward<TableImpl>(impl))) { }

    table& operator=(const table&) = default;
    table& operator=(table&&) = default;

    bool is_empty() const noexcept;
    std::int64_t get_feature_count() const;
    std::int64_t get_observation_count() const;
    const table_metadata& get_metadata() const;

protected:
    table(const detail::table_impl* impl);
    table(const detail::data_storage_iface* storage);

private:
    detail::pimpl<const detail::table_impl> impl_;
};

class homogen_table : public table {
public:
    homogen_table() = default;
    homogen_table(const homogen_table&) = default;
    homogen_table(homogen_table&&) = default;

    homogen_table(std::int64_t observation_count, std::int64_t feature_count,
                  feature_info feature = feature_info{ data_type::float32 },
                  data_layout layout = data_layout::row_major);

    template <typename DataType>
    homogen_table(std::int64_t observation_count, std::int64_t feature_count,
                  const DataType* data_pointer,
                  data_layout layout = data_layout::row_major);

    homogen_table& operator=(const homogen_table&) = default;
    homogen_table& operator=(homogen_table&&) = default;

    template <typename DataType>
    const DataType* get_data_pointer() const;
};

} // namespace dal
