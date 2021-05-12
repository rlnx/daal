/*******************************************************************************
* Copyright 2020-2021 Intel Corporation
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

#include "oneapi/dal/table/homogen.hpp"
#include "oneapi/dal/table/backend/homogen_table_impl.hpp"

namespace oneapi::dal {
namespace v1 {

static std::shared_ptr<detail::homogen_table_iface> get_homogen_iface(const table& other) {
    if (const auto homogen_iface = detail::get_homogen_table_iface(other)) {
        return homogen_iface;
    }
    return std::make_shared<backend::homogen_table_impl>();
}

int64_t homogen_table::kind() {
    return 1;
}

homogen_table::homogen_table() : homogen_table(new backend::homogen_table_impl{}) {}

homogen_table::homogen_table(const table& other) : homogen_table(get_homogen_iface(other)) {}

template <typename Policy>
void homogen_table::init_impl(const Policy& policy,
                              std::int64_t row_count,
                              std::int64_t column_count,
                              const array<byte_t>& data,
                              const data_type& dtype,
                              data_layout layout) {
    table::init_impl(
        new backend::homogen_table_impl{ row_count, column_count, data, dtype, layout });
}

const void* homogen_table::get_data() const {
    const auto& impl = detail::cast_impl<detail::homogen_table_iface>(*this);
    return impl.get_data().get_data();
}

template ONEDAL_EXPORT void homogen_table::init_impl(const detail::default_host_policy&,
                                                     std::int64_t,
                                                     std::int64_t,
                                                     const array<byte_t>&,
                                                     const data_type&,
                                                     data_layout);

#ifdef ONEDAL_DATA_PARALLEL
template ONEDAL_EXPORT void homogen_table::init_impl(const detail::data_parallel_policy&,
                                                     std::int64_t,
                                                     std::int64_t,
                                                     const array<byte_t>&,
                                                     const data_type&,
                                                     data_layout);
#endif

} // namespace v1
} // namespace oneapi::dal
