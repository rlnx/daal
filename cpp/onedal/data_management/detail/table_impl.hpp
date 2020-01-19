#pragma once

#include "onedal/common.hpp"
#include "onedal/data_management/detail/slicable.hpp"

namespace dal {
namespace data_management {
namespace detail {

class TableImpl : public Slicable {
public:
    TableImpl(std::int32_t rows, std::int32_t cols)
        : _rows(rows)
        , _cols(cols)
    { }

    virtual std::int32_t get_num_rows() const noexcept override {
        return _rows;
    }

    virtual std::int32_t get_num_cols() const noexcept override {
        return _cols;
    }

private:
    std::int32_t _rows;
    std::int32_t _cols;
};

} // namespace detail
} // namespace data_management
} // namespace dal
