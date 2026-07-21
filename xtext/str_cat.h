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
// File: str_cat.h
// -----------------------------------------------------------------------------
//
// This package contains functions for efficiently concatenating and appending
// strings: `str_cat()` and `str_append()`. These routines accept `AlphaNum` as
// a parameter type, which efficiently converts bools, integers, floating-point
// values, and strings to string representations without extra copies.
//
// Any routine accepting either a string or a number may accept `AlphaNum`.
//
// NOTE: Use of `AlphaNum` outside of the xtext/strings package is unsupported
// except for the specific case of function parameters of type `AlphaNum` or
// `const AlphaNum &`. In particular, instantiating `AlphaNum` directly as a
// stack variable is not supported.
//
// Conversion from 8-bit values is not accepted because, if it were, then an
// attempt to pass ':' instead of ":" might result in a 58 ending up in your
// result.
//
// Bools convert to "0" or "1".
//
// Floating point numbers are formatted with six-digit precision ("%.6g").
//
// You can convert to hexadecimal output using the `Hex` type. To do so, pass
// `Hex(my_int)` as a parameter to `str_cat()` or `str_append()`.
//
// Example:
//   std::string s = xtext::str_cat("The answer is ", 42);
//   // s == "The answer is 42"

#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <fmt/format.h>
#include <xtext/macros.h>

namespace xtext {

    namespace strings_internal {

        template <size_t max_size>
        struct AlphaNumBuffer {
            std::array<char, max_size> data;
            size_t size;
        };

    }  // namespace strings_internal

    // PadSpec
    //
    // Enum that specifies the number of significant digits to return in a `Hex`
    // or `Dec` conversion and fill character to use.
    enum PadSpec : uint8_t {
        kNoPad = 1,
        kZeroPad2,
        kZeroPad3,
        kZeroPad4,
        kZeroPad5,
        kZeroPad6,
        kZeroPad7,
        kZeroPad8,
        kZeroPad9,
        kZeroPad10,
        kZeroPad11,
        kZeroPad12,
        kZeroPad13,
        kZeroPad14,
        kZeroPad15,
        kZeroPad16,
        kZeroPad17,
        kZeroPad18,
        kZeroPad19,
        kZeroPad20,

        kSpacePad2 = kZeroPad2 + 64,
        kSpacePad3,
        kSpacePad4,
        kSpacePad5,
        kSpacePad6,
        kSpacePad7,
        kSpacePad8,
        kSpacePad9,
        kSpacePad10,
        kSpacePad11,
        kSpacePad12,
        kSpacePad13,
        kSpacePad14,
        kSpacePad15,
        kSpacePad16,
        kSpacePad17,
        kSpacePad18,
        kSpacePad19,
        kSpacePad20,
    };

    // Hex
    //
    // Stores a set of hexadecimal string conversion parameters for use within
    // `AlphaNum` string conversions.
    struct Hex {
        uint64_t value;
        uint8_t width;
        char fill;

        template <typename Int>
        explicit Hex(
                Int v, PadSpec spec = xtext::kNoPad,
                typename std::enable_if<sizeof(Int) == 1 &&
                                        !std::is_pointer<Int>::value>::type * = nullptr)
                : Hex(spec, static_cast<uint8_t>(v)) {}

        template <typename Int>
        explicit Hex(
                Int v, PadSpec spec = xtext::kNoPad,
                typename std::enable_if<sizeof(Int) == 2 &&
                                        !std::is_pointer<Int>::value>::type * = nullptr)
                : Hex(spec, static_cast<uint16_t>(v)) {}

        template <typename Int>
        explicit Hex(
                Int v, PadSpec spec = xtext::kNoPad,
                typename std::enable_if<sizeof(Int) == 4 &&
                                        !std::is_pointer<Int>::value>::type * = nullptr)
                : Hex(spec, static_cast<uint32_t>(v)) {}

        template <typename Int>
        explicit Hex(
                Int v, PadSpec spec = xtext::kNoPad,
                typename std::enable_if<sizeof(Int) == 8 &&
                                        !std::is_pointer<Int>::value>::type * = nullptr)
                : Hex(spec, static_cast<uint64_t>(v)) {}

    private:
        Hex(PadSpec spec, uint64_t v)
                : value(v),
                  width(spec == xtext::kNoPad
                        ? 1
                        : spec >= xtext::kSpacePad2 ? spec - xtext::kSpacePad2 + 2
                                                    : spec - xtext::kZeroPad2 + 2),
                  fill(spec >= xtext::kSpacePad2 ? ' ' : '0') {}
    };

    // Dec
    //
    // Stores a set of decimal string conversion parameters for use within
    // `AlphaNum` string conversions. Dec is slower than the default integer
    // conversion, so use it only if you need padding.
    struct Dec {
        uint64_t value;
        uint8_t width;
        char fill;
        bool neg;

        template <typename Int>
        explicit Dec(Int v, PadSpec spec = xtext::kNoPad,
                     typename std::enable_if<(sizeof(Int) <= 8)>::type * = nullptr)
                : value(v >= 0 ? static_cast<uint64_t>(v)
                               : uint64_t{0} - static_cast<uint64_t>(v)),
                  width(spec == xtext::kNoPad ? 1
                                               : spec >= xtext::kSpacePad2 ? spec - xtext::kSpacePad2 + 2
                                                                           : spec - xtext::kZeroPad2 + 2),
                  fill(spec >= xtext::kSpacePad2 ? ' ' : '0'),
                  neg(v < 0) {}
    };

    // AlphaNum
    //
    // Acts as the main parameter type for `str_cat()` and `str_append()`,
    // providing efficient conversion of numeric, boolean, decimal, and
    // hexadecimal values (through the `Dec` and `Hex` types) into strings.
    // `AlphaNum` should only be used as a function parameter.
    class AlphaNum {
    public:
        template <typename T>
        AlphaNum(std::initializer_list<T>) = delete;

        AlphaNum(int x)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{}", x) - digits_)) {}

        AlphaNum(unsigned int x)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{}", x) - digits_)) {}

        AlphaNum(long x)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{}", x) - digits_)) {}

        AlphaNum(unsigned long x)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{}", x) - digits_)) {}

        AlphaNum(long long x)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{}", x) - digits_)) {}

        AlphaNum(unsigned long long x)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{}", x) - digits_)) {}

        AlphaNum(float f)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{:.6g}", f) - digits_)) {}

        AlphaNum(double d)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{:.6g}", d) - digits_)) {}

        template <size_t size>
        AlphaNum(const strings_internal::AlphaNumBuffer<size> &buf)
                : piece_(&buf.data[0], buf.size) {}

        AlphaNum(const char *c_str)
                : piece_(c_str ? std::string_view(c_str) : std::string_view()) {}

        AlphaNum(std::string_view pc)
                : piece_(pc) {}

        template <typename Allocator>
        AlphaNum(const std::basic_string<char, std::char_traits<char>, Allocator> &str)
                : piece_(str) {}

        AlphaNum(char c) = delete;

        AlphaNum(const AlphaNum &) = delete;
        AlphaNum &operator=(const AlphaNum &) = delete;

        std::string_view::size_type size() const { return piece_.size(); }
        const char *data() const { return piece_.data(); }
        std::string_view Piece() const { return piece_; }

        template <typename T,
                  typename = typename std::enable_if<
                          std::is_enum<T>{} && std::is_convertible<T, int>{}>::type>
        AlphaNum(T e)
                : AlphaNum(+e) {}

        template <typename T,
                  typename std::enable_if<std::is_enum<T>{} &&
                                          !std::is_convertible<T, int>{},
                          char *>::type = nullptr>
        AlphaNum(T e)
                : AlphaNum(+static_cast<typename std::underlying_type<T>::type>(e)) {}

        AlphaNum(Hex hex)
                : piece_(digits_, static_cast<size_t>(
                          fmt::format_to(digits_, "{:0{}x}", hex.value, hex.width) - digits_)) {}

        AlphaNum(Dec dec) {
            char *const end = &digits_[32];
            char *const minfill = end - dec.width;
            char *writer = end;
            uint64_t val = dec.value;
            while (val > 9) {
                *--writer = '0' + (val % 10);
                val /= 10;
            }
            *--writer = '0' + static_cast<char>(val);
            if (dec.neg) *--writer = '-';

            ptrdiff_t fillers = writer - minfill;
            if (fillers > 0) {
                bool add_sign_again = false;
                if (dec.neg && dec.fill == '0') {
                    ++writer;
                    add_sign_again = true;
                }
                writer -= fillers;
                std::fill_n(writer, fillers, dec.fill);
                if (add_sign_again) *--writer = '-';
            }
            piece_ = std::string_view(writer, static_cast<size_t>(end - writer));
        }

    private:
        std::string_view piece_;
        char digits_[32];
    };

    namespace strings_internal {

        std::string CatPieces(std::initializer_list<std::string_view> pieces);

        void AppendPieces(std::string *dest,
                          std::initializer_list<std::string_view> pieces);

    }  // namespace strings_internal

    // str_cat()
    //
    // Merges given strings or numbers, using no delimiter(s), returning the merged
    // result as a string.
    //
    // Example:
    //   std::string s = xtext::str_cat("The answer is ", 42);
    //   EXPECT_EQ("The answer is 42", s);
    inline std::string str_cat() { return {}; }

    inline std::string str_cat(const AlphaNum &a) {
        return std::string(a.Piece());
    }

    inline std::string str_cat(const AlphaNum &a, const AlphaNum &b) {
        return strings_internal::CatPieces({a.Piece(), b.Piece()});
    }

    inline std::string str_cat(const AlphaNum &a, const AlphaNum &b,
                               const AlphaNum &c) {
        return strings_internal::CatPieces({a.Piece(), b.Piece(), c.Piece()});
    }

    inline std::string str_cat(const AlphaNum &a, const AlphaNum &b,
                               const AlphaNum &c, const AlphaNum &d) {
        return strings_internal::CatPieces({a.Piece(), b.Piece(), c.Piece(), d.Piece()});
    }

    inline std::string str_cat(const AlphaNum &a, const AlphaNum &b,
                               const AlphaNum &c, const AlphaNum &d,
                               const AlphaNum &e) {
        return strings_internal::CatPieces(
                {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece()});
    }

    inline std::string str_cat(const AlphaNum &a, const AlphaNum &b,
                               const AlphaNum &c, const AlphaNum &d,
                               const AlphaNum &e, const AlphaNum &f) {
        return strings_internal::CatPieces(
                {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(), f.Piece()});
    }

    inline std::string str_cat(const AlphaNum &a, const AlphaNum &b,
                               const AlphaNum &c, const AlphaNum &d,
                               const AlphaNum &e, const AlphaNum &f,
                               const AlphaNum &g) {
        return strings_internal::CatPieces(
                {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(), f.Piece(), g.Piece()});
    }

    inline std::string str_cat(const AlphaNum &a, const AlphaNum &b,
                               const AlphaNum &c, const AlphaNum &d,
                               const AlphaNum &e, const AlphaNum &f,
                               const AlphaNum &g, const AlphaNum &h) {
        return strings_internal::CatPieces(
                {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
                 f.Piece(), g.Piece(), h.Piece()});
    }

    inline std::string str_cat(const AlphaNum &a, const AlphaNum &b,
                               const AlphaNum &c, const AlphaNum &d,
                               const AlphaNum &e, const AlphaNum &f,
                               const AlphaNum &g, const AlphaNum &h,
                               const AlphaNum &i) {
        return strings_internal::CatPieces(
                {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
                 f.Piece(), g.Piece(), h.Piece(), i.Piece()});
    }

    template <typename... AV>
    inline std::string str_cat(
            const AlphaNum &a, const AlphaNum &b, const AlphaNum &c, const AlphaNum &d,
            const AlphaNum &e, const AV &... args) {
        return strings_internal::CatPieces(
                {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
                 static_cast<const AlphaNum &>(args).Piece()...});
    }

    // str_append()
    //
    // Appends one or more values to an existing string.
    //
    // Example:
    //   std::string s = "Hello";
    //   xtext::str_append(&s, " world", 42);
    //   EXPECT_EQ("Hello world42", s);
    inline void str_append(std::string *dest) {}

    inline void str_append(std::string *dest, const AlphaNum &a) {
        dest->append(a.Piece().data(), a.Piece().size());
    }

    inline void str_append(std::string *dest, const AlphaNum &a,
                           const AlphaNum &b) {
        strings_internal::AppendPieces(dest, {a.Piece(), b.Piece()});
    }

    inline void str_append(std::string *dest, const AlphaNum &a,
                           const AlphaNum &b, const AlphaNum &c) {
        strings_internal::AppendPieces(dest, {a.Piece(), b.Piece(), c.Piece()});
    }

    inline void str_append(std::string *dest, const AlphaNum &a,
                           const AlphaNum &b, const AlphaNum &c,
                           const AlphaNum &d) {
        strings_internal::AppendPieces(
                dest, {a.Piece(), b.Piece(), c.Piece(), d.Piece()});
    }

    inline void str_append(std::string *dest, const AlphaNum &a,
                           const AlphaNum &b, const AlphaNum &c,
                           const AlphaNum &d, const AlphaNum &e) {
        strings_internal::AppendPieces(
                dest, {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece()});
    }

    inline void str_append(std::string *dest, const AlphaNum &a,
                           const AlphaNum &b, const AlphaNum &c,
                           const AlphaNum &d, const AlphaNum &e,
                           const AlphaNum &f) {
        strings_internal::AppendPieces(
                dest, {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(), f.Piece()});
    }

    inline void str_append(std::string *dest, const AlphaNum &a,
                           const AlphaNum &b, const AlphaNum &c,
                           const AlphaNum &d, const AlphaNum &e,
                           const AlphaNum &f, const AlphaNum &g) {
        strings_internal::AppendPieces(
                dest, {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
                        f.Piece(), g.Piece()});
    }

    inline void str_append(std::string *dest, const AlphaNum &a,
                           const AlphaNum &b, const AlphaNum &c,
                           const AlphaNum &d, const AlphaNum &e,
                           const AlphaNum &f, const AlphaNum &g,
                           const AlphaNum &h) {
        strings_internal::AppendPieces(
                dest, {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
                        f.Piece(), g.Piece(), h.Piece()});
    }

    inline void str_append(std::string *dest, const AlphaNum &a,
                           const AlphaNum &b, const AlphaNum &c,
                           const AlphaNum &d, const AlphaNum &e,
                           const AlphaNum &f, const AlphaNum &g,
                           const AlphaNum &h, const AlphaNum &i) {
        strings_internal::AppendPieces(
                dest, {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
                        f.Piece(), g.Piece(), h.Piece(), i.Piece()});
    }

    template <typename... AV>
    inline void str_append(std::string *dest,
                           const AlphaNum &a, const AlphaNum &b, const AlphaNum &c,
                           const AlphaNum &d, const AlphaNum &e, const AV &... args) {
        strings_internal::AppendPieces(
                dest, {a.Piece(), b.Piece(), c.Piece(), d.Piece(), e.Piece(),
                       static_cast<const AlphaNum &>(args).Piece()...});
    }

}  // namespace xtext
