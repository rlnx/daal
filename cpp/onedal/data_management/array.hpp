#pragma once

#include "onedal/common.hpp"
#include "onedal/detail/common.hpp"

namespace dal {
namespace data_management {

namespace detail {
template <typename T> class ArrayImpl;
} // namespace detail


template <typename T>
class Array {
public:
    using Pimpl = dal::detail::Pimpl< detail::ArrayImpl<T> >;

public:
    Array(const Pimpl& impl)
        : _impl(impl)
    { }

    const T* get_host_ptr() const noexcept;

private:
    Pimpl _impl;
};

} // namespace data_management
} // namespace dal
