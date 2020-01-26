#pragma once

#include "onedal/ranges.hpp"

namespace dal {
namespace detail {

struct table_range {
    range rows;
    range columns;
};
    
} // namespace detail
} // namespace dal
