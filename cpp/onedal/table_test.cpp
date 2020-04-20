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

#include "gtest/gtest.h"
#include "onedal/table.hpp"
#include "onedal/accessor.hpp"

using namespace dal;

TEST(table_test, can_construct_empty_table) {
    table t;

    ASSERT_TRUE(t.is_empty());
}

TEST(table_test, can_set_custom_implementation) {
    struct table_impl {
        std::int64_t get_column_count() const noexcept {
            return 10;
        }

        std::int64_t get_row_count() const noexcept {
            return 1000;
        }

        const table_metadata& get_metadata() const noexcept {
            return m;
        }

        void pull_rows(array<float>& a, const range& r) const { }
        void pull_rows(array<double>& a, const range& r) const { }
        void pull_rows(array<std::int32_t>& a, const range& r) const { }

        table_metadata m;
    };

    ASSERT_TRUE(is_table_impl_v<table_impl>);

    table t { table_impl{} };
    ASSERT_FALSE(t.is_empty());
}

TEST(homogen_table_test, can_construct_empty_table) {
    homogen_table t;

    ASSERT_TRUE(t.is_empty());
}

static float dummy_float_data_ptr[] = { 0.0f };
static double dummy_double_data_ptr[] = { 0.0 };
static std::int32_t dummy_int32_data_ptr[] = { 0 };

TEST(homogen_table_test, can_construct_rowmajor_table_3x2) {
    homogen_table t { 3, 2, dummy_float_data_ptr };

    ASSERT_FALSE(t.is_empty());
    EXPECT_EQ(3, t.get_row_count());
    EXPECT_EQ(2, t.get_column_count());

    EXPECT_EQ(data_layout::row_major, t.get_metadata().layout);

    auto features = t.get_metadata().features;

    EXPECT_EQ(2, features.get_size());
    for (std::int64_t i = 0; i < features.get_size(); i++) {
        EXPECT_EQ(data_type::float32, features[i].dtype);
        EXPECT_EQ(feature_type::contiguous, features[i].ftype);
    }
}

TEST(homogen_table_test, can_construct_rowmajor_int32_table) {
    homogen_table t { 1000, 10, dummy_int32_data_ptr };

    ASSERT_FALSE(t.is_empty());
    EXPECT_EQ(1000, t.get_row_count());
    EXPECT_EQ(10, t.get_column_count());

    EXPECT_EQ(data_layout::row_major, t.get_metadata().layout);

    auto features = t.get_metadata().features;

    EXPECT_EQ(10, features.get_size());
    for (std::int64_t i = 0; i < features.get_size(); i++) {
        EXPECT_EQ(data_type::int32, features[i].dtype);
        EXPECT_EQ(feature_type::nominal, features[i].ftype);
    }
}

TEST(homogen_table_test, can_construct_colmajor_float64_table) {
    homogen_table t { 1000, 10, dummy_double_data_ptr, data_layout::column_major };

    ASSERT_FALSE(t.is_empty());
    EXPECT_EQ(1000, t.get_row_count());
    EXPECT_EQ(10, t.get_column_count());

    EXPECT_EQ(data_layout::column_major, t.get_metadata().layout);

    auto features = t.get_metadata().features;

    EXPECT_EQ(10, features.get_size());
    for (std::int64_t i = 0; i < features.get_size(); i++) {
        EXPECT_EQ(data_type::float64, features[i].dtype);
        EXPECT_EQ(feature_type::contiguous, features[i].ftype);
    }
}

TEST(homogen_table_test, can_construct_table_reference) {
    homogen_table t1 { 3, 2, dummy_float_data_ptr };
    homogen_table t2 = t1;

    ASSERT_FALSE(t1.is_empty());
    ASSERT_FALSE(t2.is_empty());

    EXPECT_EQ(t1.get_row_count(), t2.get_row_count());
    EXPECT_EQ(t1.get_column_count(), t2.get_column_count());
    EXPECT_EQ(t1.get_data_pointer<float>(), t2.get_data_pointer<float>());

    const auto& m1 = t1.get_metadata();
    const auto& m2 = t2.get_metadata();

    EXPECT_EQ(m1.layout, m2.layout);
    EXPECT_EQ(m1.features.get_size(), m2.features.get_size());
    EXPECT_EQ(m1.features.get_pointer(), m2.features.get_pointer());
}

TEST(homogen_table_test, can_construct_table_with_move) {
    homogen_table t1 { 3, 2, dummy_float_data_ptr };
    homogen_table t2 = std::move(t1);

    ASSERT_TRUE(t1.is_empty());
    ASSERT_FALSE(t2.is_empty());

    EXPECT_EQ(3, t2.get_row_count());
    EXPECT_EQ(2, t2.get_column_count());

    const auto& m1 = t1.get_metadata();
    const auto& m2 = t2.get_metadata();

    EXPECT_EQ(data_layout::row_major, m2.layout);
    EXPECT_EQ(2, m2.features.get_size());
    EXPECT_EQ(data_type::float32, m2.features[0].dtype);
    EXPECT_EQ(data_type::float32, m2.features[1].dtype);
}

TEST(homogen_table_test, can_assign_two_table_references) {
    homogen_table t1 { 3, 2, dummy_float_data_ptr };
    homogen_table t2 { 4, 3, dummy_int32_data_ptr };

    t1 = t2;

    ASSERT_FALSE(t1.is_empty());
    ASSERT_FALSE(t2.is_empty());

    EXPECT_EQ(4, t1.get_row_count());
    EXPECT_EQ(3, t1.get_column_count());
    EXPECT_EQ(data_type::int32, t1.get_metadata().features[0].dtype);

    EXPECT_EQ(4, t2.get_row_count());
    EXPECT_EQ(3, t2.get_column_count());
    EXPECT_EQ(data_type::int32, t2.get_metadata().features[0].dtype);
}

TEST(homogen_table_test, can_move_assigned_table_reference) {
    homogen_table t1 { 3, 2, dummy_float_data_ptr };
    homogen_table t2 { 4, 3, dummy_int32_data_ptr };

    t1 = std::move(t2);

    ASSERT_FALSE(t1.is_empty());
    ASSERT_TRUE(t2.is_empty());

    EXPECT_EQ(4, t1.get_row_count());
    EXPECT_EQ(3, t1.get_column_count());
    EXPECT_EQ(data_type::int32, t1.get_metadata().features[0].dtype);
}

TEST(homogen_table_test, can_upcast_table) {
    table t = homogen_table { 3, 2, dummy_float_data_ptr };

    ASSERT_FALSE(t.is_empty());
    EXPECT_EQ(3, t.get_row_count());
    EXPECT_EQ(2, t.get_column_count());
    EXPECT_EQ(data_type::float32, t.get_metadata().features[0].dtype);
}

TEST(homogen_table_test, can_set_user_defined_memory) {
    double data[] = {
        1.0, 2.0, 3.0,
        -1.0, -2.0, -3.0
    };

    homogen_table t { 2, 3, data };

    ASSERT_FALSE(t.is_empty());
    EXPECT_EQ(2, t.get_row_count());
    EXPECT_EQ(3, t.get_column_count());

    const auto& meta = t.get_metadata();
    EXPECT_EQ(data_layout::row_major, meta.layout);
    EXPECT_EQ(t.get_column_count(), meta.features.get_size());

    for (std::int64_t i = 0; i < meta.features.get_size(); i++) {
        EXPECT_EQ(data_type::float64, meta.features[i].dtype);
        EXPECT_EQ(feature_type::contiguous, meta.features[i].ftype);
    }

    EXPECT_EQ(data, t.get_data_pointer<double>());
}

TEST(homogen_table_test, can_read_table_data_via_row_accessor) {
    double data[] = {
        1.0, 2.0, 3.0,
        -1.0, -2.0, -3.0
    };

    homogen_table t { 2, 3, data };
    row_accessor<const double> acc { t };
    array<const double> rows = acc.pull({0, -1});

    // array<double> rows;
    // acc.get_array({0, 1}, rows);
}

