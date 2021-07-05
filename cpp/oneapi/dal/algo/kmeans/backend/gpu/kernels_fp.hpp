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

#pragma once

#include "oneapi/dal/backend/common.hpp"
#include "oneapi/dal/backend/primitives/ndarray.hpp"
#include "oneapi/dal/backend/primitives/distance.hpp"

namespace oneapi::dal::kmeans::backend {

#ifdef ONEDAL_DATA_PARALLEL

namespace bk = dal::backend;
namespace pr = dal::backend::primitives;

template <typename Float>
struct kernels_fp {
    static std::int64_t get_block_size_in_rows(sycl::queue& queue, std::int64_t column_count);

    static std::int64_t get_part_count_for_partial_centroids(sycl::queue& queue,
                                                             std::int64_t column_count,
                                                             std::int64_t cluster_count);
    static sycl::event select(sycl::queue& queue,
                              const pr::ndview<Float, 2>& data,
                              pr::ndview<Float, 2>& selection,
                              pr::ndview<std::int32_t, 2>& indices,
                              const bk::event_vector& deps = {});

    template <typename Metric>
    static sycl::event assign_clusters(sycl::queue& queue,
                                       const pr::ndview<Float, 2>& data,
                                       const pr::ndview<Float, 2>& centroids,
                                       std::int64_t block_size_in_rows,
                                       pr::ndview<std::int32_t, 2>& labels,
                                       pr::ndview<Float, 2>& distances,
                                       pr::ndview<Float, 2>& closest_distances,
                                       const bk::event_vector& deps = {});

    static sycl::event fill_empty_clusters(
        sycl::queue& queue,
        const pr::ndview<Float, 2>& data,
        const pr::ndarray<std::int32_t, 1>& candidate_indices,
        const pr::ndarray<std::int32_t, 1>& empty_cluster_indices,
        pr::ndview<Float, 2>& centroids,
        const bk::event_vector& deps = {});

    static sycl::event find_candidates(sycl::queue& queue,
                                       std::int64_t candidate_count,
                                       const pr::ndview<Float, 2>& closest_distances,
                                       pr::ndview<std::int32_t, 1>& candidate_indices,
                                       pr::ndview<Float, 1>& candidate_distances,
                                       const bk::event_vector& deps = {});

    static sycl::event merge_reduce_centroids(sycl::queue& queue,
                                              const pr::ndview<std::int32_t, 1>& counters,
                                              const pr::ndview<Float, 2>& partial_centroids,
                                              std::int64_t part_count,
                                              pr::ndview<Float, 2>& centroids,
                                              const bk::event_vector& deps = {});

    static sycl::event partial_reduce_centroids(sycl::queue& queue,
                                                const pr::ndview<Float, 2>& data,
                                                const pr::ndview<std::int32_t, 2>& labels,
                                                std::int64_t cluster_count,
                                                std::int64_t part_count,
                                                pr::ndview<Float, 2>& partial_centroids,
                                                const bk::event_vector& deps = {});

    static sycl::event compute_objective_function(sycl::queue& queue,
                                                  const pr::ndview<Float, 2>& closest_distances,
                                                  pr::ndview<Float, 1>& objective_function,
                                                  const bk::event_vector& deps = {});

    static sycl::event gather_candidates(sycl::queue& queue,
                                         std::int64_t candidate_count,
                                         const pr::ndview<Float, 2>& data,
                                         const pr::ndview<std::int32_t, 1>& candidate_indices,
                                         pr::ndview<Float, 2>& candidates,
                                         const bk::event_vector& deps = {});

    static sycl::event scatter_candidates(sycl::queue& queue,
                                          const pr::ndview<std::int32_t, 1>& empty_cluster_indices,
                                          const pr::ndview<Float, 2>& candidates,
                                          pr::ndview<Float, 2>& centroids,
                                          const bk::event_vector& deps = {});
};
#endif

} // namespace oneapi::dal::kmeans::backend
