#pragma once

#include "onedal/common.hpp"
#include "onedal/data_management/data_format.hpp"
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

    template <typename DataType>
    const DataType* get_data_ptr() const noexcept {
        return static_cast<DataType*>(_data_bytes);
    }

    dal::detail::TypeRT get_type() const noexcept {
        return _type_rt;
    }

    DataFormat get_data_format() const noexcept {
        return _fmt;
    }

private:
    template <typename DataType>
    static char* init_data(const DataType* data, std::int64_t size_bytes) {
        // TODO: use internal allocators
        char* bytes = new char[size_bytes];
        // TODO: use internal memcpy routine
        std::memcpy(bytes, data, size_bytes);
        return bytes;
    }

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
