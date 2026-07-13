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

            Arg(short value) : piece_(ConvertSigned(value)) {}
            Arg(unsigned short value) : piece_(ConvertUnsigned(value)) {}
            Arg(int value) : piece_(ConvertSigned(value)) {}
            Arg(unsigned int value) : piece_(ConvertUnsigned(value)) {}
            Arg(long value) : piece_(ConvertSigned(value)) {}
            Arg(unsigned long value) : piece_(ConvertUnsigned(value)) {}
            Arg(long long value) : piece_(ConvertSigned(value)) {}
            Arg(unsigned long long value) : piece_(ConvertUnsigned(value)) {}

            Arg(float value) : piece_(ConvertFloat(value)) {}
            Arg(double value) : piece_(ConvertFloat(value)) {}

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

            Arg(void *value) : piece_(ConvertPtr(value)) {}
            Arg(const void *value) : piece_(ConvertPtr(value)) {}
            Arg(std::nullptr_t) : piece_("NULL") {}

            std::string_view Piece() const { return piece_; }

        private:
            std::string_view ConvertSigned(int64_t value);
            std::string_view ConvertUnsigned(uint64_t value);
            std::string_view ConvertFloat(double value);
            std::string_view ConvertPtr(const void *value);

            std::string_view piece_;
            char scratch_[32];
        };

        void SubstituteAndAppendArray(
                std::string *output, std::string_view format,
                const std::string_view *args_array, size_t num_args);

    }  // namespace substitute_internal

    inline void substitute_and_append(
            std::string *output, std::string_view format) {
        substitute_internal::SubstituteAndAppendArray(
                output, format, nullptr, 0);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0) {
        std::string_view pieces[] = {a0.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 1);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 2);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece(), a2.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 3);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece(), a2.Piece(), a3.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 4);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece(), a2.Piece(), a3.Piece(), a4.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 5);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece(), a2.Piece(), a3.Piece(), a4.Piece(), a5.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 6);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5,
            const substitute_internal::Arg &a6) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece(), a2.Piece(), a3.Piece(), a4.Piece(), a5.Piece(), a6.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 7);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5,
            const substitute_internal::Arg &a6,
            const substitute_internal::Arg &a7) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece(), a2.Piece(), a3.Piece(), a4.Piece(), a5.Piece(), a6.Piece(), a7.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 8);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5,
            const substitute_internal::Arg &a6,
            const substitute_internal::Arg &a7,
            const substitute_internal::Arg &a8) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece(), a2.Piece(), a3.Piece(), a4.Piece(), a5.Piece(), a6.Piece(), a7.Piece(), a8.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 9);
    }

    inline void substitute_and_append(
            std::string *output, std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5,
            const substitute_internal::Arg &a6,
            const substitute_internal::Arg &a7,
            const substitute_internal::Arg &a8,
            const substitute_internal::Arg &a9) {
        std::string_view pieces[] = {a0.Piece(), a1.Piece(), a2.Piece(), a3.Piece(), a4.Piece(), a5.Piece(), a6.Piece(), a7.Piece(), a8.Piece(), a9.Piece()};
        substitute_internal::SubstituteAndAppendArray(
                output, format, pieces, 10);
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format) {
        std::string result;
        substitute_and_append(&result, format);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0) {
        std::string result;
        substitute_and_append(&result, format, a0);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1) {
        std::string result;
        substitute_and_append(&result, format, a0, a1);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2) {
        std::string result;
        substitute_and_append(&result, format, a0, a1, a2);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3) {
        std::string result;
        substitute_and_append(&result, format, a0, a1, a2, a3);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4) {
        std::string result;
        substitute_and_append(&result, format, a0, a1, a2, a3, a4);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5) {
        std::string result;
        substitute_and_append(&result, format, a0, a1, a2, a3, a4, a5);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5,
            const substitute_internal::Arg &a6) {
        std::string result;
        substitute_and_append(&result, format, a0, a1, a2, a3, a4, a5, a6);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5,
            const substitute_internal::Arg &a6,
            const substitute_internal::Arg &a7) {
        std::string result;
        substitute_and_append(&result, format, a0, a1, a2, a3, a4, a5, a6, a7);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5,
            const substitute_internal::Arg &a6,
            const substitute_internal::Arg &a7,
            const substitute_internal::Arg &a8) {
        std::string result;
        substitute_and_append(&result, format, a0, a1, a2, a3, a4, a5, a6, a7, a8);
        return result;
    }

    XTEXT_MUST_USE_RESULT inline std::string substitute(
            std::string_view format,
            const substitute_internal::Arg &a0,
            const substitute_internal::Arg &a1,
            const substitute_internal::Arg &a2,
            const substitute_internal::Arg &a3,
            const substitute_internal::Arg &a4,
            const substitute_internal::Arg &a5,
            const substitute_internal::Arg &a6,
            const substitute_internal::Arg &a7,
            const substitute_internal::Arg &a8,
            const substitute_internal::Arg &a9) {
        std::string result;
        substitute_and_append(
                &result, format, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
        return result;
    }

}  // namespace xtext
