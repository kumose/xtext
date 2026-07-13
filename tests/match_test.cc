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

#include <xtext/match.h>

#include <string>
#include <string_view>

#include <doctest.h>

TEST_CASE("MatchTest starts_with") {
  const std::string s1("123\0abc", 7);
  const std::string_view a("foobar");
  const std::string_view b(s1);
  const std::string_view e;
  CHECK(xtext::starts_with(a, a));
  CHECK(xtext::starts_with(a, "foo"));
  CHECK(xtext::starts_with(a, e));
  CHECK(xtext::starts_with(b, s1));
  CHECK(xtext::starts_with(b, b));
  CHECK(xtext::starts_with(b, e));
  CHECK(xtext::starts_with(e, ""));
  CHECK_FALSE(xtext::starts_with(a, b));
  CHECK_FALSE(xtext::starts_with(b, a));
  CHECK_FALSE(xtext::starts_with(e, a));
}

TEST_CASE("MatchTest ends_with") {
  const std::string s1("123\0abc", 7);
  const std::string_view a("foobar");
  const std::string_view b(s1);
  const std::string_view e;
  CHECK(xtext::ends_with(a, a));
  CHECK(xtext::ends_with(a, "bar"));
  CHECK(xtext::ends_with(a, e));
  CHECK(xtext::ends_with(b, s1));
  CHECK(xtext::ends_with(b, b));
  CHECK(xtext::ends_with(b, e));
  CHECK(xtext::ends_with(e, ""));
  CHECK_FALSE(xtext::ends_with(a, b));
  CHECK_FALSE(xtext::ends_with(b, a));
  CHECK_FALSE(xtext::ends_with(e, a));
}

TEST_CASE("MatchTest Contains") {
  std::string_view a("abcdefg");
  std::string_view b("abcd");
  std::string_view c("efg");
  std::string_view d("gh");
  CHECK(xtext::str_contains(a, a));
  CHECK(xtext::str_contains(a, b));
  CHECK(xtext::str_contains(a, c));
  CHECK_FALSE(xtext::str_contains(a, d));
  CHECK(xtext::str_contains("", ""));
  CHECK(xtext::str_contains("abc", ""));
  CHECK_FALSE(xtext::str_contains("", "a"));
}

TEST_CASE("MatchTest ContainsChar") {
  std::string_view a("abcdefg");
  std::string_view b("abcd");
  CHECK(xtext::str_contains(a, 'a'));
  CHECK(xtext::str_contains(a, 'b'));
  CHECK(xtext::str_contains(a, 'e'));
  CHECK_FALSE(xtext::str_contains(a, 'h'));

  CHECK(xtext::str_contains(b, 'a'));
  CHECK(xtext::str_contains(b, 'b'));
  CHECK_FALSE(xtext::str_contains(b, 'e'));
  CHECK_FALSE(xtext::str_contains(b, 'h'));

  CHECK_FALSE(xtext::str_contains("", 'a'));
  CHECK_FALSE(xtext::str_contains("", 'a'));
}

TEST_CASE("MatchTest ContainsNull") {
  const std::string s = "foo";
  const char* cs = "foo";
  const std::string_view sv("foo");
  const std::string_view sv2("foo\0bar", 4);
  CHECK(s == "foo");
  CHECK(sv == "foo");
  CHECK(sv2 != "foo");
  CHECK(xtext::ends_with(s, sv));
  CHECK(xtext::starts_with(cs, sv));
  CHECK(xtext::str_contains(cs, sv));
  CHECK_FALSE(xtext::str_contains(cs, sv2));
}

TEST_CASE("MatchTest equals_ignore_case") {
  std::string text = "the";
  std::string_view data(text);

  CHECK(xtext::equals_ignore_case(data, "The"));
  CHECK(xtext::equals_ignore_case(data, "THE"));
  CHECK(xtext::equals_ignore_case(data, "the"));
  CHECK_FALSE(xtext::equals_ignore_case(data, "Quick"));
  CHECK_FALSE(xtext::equals_ignore_case(data, "then"));
}

TEST_CASE("MatchTest starts_with_ignore_case") {
  CHECK(xtext::starts_with_ignore_case("foo", "foo"));
  CHECK(xtext::starts_with_ignore_case("foo", "Fo"));
  CHECK(xtext::starts_with_ignore_case("foo", ""));
  CHECK_FALSE(xtext::starts_with_ignore_case("foo", "fooo"));
  CHECK_FALSE(xtext::starts_with_ignore_case("", "fo"));
}

TEST_CASE("MatchTest ends_with_ignore_case") {
  CHECK(xtext::ends_with_ignore_case("foo", "foo"));
  CHECK(xtext::ends_with_ignore_case("foo", "Oo"));
  CHECK(xtext::ends_with_ignore_case("foo", ""));
  CHECK_FALSE(xtext::ends_with_ignore_case("foo", "fooo"));
  CHECK_FALSE(xtext::ends_with_ignore_case("", "fo"));
}

TEST_CASE("MatchTest ContainsIgnoreCase") {
  CHECK(xtext::str_contains_ignore_case("foo", "foo"));
  CHECK(xtext::str_contains_ignore_case("FOO", "Foo"));
  CHECK(xtext::str_contains_ignore_case("--FOO", "Foo"));
  CHECK(xtext::str_contains_ignore_case("FOO--", "Foo"));
  CHECK_FALSE(xtext::str_contains_ignore_case("BAR", "Foo"));
  CHECK_FALSE(xtext::str_contains_ignore_case("BAR", "Foo"));
  CHECK(xtext::str_contains_ignore_case("123456", "123456"));
  CHECK(xtext::str_contains_ignore_case("123456", "234"));
  CHECK(xtext::str_contains_ignore_case("", ""));
  CHECK(xtext::str_contains_ignore_case("abc", ""));
  CHECK_FALSE(xtext::str_contains_ignore_case("", "a"));
}

TEST_CASE("MatchTest ContainsCharIgnoreCase") {
  std::string_view a("AaBCdefg!");
  std::string_view b("AaBCd!");
  CHECK(xtext::str_contains_ignore_case(a, 'a'));
  CHECK(xtext::str_contains_ignore_case(a, 'A'));
  CHECK(xtext::str_contains_ignore_case(a, 'b'));
  CHECK(xtext::str_contains_ignore_case(a, 'B'));
  CHECK(xtext::str_contains_ignore_case(a, 'e'));
  CHECK(xtext::str_contains_ignore_case(a, 'E'));
  CHECK_FALSE(xtext::str_contains_ignore_case(a, 'h'));
  CHECK_FALSE(xtext::str_contains_ignore_case(a, 'H'));
  CHECK(xtext::str_contains_ignore_case(a, '!'));
  CHECK_FALSE(xtext::str_contains_ignore_case(a, '?'));

  CHECK(xtext::str_contains_ignore_case(b, 'a'));
  CHECK(xtext::str_contains_ignore_case(b, 'A'));
  CHECK(xtext::str_contains_ignore_case(b, 'b'));
  CHECK(xtext::str_contains_ignore_case(b, 'B'));
  CHECK_FALSE(xtext::str_contains_ignore_case(b, 'e'));
  CHECK_FALSE(xtext::str_contains_ignore_case(b, 'E'));
  CHECK_FALSE(xtext::str_contains_ignore_case(b, 'h'));
  CHECK_FALSE(xtext::str_contains_ignore_case(b, 'H'));
  CHECK(xtext::str_contains_ignore_case(b, '!'));
  CHECK_FALSE(xtext::str_contains_ignore_case(b, '?'));

  CHECK_FALSE(xtext::str_contains_ignore_case("", 'a'));
  CHECK_FALSE(xtext::str_contains_ignore_case("", 'A'));
  CHECK_FALSE(xtext::str_contains_ignore_case("", '0'));
}
