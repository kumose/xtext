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
// File: substitute.h
// -----------------------------------------------------------------------------
//
// This package contains functions for efficiently performing string
// substitutions using a format string with positional notation:
// `substitute()` and `substitute_and_append()`.
//
// Unlike printf-style format specifiers, `substitute()` functions do not need
// to specify the type of the substitution arguments. Supported arguments
// following the format string, such as strings, string_views, ints,
// floats, and bools, are automatically converted to strings during the
// substitution process.
//
// `substitute()` does not allow you to specify *how* to format a value, beyond
// the default conversion to string.
//
// The format string uses positional identifiers indicated by a dollar sign ($)
// and single digit positional ids to indicate which substitution arguments to
// use at that location within the format string.
//
// A '$$' sequence in the format string causes a literal '$' character to be
// output.
//
// Example:
//   std::string s = xtext::substitute("$1 purchased $0 $2 for $$10. Thanks $1!",
//                                      5, "Bob", "Apples");
//   EXPECT_EQ("Bob purchased 5 Apples for $10. Thanks Bob!", s);
//
// Example:
//   std::string s = "Hi. ";
//   xtext::substitute_and_append(&s, "My name is $0 and I am $1 years old.",
//                                 "Bob", 5);
//   EXPECT_EQ("Hi. My name is Bob and I am 5 years old.", s);
//
// Supported types:
//   * std::string_view, std::string, const char* (null is equivalent to "")
//   * int32_t, int64_t, uint32_t, uint64_t
//   * float, double
//   * bool (Printed as "true" or "false")
//   * pointer types other than char* (Printed as "0x<lower case hex string>",
//     except that null is printed as "NULL")
//
// If an invalid format string is provided, substitute returns an empty string
// and substitute_and_append does not change the provided output string.
// A format string is invalid if it:
//   * ends in an unescaped $ character,
//     e.g. "Hello $", or
//   * calls for a position argument which is not provided,
//     e.g. xtext::substitute("Hello $2", "world"), or
//   * specifies a non-digit, non-$ character after an unescaped $ character,
//     e.g. "Hello $f".
// In debug mode, i.e. #ifndef NDEBUG, such errors terminate the program.

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include <xtext/macros.h>

namespace xtext {

    namespace substitute_internal {

        class Arg {
        public:
            Arg(const char *value)
                    : piece_(value ? std::string_view(value) : std::string_view()) {}

            template <typename Allocator>
            Arg(const std::basic_string<char, std::char_traits<char>, Allocator> &value)
                    : piece_(value) {}

            Arg(std::string_view value)
                    : piece_(value) {}

            Arg(char value) : piece_(scratch_, 1) { scratch_[0] = value; }

            Arg(short value) { piece_ = Convert(static_cast<int64_t>(value)); }
            Arg(unsigned short value) { piece_ = Convert(static_cast<uint64_t>(value)); }
            Arg(int value) { piece_ = Convert(static_cast<int64_t>(value)); }
            Arg(unsigned int value) { piece_ = Convert(static_cast<uint64_t>(value)); }
            Arg(long value) { piece_ = Convert(static_cast<int64_t>(value)); }
            Arg(unsigned long value) { piece_ = Convert(static_cast<uint64_t>(value)); }
            Arg(long long value) { piece_ = Convert(static_cast<int64_t>(value)); }
            Arg(unsigned long long value) { piece_ = Convert(static_cast<uint64_t>(value)); }

            Arg(float value) { piece_ = ConvertFloat(value); }
            Arg(double value) { piece_ = ConvertFloat(value); }

            Arg(bool value) : piece_(value ? "true" : "false") {}

            template <typename T,
                      typename = typename std::enable_if<
                              std::is_enum<T>{} && std::is_convertible<T, int>{}>::type>
            Arg(T e) : Arg(+e) {}

            template <typename T,
                      typename std::enable_if<std::is_enum<T>{} &&
                                              !std::is_convertible<T, int>{},
                              char *>::type = nullptr>
            Arg(T e) : Arg(+static_cast<typename std::underlying_type<T>::type>(e)) {}

            Arg(std::nullptr_t) : piece_("NULL") {}
            Arg(void *value);
            Arg(const void *value);

            std::string_view Piece() const { return piece_; }

        private:
            std::string_view Convert(uint64_t value);
            std::string_view Convert(int64_t value);
            std::string_view ConvertFloat(double value);
            std::string_view ConvertPtr(const void *value);

            std::string_view piece_;
            // scratch_ must be large enough for the largest formatted value:
            // - 64-bit integer: up to 20 digits
            // - float: "{:.6g}" produces up to ~15 chars
            // - pointer: "0x" + 16 hex digits = 18 chars
            char scratch_[32];
        };

        void SubstituteAndAppendArray(
                std::string *output, std::string_view format,
                const std::string_view *args_array, size_t num_args);

    }  // namespace substitute_internal

    // substitute_and_append()
    //
    // Appends a formatted string to `output`, using `format` and `args`.
    //
    // Example:
    //   std::string s = "Hi. ";
    //   xtext::substitute_and_append(&s, "My name is $0 and I am $1 years old.",
    //                                 "Bob", 5);
    //   EXPECT_EQ("Hi. My name is Bob and I am 5 years old.", s);
    template <typename... Args>
    inline void substitute_and_append(std::string *output,
                                      std::string_view format,
                                      const Args &... args) {
        const substitute_internal::Arg arg_array[] = {args...};
        std::string_view pieces[sizeof...(args)];
        for (size_t i = 0; i < sizeof...(args); ++i) {
            pieces[i] = arg_array[i].Piece();
        }
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, sizeof...(args));
    }

    // substitute()
    //
    // Returns a formatted string, using `format` and `args`.
    //
    // Example:
    //   std::string s = xtext::substitute("$1 purchased $0 $2 for $$10. Thanks $1!",
    //                                      5, "Bob", "Apples");
    //   EXPECT_EQ("Bob purchased 5 Apples for $10. Thanks Bob!", s);
    template <typename... Args>
    inline std::string substitute(std::string_view format,
                                  const Args &... args) {
        std::string result;
        substitute_and_append(&result, format, args...);
        return result;
    }

}  // namespace xtext
