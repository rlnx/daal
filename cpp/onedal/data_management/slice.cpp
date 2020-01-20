#include "onedal/data_management/slice.hpp"
#include "onedal/data_management/detail/slicable.hpp"

#include "onedal/data_management/array.hpp"
#include "onedal/data_management/detail/array_impl.hpp"

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

namespace detail {

// TODO: move to detail folder, use internal allocator instead of new()
template <typename T>
Array<T> create_array(Slice& slice) {
    using ArrayPimpl = typename Array<T>::Pimpl;
    return Array<T>(ArrayPimpl(new ArrayImpl<T>(slice._data_owner, slice._rows, slice._cols)));
}

template Array<float> create_array(Slice&);
template Array<double> create_array(Slice&);
template Array<std::int32_t> create_array(Slice&);

} // namespace detail

} // namespace data_management
} // namespace dal
