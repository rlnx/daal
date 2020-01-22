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

#include "onedal/data_management/detail/homogen_table_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {
namespace detail {

float* homogen_table_impl::get_data_ptr(const slice& s, float*) const {
    return get_slice_impl<float>(s);
}

double* homogen_table_impl::get_data_ptr(const slice& s, double*) const {
    return get_slice_impl<double>(s);
}

int32_t* homogen_table_impl::get_data_ptr(const slice& s, int32_t*) const {
    return get_slice_impl<int32_t>(s);
}

void homogen_table_impl::release_data_ptr(const slice& s, float* data) {
    release_slice_impl(s, data);
}

void homogen_table_impl::release_data_ptr(const slice& s, double* data) {
    release_slice_impl(s, data);
}

void homogen_table_impl::release_data_ptr(const slice& s, int32_t* data) {
    release_slice_impl(s, data);
}

template <typename DataType>
DataType* homogen_table_impl::get_slice_impl(const slice& s) const {
    const int32_t end_row = s.rows.get_num_of_elements(get_num_rows());
    const int32_t end_col = s.cols.get_num_of_elements(get_num_cols());
    const int32_t ld_data = (_fmt == data_format::rowmajor) ? get_num_cols() : get_num_rows();

    const int32_t offset_x = (_fmt == data_format::rowmajor) ? s.cols.start_idx : s.rows.start_idx;
    const int32_t offset_y = (_fmt == data_format::rowmajor) ? s.rows.start_idx : s.cols.start_idx;
    const int32_t step_x = (_fmt == data_format::rowmajor) ? s.cols.step : s.rows.step;
    const int32_t step_y = (_fmt == data_format::rowmajor) ? s.rows.step : s.cols.step;
    const int32_t num_x = (_fmt == data_format::rowmajor) ? end_col : end_row;
    const int32_t num_y = (_fmt == data_format::rowmajor) ? end_row : end_col;

    if (_type_rt == dal::detail::make_type_rt<DataType>()) {
        const bool need_copy_data = (step_x != 1) || (step_y != 1) || (num_y > 1 && num_x != ld_data);

        DataType* data = reinterpret_cast<DataType*>(_data_bytes);

        if (need_copy_data) {
            DataType* out_array = new DataType[num_x * num_y];
            for (int y = 0; y < num_y; y++) {
                for (int x = 0; x < num_x; x++) {
                    out_array[y*num_x + x] = data[(y*step_y+offset_y)*ld_data + (x*step_x+offset_x)];
                }
            }
            return out_array;
        }
        else {
            return data + offset_x + offset_y*ld_data;
        }
    }
    // TODO: implement conversion
    return nullptr;
}

template <typename DataType>
void homogen_table_impl::release_slice_impl(const slice& s, DataType* data) {
    // TODO: implement data update
}

} // namespace detail
} // namespace data_management
} // namespace dal
