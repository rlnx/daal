#include "onedal/data_management/table.hpp"
#include "onedal/data_management/detail/table_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {

int32_t Table::get_num_rows() const noexcept {
    return _impl->get_num_rows();
}

int32_t Table::get_num_cols() const noexcept {
    return _impl->get_num_cols();
}

Slice Table::row(int32_t idx) const {
    // TODO: this method is the same with row() in Slice class.
    // need to generalize it?
    return Slice(_impl, Range{idx, idx+1}, Range{});
}

Slice Table::col(int32_t idx) const {
    // TODO: this method is the same with col() in Slice class.
    // need to generalize it?
    return Slice(_impl, Range{}, Range {idx, idx+1});
}

Slice Table::rows(const Range& r) const {
    // TODO: this method is the same with rows() in Slice class.
    // need to generalize it?
    return Slice(_impl, r, Range{});
}
Slice Table::cols(const Range& r) const {
    // TODO: this method is the same with cols() in Slice class.
    // need to generalize it?
    return Slice(_impl, Range{}, r);
}

} // namespace data_management
} // namespace dal
