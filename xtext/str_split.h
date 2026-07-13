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
// File: str_split.h
// -----------------------------------------------------------------------------
//
// This file contains functions for splitting strings. It defines the main
// `str_split()` function, several delimiters for determining the boundaries on
// which to split the string, and predicates for filtering delimited results.
// `str_split()` adapts the returned collection to the type specified by the
// caller.
//
// Example:
//
//   // Splits the given string on commas. Returns the results in a
//   // vector of strings.
//   std::vector<std::string> v = xtext::str_split("a,b,c", ',');
//   // Can also use ","
//   // v[0] == "a", v[1] == "b", v[2] == "c"
//
// See str_split() below for more information.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <xtext/details/str_split_internal.h>
#include <xtext/macros.h>
#include <xtext/strip.h>
#include <xtext/trim.h>

namespace xtext {

    //------------------------------------------------------------------------------
    // Delimiters
    //------------------------------------------------------------------------------
    //
    // `str_split()` uses delimiters to define the boundaries between elements in the
    // provided input. Several `Delimiter` types are defined below. If a string
    // (`const char*`, `std::string`, or `std::string_view`) is passed in place of
    // an explicit `Delimiter` object, `str_split()` treats it the same way as if it
    // were passed a `ByString` delimiter.
    //
    // A `Delimiter` is an object with a `Find()` function that knows how to find
    // the first occurrence of itself in a given `std::string_view`.
    //
    // The following `Delimiter` types are available for use within `str_split()`:
    //
    //   - `ByString` (default for string arguments)
    //   - `ByChar` (default for a char argument)
    //   - `ByAnyChar`
    //   - `ByLength`
    //   - `MaxSplits`

    // ByString
    //
    // A sub-string delimiter. If `str_split()` is passed a string in place of a
    // `Delimiter` object, the string will be implicitly converted into a
    // `ByString` delimiter.
    //
    // Example:
    //
    //   // Because a string literal is converted to an `xtext::ByString`,
    //   // the following two splits are equivalent.
    //
    //   std::vector<std::string> v1 = xtext::str_split("a, b, c", ", ");
    //
    //   using xtext::ByString;
    //   std::vector<std::string> v2 = xtext::str_split("a, b, c",
    //                                                ByString(", "));
    //   // v[0] == "a", v[1] == "b", v[2] == "c"
    class ByString {
    public:
        explicit ByString(std::string_view sp);
        std::string_view Find(std::string_view text, size_t pos) const;

    private:
        std::string delimiter_;
    };

    // ByAsciiWhitespace
    //
    // A sub-string delimiter that splits by ASCII whitespace
    // (space, tab, vertical tab, formfeed, linefeed, or carriage return).
    // Note: you probably want to use xtext::SkipEmpty() as well!
    //
    // This class is equivalent to ByAnyChar with ASCII whitespace chars.
    //
    // Example:
    //
    //   std::vector<std::string> v = xtext::str_split(
    //       "a b\tc\n  d  \n", xtext::ByAsciiWhitespace(), xtext::SkipEmpty());
    //   // v[0] == "a", v[1] == "b", v[2] == "c", v[3] == "d"
    class ByAsciiWhitespace {
    public:
        std::string_view Find(std::string_view text, size_t pos) const;
    };

    // ByChar
    //
    // A single character delimiter. `ByChar` is functionally equivalent to a
    // 1-char string within a `ByString` delimiter, but slightly more efficient.
    //
    // Example:
    //
    //   // Because a char literal is converted to a xtext::ByChar,
    //   // the following two splits are equivalent.
    //   std::vector<std::string> v1 = xtext::str_split("a,b,c", ',');
    //   using xtext::ByChar;
    //   std::vector<std::string> v2 = xtext::str_split("a,b,c", ByChar(','));
    //   // v[0] == "a", v[1] == "b", v[2] == "c"
    class ByChar {
    public:
        explicit ByChar(char c) : c_(c) {}
        std::string_view Find(std::string_view text, size_t pos) const;
        bool operator()(char c) const { return c == c_; }

    private:
        char c_;
    };

    // ByAnyChar
    //
    // A delimiter that will match any of the given byte-sized characters within
    // its provided string.
    //
    // Note: this delimiter works with single-byte string data, but does not work
    // with variable-width encodings, such as UTF-8.
    //
    // Example:
    //
    //   using xtext::ByAnyChar;
    //   std::vector<std::string> v = xtext::str_split("a,b=c", ByAnyChar(",="));
    //   // v[0] == "a", v[1] == "b", v[2] == "c"
    //
    // If `ByAnyChar` is given the empty string, it behaves exactly like
    // `ByString` and matches each individual character in the input string.
    class ByAnyChar {
    public:
        explicit ByAnyChar(std::string_view sp);
        std::string_view Find(std::string_view text, size_t pos) const;
        bool operator()(char c) const {
            return delimiters_.find(c) != std::string::npos;
        }

    private:
        std::string delimiters_;
    };

    // ByLength
    //
    // A delimiter for splitting into equal-length strings. The length argument to
    // the constructor must be greater than 0.
    //
    // Note: this delimiter works with single-byte string data, but does not work
    // with variable-width encodings, such as UTF-8.
    //
    // Example:
    //
    //   using xtext::ByLength;
    //   std::vector<std::string> v = xtext::str_split("123456789", ByLength(3));
    //   // v[0] == "123", v[1] == "456", v[2] == "789"
    class ByLength {
    public:
        explicit ByLength(ptrdiff_t length);
        std::string_view Find(std::string_view text, size_t pos) const;

    private:
        ptrdiff_t length_;
    };

    namespace strings_internal {

        // SelectDelimiter -- maps string-like types to ByString, char to ByChar.
        template <typename Delimiter>
        struct SelectDelimiter {
            using type = Delimiter;
        };

        template <>
        struct SelectDelimiter<char> {
            using type = ByChar;
        };

        template <>
        struct SelectDelimiter<char *> {
            using type = ByString;
        };

        template <>
        struct SelectDelimiter<const char *> {
            using type = ByString;
        };

        template <>
        struct SelectDelimiter<std::string_view> {
            using type = ByString;
        };

        template <>
        struct SelectDelimiter<std::string> {
            using type = ByString;
        };

        // MaxSplitsImpl wraps a delimiter and limits the number of matches.
        template <typename Delimiter>
        class MaxSplitsImpl {
        public:
            MaxSplitsImpl(Delimiter delimiter, int limit)
                    : delimiter_(delimiter), limit_(limit), count_(0) {}
            std::string_view Find(std::string_view text, size_t pos) {
                if (count_++ == limit_) {
                    return std::string_view(text.data() + text.size(), 0);
                }
                return delimiter_.Find(text, pos);
            }

        private:
            Delimiter delimiter_;
            int limit_;
            int count_;
        };

    }  // namespace strings_internal

    // MaxSplits()
    //
    // A delimiter that limits the number of matches which can occur to the passed
    // `limit`. The last element in the returned collection will contain all
    // remaining unsplit pieces, which may contain instances of the delimiter.
    // The collection will contain at most `limit` + 1 elements.
    //
    // Example:
    //
    //   using xtext::MaxSplits;
    //   std::vector<std::string> v = xtext::str_split("a,b,c", MaxSplits(',', 1));
    //   // v[0] == "a", v[1] == "b,c"
    template <typename Delimiter>
    inline strings_internal::MaxSplitsImpl<
            typename strings_internal::SelectDelimiter<Delimiter>::type>
    MaxSplits(Delimiter delimiter, int limit) {
        typedef typename strings_internal::SelectDelimiter<Delimiter>::type
                DelimiterType;
        return strings_internal::MaxSplitsImpl<DelimiterType>(
                DelimiterType(delimiter), limit);
    }

    //------------------------------------------------------------------------------
    // Predicates
    //------------------------------------------------------------------------------
    //
    // Predicates filter the results of a `str_split()` by determining whether or not
    // a resultant element is included in the result set.

    // AllowEmpty()
    //
    // Always returns `true`, indicating that all strings--including empty
    // strings--should be included in the split output.
    struct AllowEmpty {
        bool operator()(std::string_view) const { return true; }
    };

    // SkipEmpty()
    //
    // Returns `false` if the given `std::string_view` is empty, indicating that
    // `str_split()` should omit the empty string.
    struct SkipEmpty {
        bool operator()(std::string_view sp) const { return !sp.empty(); }
    };

    // SkipWhitespace()
    //
    // Returns `false` if the given `std::string_view` is empty *or* contains only
    // whitespace, indicating that `str_split()` should omit the string.
    struct SkipWhitespace {
        bool operator()(std::string_view sp) const {
            sp = xtext::trim_all(sp);
            return !sp.empty();
        }
    };

    //------------------------------------------------------------------------------
    //                                  str_split()
    //------------------------------------------------------------------------------

    // str_split()
    //
    // Splits a given string based on the provided `Delimiter` object, returning the
    // elements within the type specified by the caller. Optionally, you may pass a
    // `Predicate` to `str_split()` indicating whether to include or exclude the
    // resulting element within the final result set.
    //
    // Example:
    //
    //   std::vector<std::string> v = xtext::str_split("a,b,c,d", ',');
    //   // v[0] == "a", v[1] == "b", v[2] == "c", v[3] == "d"
    //
    // You can also provide an explicit `Delimiter` object:
    //
    //   using xtext::ByAnyChar;
    //   std::vector<std::string> v = xtext::str_split("a,b=c", ByAnyChar(",="));
    //   // v[0] == "a", v[1] == "b", v[2] == "c"
    //
    // By default, empty strings are included in the result set. You can optionally
    // include a third `Predicate` argument to apply a test for whether the
    // resultant element should be included in the result set:
    //
    //   std::vector<std::string> v = xtext::str_split(" a , ,,b,",
    //                                               ',', SkipWhitespace());
    //   // v[0] == " a ", v[1] == "b"
    template <typename Delimiter>
    strings_internal::Splitter<
            typename strings_internal::SelectDelimiter<Delimiter>::type, AllowEmpty,
            std::string_view>
    str_split(strings_internal::ConvertibleToStringView text, Delimiter d) {
        using DelimiterType =
                typename strings_internal::SelectDelimiter<Delimiter>::type;
        return strings_internal::Splitter<DelimiterType, AllowEmpty,
                std::string_view>(
                text.value(), DelimiterType(d), AllowEmpty());
    }

    template <typename Delimiter, typename StringType,
              typename = typename std::enable_if<
                      std::is_same<StringType, std::string>::value ||
                      std::is_same<StringType, const std::string>::value>::type>
    strings_internal::Splitter<
            typename strings_internal::SelectDelimiter<Delimiter>::type, AllowEmpty,
            std::string>
    str_split(StringType &&text, Delimiter d) {
        using DelimiterType =
                typename strings_internal::SelectDelimiter<Delimiter>::type;
        return strings_internal::Splitter<DelimiterType, AllowEmpty, std::string>(
                std::move(text), DelimiterType(d), AllowEmpty());
    }

    template <typename Delimiter, typename Predicate>
    strings_internal::Splitter<
            typename strings_internal::SelectDelimiter<Delimiter>::type, Predicate,
            std::string_view>
    str_split(strings_internal::ConvertibleToStringView text, Delimiter d,
              Predicate p) {
        using DelimiterType =
                typename strings_internal::SelectDelimiter<Delimiter>::type;
        return strings_internal::Splitter<DelimiterType, Predicate,
                std::string_view>(
                text.value(), DelimiterType(std::move(d)), std::move(p));
    }

    template <typename Delimiter, typename Predicate, typename StringType,
              typename = typename std::enable_if<
                      std::is_same<StringType, std::string>::value ||
                      std::is_same<StringType, const std::string>::value>::type>
    strings_internal::Splitter<
            typename strings_internal::SelectDelimiter<Delimiter>::type, Predicate,
            std::string>
    str_split(StringType &&text, Delimiter d, Predicate p) {
        using DelimiterType =
                typename strings_internal::SelectDelimiter<Delimiter>::type;
        return strings_internal::Splitter<DelimiterType, Predicate, std::string>(
                std::move(text), DelimiterType(d), std::move(p));
    }

}  // namespace xtext
