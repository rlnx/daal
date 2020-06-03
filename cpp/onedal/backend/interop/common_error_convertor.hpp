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


#include "onedal/exception.hpp"
#include "daal/include/services/error_handling.h"
#include "daal/include/services/internal/error_handling_w_a.h"

namespace dal::backend::interop {

#define CONVERT(error, exception, description) \
case error: \
    throw exception(description);

void common_status_to_exception(daal::services::Status s) {
    using namespace daal::services;
    const ErrorID error = daal::services::internal::get_error_id(s);
    const char * description = s.getDescription();

    switch (error) {
        CONVERT(ErrorID::ErrorPCAFailedToComputeCorrelationEigenvalues, dal::internal_error, description)
    }
}

#undef CONVERT

} // namespace dal::backend::interop
