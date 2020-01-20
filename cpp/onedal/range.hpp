#pragma once
#include "onedal/integral_types.hpp"

namespace dal {

  // TODO: think of how to semantically distinguish
  // "simple" and "complex" types
  struct Range {
    static constexpr std::int32_t first_element_idx = 0;
    static constexpr std::int32_t last_element_idx = -1;

    std::int32_t start_idx;
    std::int32_t end_idx;
    std::int32_t step;

    Range(std::int32_t start,
          std::int32_t end,
          std::int32_t step = 1)
      : start_idx(start),
        end_idx(end),
        step(step)
    { }
  };

} // namespace dal
