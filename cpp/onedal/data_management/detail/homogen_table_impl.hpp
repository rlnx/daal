#pragma once

#include "onedal/common.hpp"
#include "onedal/data_management/data_format.hpp"
#include "onedal/data_management/homogen_table.hpp"
#include "onedal/data_management/detail/table_impl.hpp"
#include "onedal/detail/type_rt.hpp"

#include <cstring>

namespace dal {
namespace data_management {
namespace detail {

class HomogenTableImpl : public TableImpl {
public:
    // TODO: figure out about using DataFormat object from public API here
    template <typename DataType>
    HomogenTableImpl(const DataType* data,
                     std::int32_t rows, std::int32_t cols,
                     DataFormat fmt)
        : TableImpl(rows, cols),
          _fmt(fmt),
          _type_rt(dal::detail::make_type_rt<DataType>()),
          _data_bytes(init_data(data, rows * cols * sizeof(DataType)))
    { }

    ~HomogenTableImpl() {
        delete[] _data_bytes;
        _data_bytes = nullptr;
    }

    dal::detail::TypeRT get_type() const noexcept {
        return _type_rt;
    }

    DataFormat get_data_format() const noexcept {
        return _fmt;
    }

    // TODO: warning! Usage of public class Range
    virtual float* get_slice(float* src, Range rows, Range cols) const override;

    // TODO: warning! Usage of public class Range
    virtual double* get_slice(double* src, Range rows, Range cols) const override;

    // TODO: warning! Usage of public class Range
    virtual std::int32_t* get_slice(std::int32_t* src, Range rows, Range cols) const override;

    // TODO: warning! Usage of public class Range
    virtual void release_slice(float* data, Range rows, Range cols) override;

    // TODO: warning! Usage of public class Range
    virtual void release_slice(double* data, Range rows, Range cols) override;

    // TODO: warning! Usage of public class Range
    virtual void release_slice(std::int32_t* data, Range rows, Range cols) override;

private:
    template <typename DataType>
    static char* init_data(const DataType* data, std::int64_t size_bytes) {
        // TODO: use internal allocators
        char* bytes = new char[size_bytes];
        // TODO: use internal memcpy routine
        std::memcpy(bytes, data, size_bytes);
        return bytes;
    }

    template <typename DataType>
    DataType* get_slice_impl(Range rows, Range cols) const;

private:
    DataFormat _fmt;
    dal::detail::TypeRT _type_rt;

    char* _data_bytes;
    bool _is_data_copied;
};

template <typename DataType>
HomogenTable create_table(const DataType* data, uint32_t rows, uint32_t cols, DataFormat df) {
    return HomogenTable(data, rows, cols, df);
}

} // namespace detail
} // namespace data_management
} // namespace dal
