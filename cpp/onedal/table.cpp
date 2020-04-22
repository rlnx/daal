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

#include "onedal/detail/homogen_table_impl.hpp"
#include "onedal/table.hpp"

using std::int64_t;

namespace dal {

table::table()
    : table(new detail::empty_table_impl()) {}

table::table(table&& t)
    : impl_(std::move(t.impl_)) {
    t.impl_ = detail::shared<detail::table_impl_iface>(new detail::empty_table_impl());
}

table& table::operator=(table&& t) {
    this->impl_ = std::move(t.impl_);
    t.impl_ = detail::shared<detail::table_impl_iface>(new detail::empty_table_impl());
}

bool table::has_data() const noexcept {
    return impl_->get_column_count() > 0 && impl_->get_row_count() > 0;
}

int64_t table::get_column_count() const {
    return impl_->get_column_count();
}

int64_t table::get_row_count() const {
    return impl_->get_row_count();
}

const table_metadata& table::get_metadata() const {
    return impl_->get_metadata();
}

table::table(detail::table_impl_iface* impl)
    : impl_(impl) {}

template <typename DataType>
homogen_table::homogen_table(int64_t row_count, int64_t column_count,
                             const DataType* data_pointer,
                             data_layout layout)
    : table(new detail::homogen_table_impl(row_count, column_count, data_pointer, layout)) {}

template <typename DataType>
const DataType* homogen_table::get_data_pointer() const {
    return detail::get_impl<const detail::homogen_table_impl>(*this).get_data_pointer<DataType>();
}

template homogen_table::homogen_table(int64_t, int64_t, const float*, data_layout);
template homogen_table::homogen_table(int64_t, int64_t, const double*, data_layout);
template homogen_table::homogen_table(int64_t, int64_t, const std::int32_t*, data_layout);

template const float* homogen_table::get_data_pointer() const;
template const double* homogen_table::get_data_pointer() const;
template const std::int32_t* homogen_table::get_data_pointer() const;

} // namespace dal
