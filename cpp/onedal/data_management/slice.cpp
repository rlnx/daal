#include "onedal/data_management/slice.hpp"
#include "onedal/data_management/detail/slicable.hpp"

using std::int32_t;

namespace dal {
namespace data_management {

int32_t Slice::get_num_rows() const noexcept {
    auto rows_total = _data_owner->get_num_rows();
    auto final_row = _rows.end_idx == _rows.maximum_idx ? rows_total : _rows.end_idx;
    return (final_row - _rows.start_idx - 1) / _rows.step + 1;
}

int32_t Slice::get_num_cols() const noexcept {
    auto cols_total = _data_owner->get_num_cols();
    auto final_col = _cols.end_idx == _cols.maximum_idx ? cols_total : _cols.end_idx;
    return (final_col - _cols.start_idx - 1) / _cols.step + 1;
}
    
} // namespace data_management
} // namespace dal
