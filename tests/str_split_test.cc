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

#include <xtext/str_split.h>

#include <string>
#include <vector>

#include <doctest.h>

TEST_CASE("StrSplit ByChar") {
    std::vector<std::string> v;
    for (auto s : xtext::str_split("a,b,c", ',')) {
        v.push_back(std::string(s));
    }
    REQUIRE(v.size() == 3);
    CHECK(v[0] == "a");
    CHECK(v[1] == "b");
    CHECK(v[2] == "c");
}

TEST_CASE("StrSplit ByString") {
    std::vector<std::string> v;
    for (auto s : xtext::str_split("a-->b-->c", "-->")) {
        v.push_back(std::string(s));
    }
    REQUIRE(v.size() == 3);
    CHECK(v[0] == "a");
    CHECK(v[1] == "b");
    CHECK(v[2] == "c");
}

TEST_CASE("StrSplit ByAnyChar") {
    std::vector<std::string> v;
    for (auto s : xtext::str_split("a,b;c", xtext::ByAnyChar(",;"))) {
        v.push_back(std::string(s));
    }
    REQUIRE(v.size() == 3);
    CHECK(v[0] == "a");
    CHECK(v[1] == "b");
    CHECK(v[2] == "c");
}

TEST_CASE("StrSplit SkipEmpty") {
    std::vector<std::string> v;
    for (auto s : xtext::str_split("a,,b,,c", ',', xtext::SkipEmpty())) {
        v.push_back(std::string(s));
    }
    REQUIRE(v.size() == 3);
    CHECK(v[0] == "a");
    CHECK(v[1] == "b");
    CHECK(v[2] == "c");
}

TEST_CASE("StrSplit AllowEmpty") {
    std::vector<std::string> v;
    for (auto s : xtext::str_split("a,,b", ',', xtext::AllowEmpty())) {
        v.push_back(std::string(s));
    }
    REQUIRE(v.size() == 3);
    CHECK(v[0] == "a");
    CHECK(v[1] == "");
    CHECK(v[2] == "b");
}
