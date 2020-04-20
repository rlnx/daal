/*******************************************************************************
 * Copyright 2020 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include "daal/include/data_management/data/internal/conversion.h"

#include "onedal/common.hpp"

#include <stdexcept>

namespace dal::backend::interop {

using namespace daal::data_management;

features::IndexNumType getIndexNumType(data_type t) {
    switch (t) {
        case data_type::int32:
            return features::DAAL_INT32_S;
        case data_type::int64:
            return features::DAAL_INT64_S;
        case data_type::uint32:
            return features::DAAL_INT32_U;
        case data_type::uint64:
            return features::DAAL_INT64_U;
        case data_type::float32:
            return features::DAAL_FLOAT32;
        case data_type::float64:
            return features::DAAL_FLOAT64;
        default:
            return features::DAAL_OTHER_T;
    }
}

internal::ConversionDataType getConversionDataType(data_type t) {
    switch (t) {
        case data_type::int32:
            return internal::DAAL_INT32;
        case data_type::float32:
            return internal::DAAL_SINGLE;
        case data_type::float64:
            return internal::DAAL_DOUBLE;
        default:
            return internal::DAAL_OTHER;
    }
}

void daal_convert(const void* src, void* dst,
                  data_type src_type, data_type dest_type,
                  std::int64_t size) {

    auto from_type = getIndexNumType(src_type);
    auto to_type = getConversionDataType(dest_type);

    auto check_types = [](auto from_type, auto to_type) {
        if (from_type == features::DAAL_OTHER_T || to_type == internal::DAAL_OTHER) {
            throw std::runtime_error("unsupported conversion types");
        }
    };

    if (getConversionDataType(dest_type) == internal::DAAL_OTHER &&
        getConversionDataType(src_type) != internal::DAAL_OTHER) {
        from_type = getIndexNumType(dest_type);
        to_type = getConversionDataType(src_type);

        check_types(from_type, to_type);
        internal::getVectorDownCast(from_type, to_type)(size, src, dst);
    } else {
        check_types(from_type, to_type);
        internal::getVectorUpCast(from_type, to_type)(size, src, dst);
    }
}

} // namespace dal::backend::interop

