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
//
// -----------------------------------------------------------------------------
// File: ascii.h
// -----------------------------------------------------------------------------
//
// This package contains functions operating on characters and strings
// restricted to standard ASCII. These include character classification
// functions analogous to those found in the ANSI C Standard Library <ctype.h>
// header file.
//
// C++ implementations provide <ctype.h> functionality based on their
// C environment locale. In general, reliance on such a locale is not ideal, as
// the locale standard is problematic (and may not return invariant information
// for the same character set, for example). These `ascii_*()` functions are
// hard-wired for standard ASCII, much faster, and guaranteed to behave
// consistently.  They will never be overloaded, nor will their function
// signature change.
//
// `ascii_isalnum()`, `ascii_isalpha()`, `ascii_isascii()`, `ascii_isblank()`,
// `ascii_iscntrl()`, `ascii_isdigit()`, `ascii_isgraph()`, `ascii_islower()`,
// `ascii_isprint()`, `ascii_ispunct()`, `ascii_isspace()`, `ascii_isupper()`,
// `ascii_isxdigit()`
//   Analogous to the <ctype.h> functions with similar names, these
//   functions take an unsigned char and return a bool, based on whether the
//   character matches the condition specified.
//
//   If the input character has a numerical value greater than 127, these
//   functions return `false`.
//
// `ascii_tolower()`, `ascii_toupper()`
//   Analogous to the <ctype.h> functions with similar names, these functions
//   take an unsigned char and return a char.
//
//   If the input character is not an ASCII {lower,upper}-case letter (including
//   numerical values greater than 127) then the functions return the same value
//   as the input character.

#pragma once

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>

namespace xtext {
    namespace ascii_internal {

        extern const unsigned char kPropertyBits[256];

        extern const char kToUpper[256];

        extern const char kToLower[256];

    }  // namespace ascii_internal

    inline bool ascii_isalpha(unsigned char c) {
        return (ascii_internal::kPropertyBits[c] & 0x01) != 0;
    }

    inline bool ascii_isalnum(unsigned char c) {
        return (ascii_internal::kPropertyBits[c] & 0x04) != 0;
    }

    inline bool ascii_isspace(unsigned char c) {
        return (ascii_internal::kPropertyBits[c] & 0x08) != 0;
    }

    inline bool ascii_ispunct(unsigned char c) {
        return (ascii_internal::kPropertyBits[c] & 0x10) != 0;
    }

    inline bool ascii_isblank(unsigned char c) {
        return (ascii_internal::kPropertyBits[c] & 0x20) != 0;
    }

    inline bool ascii_iscntrl(unsigned char c) {
        return (ascii_internal::kPropertyBits[c] & 0x40) != 0;
    }

    inline bool ascii_isxdigit(unsigned char c) {
        return (ascii_internal::kPropertyBits[c] & 0x80) != 0;
    }

    inline bool ascii_isdigit(unsigned char c) { return c >= '0' && c <= '9'; }

    inline bool ascii_isprint(unsigned char c) { return c >= 32 && c < 127; }

    inline bool ascii_isgraph(unsigned char c) { return c > 32 && c < 127; }

    inline bool ascii_isupper(unsigned char c) { return c >= 'A' && c <= 'Z'; }

    inline bool ascii_islower(unsigned char c) { return c >= 'a' && c <= 'z'; }

    inline bool ascii_isascii(unsigned char c) { return c < 128; }

    inline char ascii_tolower(unsigned char c) {
        return ascii_internal::kToLower[c];
    }

    void str_to_lower(std::string * s);

    inline std::string str_to_lower(std::string_view s) {
        std::string result(s);
        xtext::str_to_lower(&result);
        return result;
    }

    inline char ascii_toupper(unsigned char c) {
        return ascii_internal::kToUpper[c];
    }

    void str_to_upper(std::string * s);

    inline std::string str_to_upper(std::string_view s) {
        std::string result(s);
        xtext::str_to_upper(&result);
        return result;
    }



}  // namespace xtext
