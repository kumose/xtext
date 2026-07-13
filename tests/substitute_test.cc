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

#include <xtext/substitute.h>

#include <string>
#include <string_view>

#include <doctest.h>

TEST_CASE("Substitute basic") {
    auto s = xtext::substitute("$1 purchased $0 $2 for $$10. Thanks $1!",
                               5, "Bob", "Apples");
    CHECK(s == "Bob purchased 5 Apples for $10. Thanks Bob!");
}

TEST_CASE("Substitute single") {
    CHECK(xtext::substitute("Hello $0", "world") == "Hello world");
}

TEST_CASE("Substitute multiple") {
    CHECK(xtext::substitute("$0 $1 $2 $3", "a", "b", "c", "d") == "a b c d");
}

TEST_CASE("Substitute no args") {
    CHECK(xtext::substitute("hello") == "hello");
}

TEST_CASE("SubstituteAndAppend") {
    std::string s = "Hi. ";
    xtext::substitute_and_append(&s, "My name is $0 and I am $1 years old.",
                                 "Bob", 5);
    CHECK(s == "Hi. My name is Bob and I am 5 years old.");
}

TEST_CASE("Substitute numbers") {
    CHECK(xtext::substitute("$0", 42) == "42");
    CHECK(xtext::substitute("$0", -42) == "-42");
    CHECK(xtext::substitute("$0", 3.14f) != "");
    CHECK(xtext::substitute("$0", 3.14) != "");
}

TEST_CASE("Substitute bool") {
    CHECK(xtext::substitute("$0", true) == "true");
    CHECK(xtext::substitute("$0", false) == "false");
}

TEST_CASE("Substitute pointer") {
    int x = 0;
    auto s = xtext::substitute("$0", &x);
    CHECK(s.size() > 2);
    CHECK(s.substr(0, 2) == "0x");

    CHECK(xtext::substitute("$0", nullptr) == "NULL");
}

TEST_CASE("Substitute char") {
    auto result = xtext::substitute("$0", 'A');
    CHECK(result == "A");
}
