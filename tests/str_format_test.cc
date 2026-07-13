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

#include <xtext/str_format.h>

#include <cstdio>
#include <string>

#include <doctest.h>

TEST_CASE("StrFormat basics") {
    auto s = xtext::str_format("%s %d", "hello", 42);
    CHECK(s == "hello 42");
}

TEST_CASE("StrFormat float") {
    auto s = xtext::str_format("%.2f", 3.14159);
    CHECK(s == "3.14");
}

TEST_CASE("StrAppendFormat") {
    std::string s = "start";
    xtext::str_append_format(&s, " %d", 42);
    CHECK(s == "start 42");
}

TEST_CASE("SNPrintF") {
    char buf[16];
    int n = xtext::SNPrintF(buf, sizeof(buf), "%d", 12345);
    CHECK(n == 5);
    CHECK(std::string(buf) == "12345");
}

TEST_CASE("SNPrintF truncation") {
    char buf[4];
    int n = xtext::SNPrintF(buf, sizeof(buf), "%d", 12345);
    CHECK(n == 5);
    CHECK(std::string(buf) == "123");
}
