#pragma once
#include "onedal/integral_types.hpp"

namespace dal {
namespace detail {

enum class TypeRT {
    i32,
    f32,
    f64
};

template <typename DataType>
inline TypeRT make_type_rt();

template <>
inline TypeRT make_type_rt<std::int32_t>() {
    return TypeRT::i32;
}

template <>
inline TypeRT make_type_rt<float>() {
    return TypeRT::f32;
}

template <>
inline TypeRT make_type_rt<double>() {
    return TypeRT::f64;
}

} // namespace detail
} // namespace dal
