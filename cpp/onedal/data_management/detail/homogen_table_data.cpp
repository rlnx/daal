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

#include "onedal/data_management/detail/homogen_table_data.hpp"

using std::int32_t;

namespace dal {
namespace data_management {
namespace detail {

float* homogen_table_data::get_data_ptr(const slice& s, float*) const {
    return get_slice_impl<float>(s);
}

double* homogen_table_data::get_data_ptr(const slice& s, double*) const {
    return get_slice_impl<double>(s);
}

int32_t* homogen_table_data::get_data_ptr(const slice& s, int32_t*) const {
    return get_slice_impl<int32_t>(s);
}

void homogen_table_data::release_data_ptr(const slice& s, float* data) {
    release_slice_impl(s, data);
}

void homogen_table_data::release_data_ptr(const slice& s, double* data) {
    release_slice_impl(s, data);
}

void homogen_table_data::release_data_ptr(const slice& s, int32_t* data) {
    release_slice_impl(s, data);
}

struct index_pair {
    int32_t x;
    int32_t y;

    void swap() {
        int32_t tmp = x;
        x = y;
        y = tmp;
    }
};

struct slice_info {
    index_pair size;
    index_pair offset;
    index_pair step;

    int32_t ld_data;

    slice_info(const slice& s, int32_t num_rows, int32_t num_cols, const data_format& fmt) 
        : size({ s.cols.get_num_of_elements(num_cols), s.rows.get_num_of_elements(num_rows) }),
          offset({ s.cols.start_idx, s.rows.start_idx }),
          step({ s.cols.step, s.rows.step }),
          ld_data(num_cols){
        if (fmt == data_format::colmajor) {
            size.swap();
            offset.swap();
            step.swap();
            ld_data = num_rows;
        }
    }
};

bool need_copy_data(const slice_info& info) {
    return (info.step.x != 1) || 
           (info.step.y != 1) || 
           (info.size.y > 1 && info.size.x != info.ld_data);
}

template <typename DataType>
DataType* homogen_table_data::get_slice_impl(const slice& s) const {
    slice_info info { s, get_num_rows(), get_num_cols(), get_data_format() };

    if (_type_rt == dal::detail::make_type_rt<DataType>()) {
        DataType* data = reinterpret_cast<DataType*>(_data_bytes);

        if (need_copy_data(info) == true) {
            DataType* out_array = new DataType[info.size.x * info.size.y];
            for (int y = 0; y < info.size.y; y++) {
                for (int x = 0; x < info.size.x; x++) {
                    const int32_t data_x = x*info.step.x + info.offset.x;
                    const int32_t data_y = y*info.step.y + info.offset.y;
                    const int32_t ld_out = info.size.x;
                    const int32_t ld_data = info.ld_data;

                    out_array[y*ld_out + x] = data[data_y*ld_data + data_x];
                }
            }
            return out_array;
        }
        else {
            return data + info.offset.x + info.offset.y*info.ld_data;
        }
    }
    // TODO: implement conversion
    return nullptr;
}

template <typename DataType>
void homogen_table_data::release_slice_impl(const slice& s, DataType* data) {
    slice_info info { s, get_num_rows(), get_num_cols(), get_data_format() };

    if (_type_rt == dal::detail::make_type_rt<DataType>()) {

        if (need_copy_data(info) == true) {
            DataType* dest = reinterpret_cast<DataType*>(_data_bytes);
    
            for (int y = 0; y < info.size.y; y++) {
                for (int x = 0; x < info.size.x; x++) {
                    const int32_t data_x = x*info.step.x + info.offset.x;
                    const int32_t data_y = y*info.step.y + info.offset.y;
                    const int32_t ld_out = info.size.x;
                    const int32_t ld_data = info.ld_data;

                    dest[data_y*ld_data + data_x] = data[y*ld_out + x];
                }
            }

            delete[] data;
        }
        else {
            // TODO: think about case where data must be readonly
        }
    }
    // TODO: implement conversion
}

} // namespace detail
} // namespace data_management
} // namespace dal
