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
    // TODO: check that index is correct. 
    // How to organize error handling?
    return Slice(_impl, Range{idx, idx+1}, Range{0, -1});
}

Slice Table::col(int32_t idx) const {
    // TODO: check that index is correct. 
    // How to organize error handling?
    return Slice(_impl, Range{0, -1}, Range {idx, idx+1});
}

Slice Table::rows(const Range& r) const {
    // TODO: check that range is correct. 
    // How to organize error handling?
    return Slice(_impl, r, Range{0, -1});
}
Slice Table::cols(const Range& r) const {
    // TODO: check that range is correct. 
    // How to organize error handling?
    return Slice(_impl, Range{0, -1}, r);
}

} // namespace data_management
} // namespace dal
