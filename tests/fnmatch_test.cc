// Copyright (C) 2026 Kumo Inc. and its affiliates. All Rights Reserved.
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

#include <xtext/fnmatch.h>

#include <doctest.h>

TEST_CASE("FNMatch Works") {
  using xtext::fn_match;
  CHECK(fn_match("foo", "foo"));
  CHECK_FALSE(fn_match("foo", "bar"));
  CHECK_FALSE(fn_match("foo", "fo"));
  CHECK_FALSE(fn_match("foo", "foo2"));
  CHECK(fn_match("bar/foo.ext", "bar/foo.ext"));
  CHECK(fn_match("*ba*r/fo*o.ext*", "bar/foo.ext"));
  CHECK_FALSE(fn_match("bar/foo.ext", "bar/baz.ext"));
  CHECK_FALSE(fn_match("bar/foo.ext", "bar/foo"));
  CHECK_FALSE(fn_match("bar/foo.ext", "bar/foo.ext.zip"));
  CHECK(fn_match("ba?/*.ext", "bar/foo.ext"));
  CHECK(fn_match("ba?/*.ext", "baZ/FOO.ext"));
  CHECK_FALSE(fn_match("ba?/*.ext", "barr/foo.ext"));
  CHECK_FALSE(fn_match("ba?/*.ext", "bar/foo.ext2"));
  CHECK(fn_match("ba?/*", "bar/foo.ext2"));
  CHECK(fn_match("ba?/*", "bar/"));
  CHECK_FALSE(fn_match("ba?/?", "bar/"));
  CHECK_FALSE(fn_match("ba?/*", "bar"));
  CHECK(fn_match("?x", "zx"));
  CHECK(fn_match("*b", "aab"));
  CHECK(fn_match("a*b", "aXb"));
  CHECK(fn_match("", ""));
  CHECK_FALSE(fn_match("", "a"));
  CHECK(fn_match("ab*", "ab"));
  CHECK(fn_match("ab**", "ab"));
  CHECK_FALSE(fn_match("ab*?", "ab"));
  CHECK(fn_match("*", "bbb"));
  CHECK(fn_match("*", ""));
  CHECK_FALSE(fn_match("?", ""));
  CHECK(fn_match("***", "**p"));
  CHECK(fn_match("**", "*"));
  CHECK(fn_match("*?", "*"));
}
