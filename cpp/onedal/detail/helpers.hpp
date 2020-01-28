#pragma once

#include "onedal/types_data.hpp"

namespace dal {
namespace detail {

struct table_range {
    range rows;
    range columns;
};

} // namespace detail
} // namespace dal
