#include "onedal/data_management/detail/homogen_table_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {
namespace detail {

float* HomogenTableImpl::get_slice(float* src, Range rows, Range cols) const {
    return get_slice_impl<float>(rows, cols);
}

double* HomogenTableImpl::get_slice(double* src, Range rows, Range cols) const {
    return get_slice_impl<double>(rows, cols);
}

int32_t* HomogenTableImpl::get_slice(int32_t* src, Range rows, Range cols) const {
    return get_slice_impl<int32_t>(rows, cols);
}

void HomogenTableImpl::release_slice(float* data, Range rows, Range cols) {

}

void HomogenTableImpl::release_slice(double* data, Range rows, Range cols) {

}

void HomogenTableImpl::release_slice(int32_t* data, Range rows, Range cols) {

}

template <typename DataType>
DataType* HomogenTableImpl::get_slice_impl(Range rows, Range cols) const {
    const int32_t end_row = rows.end_idx == rows.last_element_idx ? get_num_rows() : rows.end_idx;
    const int32_t end_col = cols.end_idx == cols.last_element_idx ? get_num_cols() : cols.end_idx;

    const int32_t ld_data = (_fmt == DataFormat::rowmajor) ? get_num_cols() : get_num_rows();

    int32_t offset_x = cols.start_idx;
    int32_t offset_y = rows.start_idx;
    int32_t step_x = cols.step;
    int32_t step_y = rows.step;
    int32_t num_x = (end_col - cols.start_idx - 1) / cols.step + 1;
    int32_t num_y = (end_row - rows.start_idx - 1) / rows.step + 1;

    if (_fmt == DataFormat::colmajor) {
        offset_x = rows.start_idx;
        offset_y = cols.start_idx;
        step_x = rows.step;
        step_y = cols.step;
        num_x = (end_row - rows.start_idx - 1) / rows.step + 1;
        num_y = (end_col - cols.start_idx - 1) / cols.step + 1;
    }

    if (_type_rt == dal::detail::make_type_rt<DataType>()) {
        const bool need_copy = (step_x != 1) || (step_y != 1) || (num_y > 1 && num_x != ld_data);

        DataType* data = reinterpret_cast<DataType*>(_data_bytes);

        if (need_copy) {
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

} // namespace detail
} // namespace data_management
} // namespace dal
