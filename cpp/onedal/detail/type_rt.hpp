#pragma once

namespace dal {
namespace detail {

enum class TypeRT {
    fp32,
    fp64
};

template <typename DataType>
TypeRT make_type_rt();

template <>
TypeRT make_type_rt<float>() {
    return TypeRT::fp32;
}

template <>
TypeRT make_type_rt<double>() {
    return TypeRT::fp64;
}

} // namespace detail
} // namespace dal
