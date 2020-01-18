#pragma once

#include "onedal/data_management/table.hpp"
#include "onedal/data_management/data_format.hpp"

namespace dal {
namespace data_management {

namespace detail {
class HomogenTableImpl;
} // namespace detail

class HomogenTable : public Table {
public:
    using Pimpl = dal::detail::Pimpl<detail::HomogenTableImpl>;

    HomogenTable(const HomogenTable& table)
        : Table(table.get_impl())
    { }

    template<typename DataType>
    HomogenTable(const DataType* data, uint32_t rows, uint32_t cols, DataFormat df);

    detail::HomogenTableImpl* get_impl_ptr() const noexcept {
        return reinterpret_cast<detail::HomogenTableImpl*>(Table::get_impl_ptr());
    }
};

} // namespace data_management
} // namespace dal
