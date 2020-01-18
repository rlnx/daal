#include "onedal/data_management/table.hpp"
#include "onedal/data_management/detail/table_impl.hpp"

namespace dal {
namespace data_management {

std::int32_t Table::get_num_rows() const noexcept {
    return _impl->get_num_rows();
}

std::int32_t Table::get_num_cols() const noexcept {
    return _impl->get_num_cols();
}

} // namespace data_management
} // namespace dal
