#pragma once

#include "onedal/common.hpp"

namespace dal {
namespace data_management {
namespace detail {

class TableImpl : public Base {
public:
    TableImpl(std::int32_t rows, std::int32_t cols)
        : _rows(rows)
        , _cols(cols)
    { }

    std::int32_t get_num_rows() const noexcept {
        return _rows;
    }

    std::int32_t get_num_cols() const noexcept {
        return _cols;
    }

private:
    std::int32_t _rows;
    std::int32_t _cols;
};

} // namespace detail
} // namespace data_management
} // namespace dal
