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

#include <xtext/trim.h>
#include <xtext/str_split.h>

#include <string>

#include <doctest.h>

TEST_CASE("TrimLeft char") {
    CHECK(xtext::trim_left("__hello", '_') == "hello");
    CHECK(xtext::trim_left("hello", 'x') == "hello");
    CHECK(xtext::trim_left("___", '_') == "");
}

TEST_CASE("TrimLeft string_view") {
    CHECK(xtext::trim_left(".,hello", ".,") == "hello");
    CHECK(xtext::trim_left(".,hello", "!") == ".,hello");
}

TEST_CASE("TrimRight char") {
    CHECK(xtext::trim_right("hello__", '_') == "hello");
    CHECK(xtext::trim_right("hello", 'x') == "hello");
    CHECK(xtext::trim_right("___", '_') == "");
}

TEST_CASE("TrimRight string_view") {
    CHECK(xtext::trim_right("hello.,", ".,") == "hello");
}

TEST_CASE("TrimAll char") {
    CHECK(xtext::trim_all("__hello__", '_') == "hello");
    CHECK(xtext::trim_all("hello", '_') == "hello");
    CHECK(xtext::trim_all("_", '_') == "");
}

TEST_CASE("TrimAll string_view") {
    CHECK(xtext::trim_all(".,hello;!", ".,;!") == "hello");
    CHECK(xtext::trim_all(".,hello;!", ".") == ",hello;!");
}

TEST_CASE("TrimAll predicate") {
    CHECK(xtext::trim_all("12hello34",
                          [](char c) { return c >= '0' && c <= '9'; })
          == "hello");
}

TEST_CASE("TrimAll ByChar") {
    CHECK(xtext::trim_all("__hello__", xtext::ByChar('_')) == "hello");
}

TEST_CASE("TrimAll ByAnyChar") {
    CHECK(xtext::trim_all(".,hello;!", xtext::ByAnyChar(".,;!")) == "hello");
}

TEST_CASE("Trim string* mutate") {
    std::string s = "__hello__";
    xtext::trim_all(&s, '_');
    CHECK(s == "hello");
}

TEST_CASE("Trim predicate AsciiIsSpace") {
    CHECK(xtext::trim_all("  hello  ", xtext::AsciiIsSpace{}) == "hello");
    CHECK(xtext::trim_left("  hello", xtext::AsciiIsSpace{}) == "hello");
    CHECK(xtext::trim_right("hello  ", xtext::AsciiIsSpace{}) == "hello");
}
