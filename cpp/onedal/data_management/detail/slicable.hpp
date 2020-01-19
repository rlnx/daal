#pragma once

#include "onedal/common.hpp"

namespace dal {
namespace data_management {
namespace detail {

class Slicable : public Base {
public:
    virtual std::int32_t get_num_rows() const noexcept = 0;
    virtual std::int32_t get_num_cols() const noexcept = 0;
};

} // namespace detail
} // namespace data_management
} // namespace dal