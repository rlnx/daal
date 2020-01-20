#include "onedal/data_management/array.hpp"
#include "onedal/data_management/detail/array_impl.hpp"

namespace dal {
namespace data_management {

template <typename T>
const T* Array<T>::get_host_ptr() const noexcept {
    return _impl->get_data_ptr();
}

template class Array<float>;
template class Array<double>;
template class Array<std::int32_t>;

} // namespace data_management
} // namespace dal
