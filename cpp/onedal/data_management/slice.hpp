#pragma once
#include "onedal/common.hpp"
#include "onedal/detail/common.hpp"

namespace dal {
namespace data_management {

namespace detail {
class Slicable;
} // namespace detail

// TODO: think of how to semantically distinguish
// "simple" and "complex" types
class Slice : public Base {
public:
    using PimplDataOwner = dal::detail::Pimpl<detail::Slicable>;

public:
    Slice(const PimplDataOwner& impl, Range rows_range, Range cols_range)
        : _data_owner(impl),
          _rows(rows_range),
          _cols(cols_range)
    { } // TODO: check that ranges are correct

    std::int32_t get_num_rows() const noexcept;
    std::int32_t get_num_cols() const noexcept;

    const Range& get_rows_range() const noexcept {
        return _rows;
    }

    const Range& get_cols_range() const noexcept {
        return _cols;
    }

    // TODO: too big exposure of details
    // need to make SliceImpl?
    PimplDataOwner get_data_owner_impl() const noexcept {
        return _data_owner;
    }

    Slice& row(std::int32_t idx);
    Slice& col(std::int32_t idx);
    Slice& rows(Range r);
    Slice& cols(Range r);

private:
    PimplDataOwner _data_owner;
    Range _rows;
    Range _cols;
};

} // namespace data_management
} // namespace dal
