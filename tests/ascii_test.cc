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

#include <xtext/ascii.h>
#include <xtext/trim.h>

#include <algorithm>
#include <cctype>
#include <clocale>
#include <cstring>
#include <string>

#include <doctest.h>

TEST_CASE("AsciiIsFoo All") {
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
      CHECK(xtext::ascii_isalpha(c));
    else
      CHECK(!xtext::ascii_isalpha(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if ((c >= '0' && c <= '9'))
      CHECK(xtext::ascii_isdigit(c));
    else
      CHECK(!xtext::ascii_isdigit(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (xtext::ascii_isalpha(c) || xtext::ascii_isdigit(c))
      CHECK(xtext::ascii_isalnum(c));
    else
      CHECK(!xtext::ascii_isalnum(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (i != '\0' && strchr(" \r\n\t\v\f", i))
      CHECK(xtext::ascii_isspace(c));
    else
      CHECK(!xtext::ascii_isspace(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (i >= 32 && i < 127)
      CHECK(xtext::ascii_isprint(c));
    else
      CHECK(!xtext::ascii_isprint(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (xtext::ascii_isprint(c) && !xtext::ascii_isspace(c) &&
        !xtext::ascii_isalnum(c)) {
      CHECK(xtext::ascii_ispunct(c));
    } else {
      CHECK(!xtext::ascii_ispunct(c));
    }
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (i == ' ' || i == '\t')
      CHECK(xtext::ascii_isblank(c));
    else
      CHECK(!xtext::ascii_isblank(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (i < 32 || i == 127)
      CHECK(xtext::ascii_iscntrl(c));
    else
      CHECK(!xtext::ascii_iscntrl(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (xtext::ascii_isdigit(c) || (i >= 'A' && i <= 'F') ||
        (i >= 'a' && i <= 'f')) {
      CHECK(xtext::ascii_isxdigit(c));
    } else {
      CHECK(!xtext::ascii_isxdigit(c));
    }
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (i > 32 && i < 127)
      CHECK(xtext::ascii_isgraph(c));
    else
      CHECK(!xtext::ascii_isgraph(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (i >= 'A' && i <= 'Z')
      CHECK(xtext::ascii_isupper(c));
    else
      CHECK(!xtext::ascii_isupper(c));
  }
  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (i >= 'a' && i <= 'z')
      CHECK(xtext::ascii_islower(c));
    else
      CHECK(!xtext::ascii_islower(c));
  }
  for (unsigned char c = 0; c < 128; c++) {
    CHECK(xtext::ascii_isascii(c));
  }
  for (int i = 128; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    CHECK(!xtext::ascii_isascii(c));
  }
}

TEST_CASE("AsciiIsFoo SameAsIsFoo") {
#ifndef __ANDROID__
  const char* old_locale = setlocale(LC_CTYPE, "C");
  CHECK(old_locale != nullptr);
#endif

  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    CHECK((isalpha(c) != 0) == xtext::ascii_isalpha(c));
    CHECK((isdigit(c) != 0) == xtext::ascii_isdigit(c));
    CHECK((isalnum(c) != 0) == xtext::ascii_isalnum(c));
    CHECK((isspace(c) != 0) == xtext::ascii_isspace(c));
    CHECK((ispunct(c) != 0) == xtext::ascii_ispunct(c));
    CHECK((isblank(c) != 0) == xtext::ascii_isblank(c));
    CHECK((iscntrl(c) != 0) == xtext::ascii_iscntrl(c));
    CHECK((isxdigit(c) != 0) == xtext::ascii_isxdigit(c));
    CHECK((isprint(c) != 0) == xtext::ascii_isprint(c));
    CHECK((isgraph(c) != 0) == xtext::ascii_isgraph(c));
    CHECK((isupper(c) != 0) == xtext::ascii_isupper(c));
    CHECK((islower(c) != 0) == xtext::ascii_islower(c));
    CHECK((isascii(c) != 0) == xtext::ascii_isascii(c));
  }

#ifndef __ANDROID__
  CHECK(setlocale(LC_CTYPE, old_locale));
#endif
}

TEST_CASE("AsciiToFoo All") {
#ifndef __ANDROID__
  const char* old_locale = setlocale(LC_CTYPE, "C");
  CHECK(old_locale != nullptr);
#endif

  for (int i = 0; i < 256; i++) {
    const auto c = static_cast<unsigned char>(i);
    if (xtext::ascii_islower(c))
      CHECK(xtext::ascii_toupper(c) == 'A' + (i - 'a'));
    else
      CHECK(xtext::ascii_toupper(c) == static_cast<char>(i));

    if (xtext::ascii_isupper(c))
      CHECK(xtext::ascii_tolower(c) == 'a' + (i - 'A'));
    else
      CHECK(xtext::ascii_tolower(c) == static_cast<char>(i));

    CHECK(static_cast<char>(tolower(i)) == xtext::ascii_tolower(c));
    CHECK(static_cast<char>(toupper(i)) == xtext::ascii_toupper(c));
  }
#ifndef __ANDROID__
  CHECK(setlocale(LC_CTYPE, old_locale));
#endif
}

TEST_CASE("AsciiStrTo Lower") {
  const char buf[] = "ABCDEF";
  const std::string str("GHIJKL");
  const std::string str2("MNOPQR");
  const std::string_view sp(str2);
  const std::string long_str("ABCDEFGHIJKLMNOPQRSTUVWXYZ1!a");
  std::string mutable_str("_`?@[{AMNOPQRSTUVWXYZ");

  CHECK(xtext::str_to_lower(buf) == "abcdef");
  CHECK(xtext::str_to_lower(str) == "ghijkl");
  CHECK(xtext::str_to_lower(sp) == "mnopqr");
  CHECK(xtext::str_to_lower(long_str) == "abcdefghijklmnopqrstuvwxyz1!a");

  xtext::str_to_lower(&mutable_str);
  CHECK(mutable_str == "_`?@[{amnopqrstuvwxyz");

  char mutable_buf[] = "Mutable";
  std::transform(mutable_buf, mutable_buf + strlen(mutable_buf),
                 mutable_buf, xtext::ascii_tolower);
  CHECK(strcmp("mutable", mutable_buf) == 0);
}

TEST_CASE("AsciiStrTo Upper") {
  const char buf[] = "abcdef";
  const std::string str("ghijkl");
  const std::string str2("_`?@[{amnopqrstuvwxyz");
  const std::string_view sp(str2);
  const std::string long_str("abcdefghijklmnopqrstuvwxyz1!A");

  CHECK(xtext::str_to_upper(buf) == "ABCDEF");
  CHECK(xtext::str_to_upper(str) == "GHIJKL");
  CHECK(xtext::str_to_upper(sp) == "_`?@[{AMNOPQRSTUVWXYZ");
  CHECK(xtext::str_to_upper(long_str) == "ABCDEFGHIJKLMNOPQRSTUVWXYZ1!A");

  char mutable_buf[] = "Mutable";
  std::transform(mutable_buf, mutable_buf + strlen(mutable_buf),
                 mutable_buf, xtext::ascii_toupper);
  CHECK(strcmp("MUTABLE", mutable_buf) == 0);
}

TEST_CASE("trim_left FromStringView") {
  CHECK(xtext::trim_left(std::string_view{}) == std::string_view{});
  CHECK(xtext::trim_left({"foo"}) == "foo");
  CHECK(xtext::trim_left({"\t  \n\f\r\n\vfoo"}) == "foo");
  CHECK(xtext::trim_left({"\t  \n\f\r\n\vfoo foo\n "}) == "foo foo\n ");
  CHECK(xtext::trim_left({"\t  \n\f\r\v\n\t  \n\f\r\v\n"}) == std::string_view{});
}

TEST_CASE("trim_left InPlace") {
  std::string str;

  xtext::trim_left(&str);
  CHECK(str == "");

  str = "foo";
  xtext::trim_left(&str);
  CHECK(str == "foo");

  str = "\t  \n\f\r\n\vfoo";
  xtext::trim_left(&str);
  CHECK(str == "foo");

  str = "\t  \n\f\r\n\vfoo foo\n ";
  xtext::trim_left(&str);
  CHECK(str == "foo foo\n ");

  str = "\t  \n\f\r\v\n\t  \n\f\r\v\n";
  xtext::trim_left(&str);
  CHECK(str == std::string_view{});
}

TEST_CASE("trim_right FromStringView") {
  CHECK(xtext::trim_right(std::string_view{}) == std::string_view{});
  CHECK(xtext::trim_right({"foo"}) == "foo");
  CHECK(xtext::trim_right({"foo\t  \n\f\r\n\v"}) == "foo");
  CHECK(xtext::trim_right({" \nfoo foo\t  \n\f\r\n\v"}) == " \nfoo foo");
  CHECK(xtext::trim_right({"\t  \n\f\r\v\n\t  \n\f\r\v\n"}) == std::string_view{});
}

TEST_CASE("trim_right InPlace") {
  std::string str;

  xtext::trim_right(&str);
  CHECK(str == "");

  str = "foo";
  xtext::trim_right(&str);
  CHECK(str == "foo");

  str = "foo\t  \n\f\r\n\v";
  xtext::trim_right(&str);
  CHECK(str == "foo");

  str = " \nfoo foo\t  \n\f\r\n\v";
  xtext::trim_right(&str);
  CHECK(str == " \nfoo foo");

  str = "\t  \n\f\r\v\n\t  \n\f\r\v\n";
  xtext::trim_right(&str);
  CHECK(str == std::string_view{});
}

TEST_CASE("trim_all FromStringView") {
  CHECK(xtext::trim_all(std::string_view{}) == std::string_view{});
  CHECK(xtext::trim_all({"foo"}) == "foo");
  CHECK(xtext::trim_all({"\t  \n\f\r\n\vfoo\t  \n\f\r\n\v"}) == "foo");
  CHECK(xtext::trim_all({"\t  \n\f\r\n\vfoo foo\t  \n\f\r\n\v"}) == "foo foo");
  CHECK(xtext::trim_all({"\t  \n\f\r\v\n\t  \n\f\r\v\n"}) == std::string_view{});
}

TEST_CASE("trim_all InPlace") {
  std::string str;

  xtext::trim_all(&str);
  CHECK(str == "");

  str = "foo";
  xtext::trim_all(&str);
  CHECK(str == "foo");

  str = "\t  \n\f\r\n\vfoo\t  \n\f\r\n\v";
  xtext::trim_all(&str);
  CHECK(str == "foo");

  str = "\t  \n\f\r\n\vfoo foo\t  \n\f\r\n\v";
  xtext::trim_all(&str);
  CHECK(str == "foo foo");

  str = "\t  \n\f\r\v\n\t  \n\f\r\v\n";
  xtext::trim_all(&str);
  CHECK(str == std::string_view{});
}

TEST_CASE("trim_complete InPlace") {
  const char* inputs[] = {"No extra space",
                          "  Leading whitespace",
                          "Trailing whitespace  ",
                          "  Leading and trailing  ",
                          " Whitespace \t  in\v   middle  ",
                          "'Eeeeep!  \n Newlines!\n",
                          "nospaces",
                          "",
                          "\n\t a\t\n\nb \t\n"};

  const char* outputs[] = {
      "No extra space",
      "Leading whitespace",
      "Trailing whitespace",
      "Leading and trailing",
      "Whitespace in middle",
      "'Eeeeep! Newlines!",
      "nospaces",
      "",
      "a\nb",
  };
  const int NUM_TESTS = sizeof(inputs) / sizeof(inputs[0]);

  for (int i = 0; i < NUM_TESTS; i++) {
    std::string s(inputs[i]);
    xtext::trim_complete(&s);
    CHECK(outputs[i] == s);
  }
}
