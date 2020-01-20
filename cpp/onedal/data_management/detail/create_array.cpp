#include "onedal/data_management/detail/create_array.hpp"
#include "onedal/data_management/detail/array_impl.hpp"

using std::int32_t;

namespace dal {
namespace data_management {
namespace detail {

// TODO: use internal allocator instead of new()
template <typename T>
Array<T> create_array(const Slice& slice) {
    using ArrayPimpl = typename Array<T>::Pimpl;
    return Array<T>(ArrayPimpl(new ArrayImpl<T>(slice.get_data_owner_impl(), 
                                                slice.get_rows_range(), 
                                                slice.get_cols_range())));
}

template Array<float> create_array(const Slice&);
template Array<double> create_array(const Slice&);
template Array<int32_t> create_array(const Slice&);

} // namespace detail
} // namespace data_management
} // namespace dal