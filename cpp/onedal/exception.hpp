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

#include <new>
#include <system_error>
#include <stdexcept>
#include <memory>
#include <cstring>

namespace dal {

class exception{
public:
    virtual const char* what() const noexcept = 0;
};

class logic_error : public exception {};
class runtime_error : public exception {};

class invalid_argument :  public logic_error, public std::invalid_argument {
public:
    invalid_argument(const invalid_argument& other) noexcept : invalid_argument(other.what()) {}
    invalid_argument(const char* what_arg) : std::invalid_argument(what_arg) {}
    const char* what() const noexcept;
};

class domain_error :  public logic_error, public std::domain_error {
public:
    domain_error(const domain_error& other) noexcept : domain_error(other.what()) {}
    domain_error(const char* what_arg) : std::domain_error(what_arg) {}
    const char* what() const noexcept;
};

class out_of_range :  public logic_error, public std::out_of_range {
public:
    out_of_range(const out_of_range& other) noexcept : out_of_range(other.what()) {}
    out_of_range(const char* what_arg) : std::out_of_range(what_arg) {}
    const char* what() const noexcept;
};

class unimplemented_error :  public logic_error, public std::logic_error {
public:
    unimplemented_error(const unimplemented_error& other) noexcept : unimplemented_error(other.what()) {}
    unimplemented_error(const char* what_arg) : std::logic_error(what_arg) {}
    const char* what() const noexcept;
};

class unavailable_error :  public logic_error, public std::logic_error {
public:
    unavailable_error(const unavailable_error& other) noexcept : unavailable_error(other.what()) {}
    unavailable_error(const char* what_arg) : std::logic_error(what_arg) {}
    const char* what() const noexcept;
};

class range_error :  public runtime_error, public std::range_error {
public:
    range_error(const range_error& other) noexcept : range_error(other.what()) {}
    range_error(const char* what_arg) : std::range_error(what_arg) {}
    const char* what() const noexcept;
};

class system_error :  public runtime_error, public std::system_error {
public:
    system_error(const system_error& other) noexcept : system_error(other.code(), other.what()) {}
    system_error(std::error_code ec, const char* what_arg) : std::system_error(ec, what_arg) {}
    const char* what() const noexcept;
    const std::error_code& code() const noexcept;
};

class internal_error :  public runtime_error, public std::runtime_error {
public:
    internal_error(const internal_error& other) noexcept : internal_error(other.what()) {}
    internal_error(const char* what_arg) : std::runtime_error(what_arg) {}
    const char* what() const noexcept;
};

class bad_alloc : public exception, public std::bad_alloc {
protected:
std::unique_ptr<char[]> str;

public:
    bad_alloc(const bad_alloc& other) noexcept : bad_alloc(other.what()) {}
    bad_alloc(const char* what_arg);
    const char* what() const noexcept;
};



} // namespace dal
