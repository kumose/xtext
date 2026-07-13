// Copyright (C) 2026 Kumo inc. and its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <xtext/str_cat.h>

#include <cstddef>
#include <cstdint>
#include <climits>
#include <string>
#include <vector>

#include <doctest.h>

TEST_CASE("StrCat Basics") {
    std::string result;

    result = xtext::str_cat(std::string("hi"), 123, std::string_view(" world"));
    CHECK(result == "hi123 world");

    result = xtext::str_cat();
    CHECK(result == "");
}

TEST_CASE("StrCat Numbers") {
    std::string result;

    result = xtext::str_cat(42);
    CHECK(result == "42");

    result = xtext::str_cat(-42);
    CHECK(result == "-42");

    result = xtext::str_cat(0);
    CHECK(result == "0");

    result = xtext::str_cat(std::numeric_limits<int64_t>::min());
    CHECK(result == "-9223372036854775808");

    result = xtext::str_cat(std::numeric_limits<uint64_t>::max());
    CHECK(result == "18446744073709551615");
}

TEST_CASE("StrCat Float") {
    std::string result;
    result = xtext::str_cat(3.14);
    CHECK_FALSE(result.empty());

    result = xtext::str_cat(0.0f);
    CHECK_FALSE(result.empty());
}

TEST_CASE("StrCat Bool") {
    CHECK(xtext::str_cat(true) == "1");
    CHECK(xtext::str_cat(false) == "0");
}

TEST_CASE("StrCat Pointer") {
    // nullptr produces empty string
    const char *null_str = nullptr;
    CHECK(xtext::str_cat(null_str) == "");
}

TEST_CASE("StrCat Hex") {
    auto result = xtext::str_cat(xtext::Hex(255));
    CHECK(result == "ff");

    result = xtext::str_cat(xtext::Hex(255, xtext::kZeroPad8));
    CHECK(result == "000000ff");

    result = xtext::str_cat(xtext::Hex(0));
    CHECK(result == "0");
}

TEST_CASE("StrCat Dec") {
    auto result = xtext::str_cat(xtext::Dec(42));
    CHECK(result == "42");

    result = xtext::str_cat(xtext::Dec(-42));
    CHECK(result == "-42");

    result = xtext::str_cat(xtext::Dec(42, xtext::kZeroPad5));
    CHECK(result == "00042");

    result = xtext::str_cat(xtext::Dec(-42, xtext::kZeroPad5));
    CHECK(result == "-0042");

    result = xtext::str_cat(xtext::Dec(42, xtext::kSpacePad5));
    CHECK(result == "   42");
}

TEST_CASE("StrCat Variadic") {
    const std::string s = "hello";
    const std::string_view sv = " world";
    auto result = xtext::str_cat(s, sv, 42, "!");
    CHECK(result == "hello world42!");
}

TEST_CASE("StrAppend Basics") {
    std::string result = "start";
    xtext::str_append(&result, 123, " end");
    CHECK(result == "start123 end");
}

TEST_CASE("StrAppend Multiple") {
    std::string result;
    xtext::str_append(&result, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j");
    CHECK(result == "abcdefghij");
}

TEST_CASE("StrAppend Empty") {
    std::string result = "x";
    xtext::str_append(&result);
    CHECK(result == "x");
}

TEST_CASE("StrAppend Hex") {
    std::string result;
    xtext::str_append(&result, xtext::Hex(255, xtext::kZeroPad4));
    CHECK(result == "00ff");
}

TEST_CASE("StrAppend Dec") {
    std::string result;
    xtext::str_append(&result, xtext::Dec(-100, xtext::kSpacePad6));
    CHECK(result == "  -100");
}
