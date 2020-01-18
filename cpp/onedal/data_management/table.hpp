#pragma once

#include "onedal/common.hpp"
#include "onedal/detail/common.hpp"

namespace dal {
namespace data_management {

namespace detail {
class TableImpl;
} // namespace detail

class Table : public Base {
public:
    using Pimpl = dal::detail::Pimpl<detail::TableImpl>;

public:
    Table(const Table& table)
        : _impl(table._impl)
    { }

    std::int32_t get_num_rows() const noexcept;
    std::int32_t get_num_cols() const noexcept;

    detail::TableImpl* get_impl_ptr() const noexcept {
        return _impl.get();
    }

    Pimpl get_impl() const noexcept {
        return _impl;
    }

protected:
    Table(const Pimpl& impl)
        : _impl(impl)
    { }

private:
    Pimpl _impl;
};

} // namespace data_management
} // namespace dal
