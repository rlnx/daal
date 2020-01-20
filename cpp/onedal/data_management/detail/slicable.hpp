#pragma once

#include "onedal/common.hpp"

namespace dal {
namespace data_management {
namespace detail {

class Slicable : public Base {
public:
    virtual std::int32_t get_num_rows() const noexcept = 0;
    virtual std::int32_t get_num_cols() const noexcept = 0;

    // TODO: warning! Usage of public class Range
    virtual float* get_slice(float* src, Range rows, Range cols) const = 0;
    // TODO: warning! Usage of public class Range
    virtual double* get_slice(double* src, Range rows, Range cols) const = 0;
    // TODO: warning! Usage of public class Range
    virtual std::int32_t* get_slice(std::int32_t* src, Range rows, Range cols) const = 0;

    // TODO: warning! Usage of public class Range
    virtual void release_slice(float* data, Range rows, Range cols) = 0;
    // TODO: warning! Usage of public class Range
    virtual void release_slice(double* data, Range rows, Range cols) = 0;
    // TODO: warning! Usage of public class Range
    virtual void release_slice(std::int32_t* data, Range rows, Range cols) = 0;
};

} // namespace detail
} // namespace data_management
} // namespace dal
