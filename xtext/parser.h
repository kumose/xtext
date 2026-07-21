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

#pragma once

#include <charconv>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>

#include <xtext/ascii.h>
#include <xtext/fast_float/fast_float.h>
#include <xtext/macros.h>
#include <xtext/match.h>
#include <xtext/trim.h>

namespace xtext {

    // parse_integer_prefix()
    //
    // Parses the prefix of a string into an integer value.
    // On success, `remained` is updated to point to the first unparsed character.
    // Returns false if parsing fails.
    template<typename T, int Base = 10>
    bool parse_integer_prefix(std::string_view src, T *out,
                              std::string_view *remained) noexcept {
        static_assert(std::is_integral_v<T>, "T must be an integral type");
        static_assert(!std::is_same_v<T, bool>, "T must not be bool");

        if (src.empty()) return false;

        T value;
        auto [ptr, ec] = std::from_chars(src.data(), src.data() + src.size(), value, Base);

        if (XTEXT_LIKELY(ec == std::errc())) {
            if (remained) *remained = src.substr(ptr - src.data());
            *out = value;
            return true;
        }
        return false;
    }

    // parse_integer()
    //
    // Parses the entire string into an integer value.
    // Returns false if there are trailing characters after the number.
    template<typename T, int Base = 10>
    bool parse_integer(std::string_view src, T *out) noexcept {
        std::string_view remained;
        if (!parse_integer_prefix<T, Base>(src, out, &remained)) return false;
        return remained.empty();
    }

    // parse_float_prefix()
    //
    // Parses the prefix of a string into a floating-point value.
    // On success, `remained` is updated to point to the first unparsed character.
    // Returns false if parsing fails.
    template<typename T, xtext::chars_format fmt = xtext::chars_format::general>
    bool parse_float_prefix(std::string_view src, T *out,
                            std::string_view *remained) noexcept {
        static_assert(std::is_floating_point_v<T>, "T must be a floating-point type");

        if (XTEXT_UNLIKELY(src.empty())) return false;

        T value;
        auto r = xtext::from_chars(src.data(), src.data() + src.size(), value, fmt);

        if (XTEXT_LIKELY(r.ec == std::errc())) {
            if (remained) *remained = src.substr(r.ptr - src.data());
            *out = value;
            return true;
        }
        return false;
    }

    // parse_float()
    //
    // Parses the entire string into a floating-point value.
    // Returns false if there are trailing characters after the number.
    template<typename T, xtext::chars_format fmt = xtext::chars_format::general>
    bool parse_float(std::string_view src, T *out) noexcept {
        std::string_view remained;
        if (!parse_float_prefix<T, fmt>(src, out, &remained)) return false;
        return remained.empty();
    }

    // parse_bool_prefix()
    //
    // Parses the prefix of a string into a boolean value.
    // Supports case-insensitive: "true"/"t"/"yes"/"y"/"1" -> true,
    // "false"/"f"/"no"/"n"/"0" -> false.
    // On success, `remained` is updated to point to the first unparsed character.
    inline bool parse_bool_prefix(std::string_view src, bool *out,
                                  std::string_view *remained) noexcept {
        if (src.empty()) return false;

        auto match = [&](std::initializer_list<std::string_view> list, bool result) -> bool {
            for (auto s: list) {
                if (src.size() >= s.size() &&
                    xtext::equals_ignore_case(src.substr(0, s.size()), s)) {
                    if (remained) *remained = src.substr(s.size());
                    *out = result;
                    return true;
                }
            }
            return false;
        };

        if (match({"true", "yes", "t", "y", "1"}, true)) return true;
        if (match({"false", "no", "f", "n", "0"}, false)) return true;

        return false;
    }

    // parse_bool()
    //
    // Parses the entire string into a boolean value.
    // Returns false if there are trailing characters after the boolean.
    inline bool parse_bool(std::string_view src, bool *out) noexcept {
        std::string_view remained;
        if (!parse_bool_prefix(src, out, &remained)) return false;
        return remained.empty();
    }

    // Helper: extracts multiplier from unit string (case-insensitive).
    inline bool get_unit_multiplier(std::string_view unit, double *out) noexcept {
        unit = xtext::trim_all(unit);
        if (unit.empty()) {
            *out = 1.0;
            return true;
        }

        char u = (unit[0] >= 'A' && unit[0] <= 'Z') ? (unit[0] + 32) : unit[0];

        switch (u) {
            case 'k': *out = 1e3; return true;
            case 'm': *out = 1e6; return true;
            case 'g': *out = 1e9; return true;
            case 't': *out = 1e12; return true;
            case 'p': *out = 1e15; return true;
            case 'b': *out = 1.0; return true;
            default:  return false;
        }
    }

    // parse_integer_unit()
    //
    // Parses an integer followed by an optional unit (e.g., "10K", "20GB").
    template<typename T, int Base = 10>
    bool parse_integer_unit(std::string_view src, T *out) noexcept {
        static_assert(std::is_integral_v<T>, "T must be an integral type");

        int64_t val;
        std::string_view remained;
        if (!parse_integer_prefix<int64_t, Base>(src, &val, &remained)) return false;

        double mul;
        if (!get_unit_multiplier(remained, &mul)) return false;

        double final_val = static_cast<double>(val) * mul;
        if (final_val > static_cast<double>(std::numeric_limits<T>::max()) ||
            final_val < static_cast<double>(std::numeric_limits<T>::min())) {
            return false;
        }
        *out = static_cast<T>(final_val);
        return true;
    }

    // parse_float_unit()
    //
    // Parses a float followed by an optional unit (e.g., "1.5M").
    template<typename T, xtext::chars_format fmt = xtext::chars_format::general>
    bool parse_float_unit(std::string_view src, T *out) noexcept {
        static_assert(std::is_floating_point_v<T>, "T must be a floating-point type");

        double val;
        std::string_view remained;
        if (!parse_float_prefix<double, fmt>(src, &val, &remained)) return false;

        double mul;
        if (!get_unit_multiplier(remained, &mul)) return false;

        *out = static_cast<T>(val * mul);
        return true;
    }

    // parse_hex_integer_prefix()
    //
    // Parses the hexadecimal prefix. Automatically strips "0x"/"0X" if present.
    template<typename T, int Base = 16>
    bool parse_hex_integer_prefix(std::string_view src, T *out,
                                  std::string_view *remained) noexcept {
        static_assert(std::is_integral_v<T>, "T must be an integral type");

        if (src.empty()) return false;

        if constexpr (Base == 16) {
            if (src.size() >= 2 && src[0] == '0' && (src[1] == 'x' || src[1] == 'X')) {
                src.remove_prefix(2);
            }
        }

        return parse_integer_prefix<T, Base>(src, out, remained);
    }

    // parse_hex_integer()
    //
    // Parses the entire string as a hexadecimal integer.
    template<typename T, int Base = 16>
    bool parse_hex_integer(std::string_view src, T *out) noexcept {
        std::string_view remained;
        if (!parse_hex_integer_prefix<T, Base>(src, out, &remained)) return false;
        return remained.empty();
    }

    // parse_hex_integer_unit()
    //
    // Parses a hexadecimal integer followed by an optional unit (e.g., "0x10K").
    template<typename T>
    bool parse_hex_integer_unit(std::string_view src, T *out) noexcept {
        static_assert(std::is_integral_v<T>, "T must be an integral type");

        if (src.empty()) return false;

        int64_t val;
        std::string_view remained;
        if (!parse_hex_integer_prefix<int64_t, 16>(src, &val, &remained)) return false;

        double mul;
        if (!get_unit_multiplier(remained, &mul)) return false;

        double final_val = static_cast<double>(val) * mul;
        if (final_val > static_cast<double>(std::numeric_limits<T>::max()) ||
            final_val < static_cast<double>(std::numeric_limits<T>::min())) {
            return false;
        }
        *out = static_cast<T>(final_val);
        return true;
    }

    // parse_bin_integer_prefix()
    //
    // Parses a binary integer prefix (e.g., "0b1010"). Strips "0b"/"0B" if present.
    template<typename T, int Base = 2>
    bool parse_bin_integer_prefix(std::string_view src, T *out,
                                  std::string_view *remained) noexcept {
        static_assert(std::is_integral_v<T>, "T must be an integral type");

        if (src.empty()) return false;

        if constexpr (Base == 2) {
            if (src.size() >= 2 && src[0] == '0' && (src[1] == 'b' || src[1] == 'B')) {
                src.remove_prefix(2);
            }
        }
        return parse_integer_prefix<T, Base>(src, out, remained);
    }

    // parse_bin_integer()
    //
    // Parses the entire string as a binary integer.
    template<typename T, int Base = 2>
    bool parse_bin_integer(std::string_view src, T *out) noexcept {
        std::string_view remained;
        if (!parse_bin_integer_prefix<T, Base>(src, out, &remained)) return false;
        return remained.empty();
    }

    // parse_oct_integer_prefix()
    //
    // Parses an octal integer prefix (e.g., "0755").
    template<typename T, int Base = 8>
    bool parse_oct_integer_prefix(std::string_view src, T *out,
                                  std::string_view *remained) noexcept {
        static_assert(std::is_integral_v<T>, "T must be an integral type");
        return parse_integer_prefix<T, Base>(src, out, remained);
    }

    // parse_oct_integer()
    //
    // Parses the entire string as an octal integer.
    template<typename T, int Base = 8>
    bool parse_oct_integer(std::string_view src, T *out) noexcept {
        std::string_view remained;
        if (!parse_oct_integer_prefix<T, Base>(src, out, &remained)) return false;
        return remained.empty();
    }

}  // namespace xtext
