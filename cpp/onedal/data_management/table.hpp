#pragma once

#include "onedal/common.hpp"
#include "onedal/detail/common.hpp"
#include "onedal/data_management/slice.hpp"

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

    Table(const Pimpl& impl)
        : _impl(impl)
    { }

    std::int32_t get_num_rows() const noexcept;
    std::int32_t get_num_cols() const noexcept;

    // TODO: this methods is the same with row(), rows(),
    // col(), cols() in Slice class.
    // need to generalize them?
    Slice row(std::int32_t idx) const;
    Slice col(std::int32_t idx) const;
    Slice rows(const Range& r) const;
    Slice cols(const Range& r) const;

    detail::TableImpl* get_impl_ptr() const noexcept {
        return _impl.get();
    }

    Pimpl get_impl() const noexcept {
        return _impl;
    }

private:
    Pimpl _impl;
};

} // namespace data_management
} // namespace dal
