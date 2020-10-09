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

#pragma once

#include "oneapi/dal/test/common.hpp"
#include "oneapi/dal/table/common.hpp"
#include "oneapi/dal/table/row_accessor.hpp"
#include "oneapi/dal/backend/linalg/matrix.hpp"

namespace oneapi::dal::test {

class matcher {
public:
    virtual ~matcher() = default;
    virtual bool match(const table& actual) const = 0;
    virtual bool match(const dal::backend::linalg::matrix<float>& actual) const = 0;
    virtual bool match(const dal::backend::linalg::matrix<double>& actual) const = 0;
};

template <typename ElementMatcher>
class elementwise_matcher : public matcher {
public:
    elementwise_matcher() = default;

    explicit elementwise_matcher(const ElementMatcher& element_matcher)
            : element_matcher_(element_matcher) {}

protected:
    ElementMatcher& get_element_matcher() {
        return element_matcher_;
    }

    const ElementMatcher& get_element_matcher() const {
        return element_matcher_;
    }

private:
    ElementMatcher element_matcher_;
};

template <typename ComparisonType, typename ElementMatcher>
class array_like_matcher : public elementwise_matcher<ElementMatcher> {
private:
    using super = elementwise_matcher<ElementMatcher>;

public:
    template <typename T>
    explicit array_like_matcher(const array<T>& expected)
            : expected_(convert_array(expected)),
              row_count_(-1),
              column_count_(-1),
              element_count_(expected.get_count()) {}

    template <typename Float>
    explicit array_like_matcher(const dal::backend::linalg::matrix<Float>& expected)
            : expected_(convert_array(expected.get_array())),
              row_count_(expected.get_row_count()),
              column_count_(expected.get_column_count()),
              element_count_(expected.get_count()) {}

    explicit array_like_matcher(const table& expected)
            : expected_(row_accessor<const ComparisonType>{expected}.pull()),
              row_count_(expected.get_row_count()),
              column_count_(expected.get_column_count()),
              element_count_(expected.get_row_count() * expected.get_column_count()) {}


    bool match(const table& actual) const override {
        const auto actual_array = row_accessor<const ComparisonType>{actual}.pull();
        return match_tabular(actual_array.get_data(), actual.get_row_count(), actual.get_column_count());
    }

    bool match(const dal::backend::linalg::matrix<float>& actual) const override {
        return match_matrix<float>(actual);
    }

    bool match(const dal::backend::linalg::matrix<double>& actual) const override {
        return match_matrix<double>(actual);
    }

    template <typename T>
    bool match_matrix(const dal::backend::linalg::matrix<T>& actual) const {
        const auto actual_array = convert_array(actual.get_array());
        return match_tabular(actual_array.get_data(), actual.get_row_count(), actual.get_column_count());
    }

private:
    bool match_tabular(const ComparisonType* actual_data,
                       std::int64_t actual_row_count,
                       std::int64_t actual_column_count) const {
        if (expected_is_tabular()) {
            if (actual_row_count != row_count_) {
                UNSCOPED_INFO(fmt::format("Unexpected row count in LHS expression: {}, expected {}",
                              actual_row_count, row_count_));
                return false;
            }

            if (actual_column_count != column_count_) {
                UNSCOPED_INFO(fmt::format("Unexpected column count in LHS expression: {}, expected {}",
                              actual_column_count, column_count_));
                return false;
            }

            ONEDAL_ASSERT(row_count_ * column_count_ == element_count_);
        }
        else {
            const std::int64_t actual_element_count = actual_row_count * actual_column_count;
            if (actual_element_count != element_count_) {
                UNSCOPED_INFO(fmt::format("Unexpected element count in LHS expression: {}, expected {}",
                              actual_element_count, element_count_));
                return false;
            }
        }

        const auto& element_matcher = super::get_element_matcher();
        for (std::int64_t t = 0; t < element_count_; t++) {
            if (actual_data[t] != expected_[t]) {
                const std::int64_t i = t / actual_row_count;
                const std::int64_t j = t % actual_column_count;
                if (!element_matcher(i, j, actual_data[t], expected_[t])) {
                    return false;
                }
            }
        }

        return true;
    }

    bool expected_is_tabular() const {
        return row_count_ >= 0 && column_count_ >= 0;
    }

    template <typename Source>
    static array<ComparisonType> convert_array(const array<Source>& source) {
        if constexpr (std::is_same_v<ComparisonType, Source>) {
            return source;
        }
        else {
            const auto dest = array<ComparisonType>::empty(source.get_count());
            ComparisonType* dest_data = dest.get_mutable_data();
            for (std::int64_t i = 0; i < dest.get_count(); i++) {
                dest_data[i] = ComparisonType(source[i]);
            }
            return dest;
        }
    }

    array<ComparisonType> expected_;
    std::int64_t row_count_;
    std::int64_t column_count_;
    std::int64_t element_count_;
};

template <typename Data>
struct exact_element_matcher {
    bool operator()(std::int64_t i, std::int64_t j, Data actual, Data expected) const {
        const bool is_match = (actual == expected);
        if (!is_match) {
            UNSCOPED_INFO(fmt::format("Mismatch detected at [{}, {}]: got {}, but expected {}",
                          i, j, actual, expected));
        }
        return is_match;
    }
};

template <typename Float>
struct approx_element_matcher {
    explicit approx_element_matcher(double epsilon = 1e-10)
            : epsilon_(epsilon) {}

    bool operator()(std::int64_t i, std::int64_t j, Float actual, Float expected) const {
        const double diff = std::abs(double(actual) - double(expected));
        const bool is_match = diff < epsilon_;
        if (!is_match) {
            UNSCOPED_INFO(fmt::format("Mismatch detected at [{}, {}]: |{} - {}| = {} > {}",
                          i, j, actual, expected, diff, epsilon_));
        }
        return is_match;
    }

    double epsilon_;
};

template <typename ComparisonType>
using array_like_exact_matcher = array_like_matcher<ComparisonType, exact_element_matcher<ComparisonType>>;

template <typename ComparionType>
class array_like_approx_matcher : public array_like_matcher<ComparionType, approx_element_matcher<ComparionType>> {
private:
    using super = array_like_matcher<ComparionType, approx_element_matcher<ComparionType>>;

public:
    template <typename T>
    explicit array_like_approx_matcher(T&& expected, double epsilon)
            : super(std::forward<T>(expected)) {
        super::get_element_matcher().epsilon_ = epsilon;
    }
};

template <typename ComparisonType>
using all_exact = array_like_exact_matcher<ComparisonType>;

template <typename ComparisonType>
using all_close = array_like_approx_matcher<ComparisonType>;

bool operator==(const table& actual, const matcher& m) {
    return m.match(actual);
}

template <typename Float>
bool operator==(const dal::backend::linalg::matrix<Float>& actual, const matcher& m) {
    return m.match(actual);
}

} // namespace oneapi::dal::test
