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

// This file contains functions that remove a defined part from the string,
// i.e., strip the string.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <xtext/strip.h>
#include <xtext/ascii.h>
#include <xtext/trim.h>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>

#include <doctest.h>

TEST_CASE("Strip ConsumePrefixOneChar") {
  std::string_view input("abc");
  CHECK(xtext::consume_prefix(&input, "a"));
  CHECK(input == "bc");

  CHECK_FALSE(xtext::consume_prefix(&input, "x"));
  CHECK(input == "bc");

  CHECK(xtext::consume_prefix(&input, "b"));
  CHECK(input == "c");

  CHECK(xtext::consume_prefix(&input, "c"));
  CHECK(input == "");

  CHECK_FALSE(xtext::consume_prefix(&input, "a"));
  CHECK(input == "");
}

TEST_CASE("Strip consume_prefix") {
  std::string_view input("abcdef");
  CHECK_FALSE(xtext::consume_prefix(&input, "abcdefg"));
  CHECK(input == "abcdef");

  CHECK_FALSE(xtext::consume_prefix(&input, "abce"));
  CHECK(input == "abcdef");

  CHECK(xtext::consume_prefix(&input, ""));
  CHECK(input == "abcdef");

  CHECK_FALSE(xtext::consume_prefix(&input, "abcdeg"));
  CHECK(input == "abcdef");

  CHECK(xtext::consume_prefix(&input, "abcdef"));
  CHECK(input == "");

  input = "abcdef";
  CHECK(xtext::consume_prefix(&input, "abcde"));
  CHECK(input == "f");
}

TEST_CASE("Strip consume_suffix") {
  std::string_view input("abcdef");
  CHECK_FALSE(xtext::consume_suffix(&input, "abcdefg"));
  CHECK(input == "abcdef");

  CHECK(xtext::consume_suffix(&input, ""));
  CHECK(input == "abcdef");

  CHECK(xtext::consume_suffix(&input, "def"));
  CHECK(input == "abc");

  input = "abcdef";
  CHECK_FALSE(xtext::consume_suffix(&input, "abcdeg"));
  CHECK(input == "abcdef");

  CHECK(xtext::consume_suffix(&input, "f"));
  CHECK(input == "abcde");

  CHECK(xtext::consume_suffix(&input, "abcde"));
  CHECK(input == "");
}

TEST_CASE("Strip strip_prefix") {
  const std::string_view null_str;

  CHECK(xtext::strip_prefix("foobar", "foo") == "bar");
  CHECK(xtext::strip_prefix("foobar", "") == "foobar");
  CHECK(xtext::strip_prefix("foobar", null_str) == "foobar");
  CHECK(xtext::strip_prefix("foobar", "foobar") == "");
  CHECK(xtext::strip_prefix("foobar", "bar") == "foobar");
  CHECK(xtext::strip_prefix("foobar", "foobarr") == "foobar");
  CHECK(xtext::strip_prefix("", "") == "");
}

TEST_CASE("Strip strip_suffix") {
  const std::string_view null_str;

  CHECK(xtext::strip_suffix("foobar", "bar") == "foo");
  CHECK(xtext::strip_suffix("foobar", "") == "foobar");
  CHECK(xtext::strip_suffix("foobar", null_str) == "foobar");
  CHECK(xtext::strip_suffix("foobar", "foobar") == "");
  CHECK(xtext::strip_suffix("foobar", "foo") == "foobar");
  CHECK(xtext::strip_suffix("foobar", "ffoobar") == "foobar");
  CHECK(xtext::strip_suffix("", "") == "");
}

TEST_CASE("Strip trim_complete") {
  const char* inputs[] = {
      "No extra space",
      "  Leading whitespace",
      "Trailing whitespace  ",
      "  Leading and trailing  ",
      " Whitespace \t  in\v   middle  ",
      "'Eeeeep!  \n Newlines!\n",
      "nospaces",
  };
  const char* outputs[] = {
      "No extra space",
      "Leading whitespace",
      "Trailing whitespace",
      "Leading and trailing",
      "Whitespace in middle",
      "'Eeeeep! Newlines!",
      "nospaces",
  };
  int NUM_TESTS = 7;

  for (int i = 0; i < NUM_TESTS; i++) {
    std::string s(inputs[i]);
    xtext::trim_complete(&s);
    CHECK(outputs[i] == s);
  }

  std::string zero_string = "";
  assert(zero_string.empty());
  xtext::trim_complete(&zero_string);
  CHECK(zero_string.size() == 0);
  CHECK(zero_string.empty());
}

TEST_CASE("Strip trim_right") {
  std::string test = "foo  ";
  xtext::trim_right(&test);
  CHECK(test == "foo");

  test = "   ";
  xtext::trim_right(&test);
  CHECK(test == "");

  test = "";
  xtext::trim_right(&test);
  CHECK(test == "");

  test = " abc\t";
  xtext::trim_right(&test);
  CHECK(test == " abc");
}

TEST_CASE("String trim_left") {
  std::string_view orig = "\t  \n\f\r\n\vfoo";
  CHECK(xtext::trim_left(orig) == "foo");
  orig = "\t  \n\f\r\v\n\t  \n\f\r\v\n";
  CHECK(xtext::trim_left(orig) == std::string_view());
}

TEST_CASE("Strip trim_all") {
  std::string test2 = "\t  \f\r\n\vfoo \t\f\r\v\n";
  xtext::trim_all(&test2);
  CHECK(test2 == "foo");
  std::string test3 = "bar";
  xtext::trim_all(&test3);
  CHECK(test3 == "bar");
  std::string test4 = "\t  \f\r\n\vfoo";
  xtext::trim_all(&test4);
  CHECK(test4 == "foo");
  std::string test5 = "foo \t\f\r\v\n";
  xtext::trim_all(&test5);
  CHECK(test5 == "foo");
  std::string_view test6("\t  \f\r\n\vfoo \t\f\r\v\n");
  test6 = xtext::trim_all(test6);
  CHECK(test6 == "foo");
  test6 = xtext::trim_all(test6);
  CHECK(test6 == "foo");
}
