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
// File: str_format.h
// -----------------------------------------------------------------------------
//
// The `str_format` library is a typesafe replacement for the family of
// `printf()` string formatting routines within the `<cstdio>` standard library
// header. Like the `printf` family, `str_format` uses a "format string" to
// perform argument substitutions based on types.
//
// Example:
//
//   std::string s = xtext::str_format("%s %s You have $%d!", "Hello", name, dollars);
//
// The library consists of the following basic utilities:
//
//   * `xtext::str_format()`, a type-safe replacement for `sprintf()`, to
//     write a format string to a `string` value.
//   * `xtext::str_append_format()` to append a format string to a `string`.
//   * `xtext::PrintF()`, `xtext::FPrintF()` and `xtext::SNPrintF()` as
//     drop-in replacements for `printf()`, `fprintf()` and `snprintf()`.

#pragma once

#include <cstddef>
#include <cstdio>
#include <string>
#include <string_view>
#include <type_traits>

#include <xtext/fmt/printf.h>
#include <xtext/macros.h>

namespace xtext {

    // str_format()
    //
    // Returns a formatted string using printf-style format specifiers.
    //
    // Example:
    //   std::string s = xtext::str_format("%s %d", "hello", 42);
    template <typename... Args>
    XTEXT_MUST_USE_RESULT inline std::string str_format(
            std::string_view fmt, const Args &... args) {
        return fmt::sprintf(fmt, args...);
    }

    // str_append_format()
    //
    // Appends a formatted string to an existing string.
    //
    // Example:
    //   std::string s = "result: ";
    //   xtext::str_append_format(&s, "%d", 42);
    template <typename... Args>
    inline std::string &str_append_format(
            std::string *dst, std::string_view fmt, const Args &... args) {
        dst->append(fmt::sprintf(fmt, args...));
        return *dst;
    }

    // PrintF()
    //
    // Prints a formatted string to stdout.
    //
    // Example:
    //   xtext::PrintF("The answer is %d\n", 42);
    template <typename... Args>
    inline int PrintF(std::string_view fmt, const Args &... args) {
        return fmt::printf(fmt, args...);
    }

    // FPrintF()
    //
    // Prints a formatted string to a file.
    //
    // Example:
    //   xtext::FPrintF(stdout, "The answer is %d\n", 42);
    template <typename... Args>
    inline int FPrintF(std::FILE *output, std::string_view fmt,
                       const Args &... args) {
        return fmt::fprintf(output, fmt, args...);
    }

    // SNPrintF()
    //
    // Writes a formatted string to a buffer, truncating if necessary.
    // Returns the number of characters that would have been written if the
    // buffer were large enough (excluding the null terminator).
    //
    // Example:
    //   char buf[64];
    //   int n = xtext::SNPrintF(buf, sizeof(buf), "%d", 42);
    template <typename... Args>
    inline int SNPrintF(char *output, std::size_t size,
                        std::string_view fmt, const Args &... args) {
        auto s = fmt::sprintf(fmt, args...);
        auto written = s.copy(output, size - 1);
        output[written] = '\0';
        return static_cast<int>(s.size());
    }

}  // namespace xtext
