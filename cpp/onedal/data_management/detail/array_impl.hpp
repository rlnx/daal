#pragma once

#include "onedal/common.hpp"
#include "onedal/data_management/detail/slicable.hpp"
#include "onedal/detail/common.hpp"

namespace dal {
namespace data_management {
namespace detail {

template <typename T>
class ArrayImpl {
public:
    using DataOwnerPtr = dal::detail::Pimpl<Slicable>;

public:
    // TODO: warning! Usage of public class Range
    ArrayImpl(const DataOwnerPtr& dataOwner, Range rows, Range cols)
        : _dataOwner(dataOwner),
          _rows(rows),
          _cols(cols) {
        _data = _dataOwner->get_slice(_data, _rows, _cols);
    }

    ~ArrayImpl() {
        _dataOwner->release_slice(_data, _rows, _cols);
    }

    const T* get_data_ptr() const noexcept {
        return _data;
    }

private:
    DataOwnerPtr _dataOwner;

    Range _rows;
    Range _cols;

    T* _data;
};

} // namespace detail
} // namespace data_management
} // namespace dal
