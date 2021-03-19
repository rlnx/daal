/*******************************************************************************
* Copyright 2021 Intel Corporation
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

#include <iomanip>
#include <string>
#include <sstream>
#include <fmt/core.h>

#include "oneapi/dal/backend/memory.hpp"

namespace be = oneapi::dal::backend;

inline std::string format_value(double x, double base) {
    const double k = x / base;
    const double m = k / base;
    const double g = m / base;
    const double t = g / base;

    std::stringstream ss;
    ss << std::setprecision(5);

    if (t >= 1) {
        ss << t << "T";
    }
    else if (g >= 1) {
        ss << g << "G";
    }
    else if (m >= 1) {
        ss << m << "M";
    }
    else if (k >= 1) {
        ss << k << "K";
    }
    else {
        ss << x;
    }

    return ss.str();
}

inline std::string format_value(double x) {
    return format_value(x, 1000.0);
}

inline std::string format_memory(double x) {
    return format_value(x, 1024.0) + "b";
}

template <typename Op>
inline double measure_time_ms(Op op, std::size_t max_iter = 10) {
    double ms = std::numeric_limits<double>::max();
    for (std::size_t iter = 0; iter < max_iter; iter++) {
        sycl::event event = op();
        event.wait_and_throw();
        const std::uint64_t t0_ns =
            event.get_profiling_info<sycl::info::event_profiling::command_start>();
        const std::uint64_t t1_ns =
            event.get_profiling_info<sycl::info::event_profiling::command_end>();
        const double elapsed_ms = double(t1_ns - t0_ns) * 1e-6;
        ms = std::min(ms, elapsed_ms);
    }
    return ms;
}

template <typename Op>
inline double measure_bandwidth(std::size_t traffic_in_bytes, Op op, std::size_t max_iter = 10) {
    const double time_ms = measure_time_ms(op, max_iter);
    const double bandwidth_bytes_per_second = double(traffic_in_bytes) / time_ms * 1000.0;
    return bandwidth_bytes_per_second;
}

inline void measure_global_memory_bandwidth_peak(sycl::queue& q, size_t mem_size) {
    const std::size_t max_mem_alloc_size =
        q.get_device().get_info<sycl::info::device::max_mem_alloc_size>();
    if (mem_size > max_mem_alloc_size) {
        throw std::invalid_argument{ "mem_size cannot be greater than "
                                     "max_mem_alloc_size which is " +
                                     format_memory(max_mem_alloc_size) };
    }

    if (mem_size % sizeof(float) > 0) {
        throw std::invalid_argument{ "mem_size must be a multiple of " +
                                     std::to_string(sizeof(float)) };
    }

    const std::size_t count = mem_size / sizeof(float);
    const auto data = be::make_unique_usm_device<float>(q, std::int64_t(count));
    float* data_ptr = data.get();

    const double read_bandwidth = measure_bandwidth(mem_size, [&]() {
        return q.parallel_for(sycl::nd_range<1>(count, 256), [=](sycl::nd_item<1> item) {
            const size_t i = item.get_global_id();
            [[maybe_unused]] volatile float f = data_ptr[i];
        });
    });
    fmt::print("Read bandwidth: {}/s \n", format_memory(read_bandwidth));

    const double write_bandwidth = measure_bandwidth(mem_size, [&]() {
        return q.parallel_for(sycl::nd_range<1>(count, 256), [=](sycl::nd_item<1> item) {
            const size_t i = item.get_global_id();
            data_ptr[i] = 0.0f;
        });
    });
    fmt::print("Write bandwidth: {}/s \n", format_memory(write_bandwidth));

    const double rw_bandwidth = measure_bandwidth(2 * mem_size, [&]() {
        return q.parallel_for(sycl::nd_range<1>(count, 256), [=](sycl::nd_item<1> item) {
            const size_t i = item.get_global_id();
            data_ptr[i] = data_ptr[i] + 1;
        });
    });
    fmt::print("Read/Write bandwidth: {}/s \n", format_memory(rw_bandwidth));
}

int main(int argc, char const* argv[]) {
    auto queue = sycl::queue{ sycl::gpu_selector{}, { sycl::property::queue::enable_profiling{} } };
    const std::size_t allocation_size = 1 * (1ul << (10 * 3)); // 1Gb
    measure_global_memory_bandwidth_peak(queue, allocation_size);
    return 0;
}
