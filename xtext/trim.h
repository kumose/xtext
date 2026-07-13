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
// File: trim.h
// -----------------------------------------------------------------------------
//
// This file contains functions for trimming characters from the beginning
// and/or end of strings.

#pragma once

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>

#include <xtext/predicate.h>

namespace xtext {

    // trim_left()
    //
    // Strips leading characters that satisfy the predicate.
    template <typename Pred>
    std::string_view trim_left(std::string_view str, Pred p) {
        auto it = std::find_if_not(str.begin(), str.end(), std::move(p));
        return str.substr(static_cast<size_t>(it - str.begin()));
    }

    inline std::string_view trim_left(std::string_view str) {
        return trim_left(str, AsciiIsSpace{});
    }

    inline std::string_view trim_left(std::string_view str, char c) {
        return trim_left(str, [c](char x) { return x == c; });
    }

    inline std::string_view trim_left(std::string_view str,
                                      std::string_view chars) {
        return trim_left(str, [chars](char x) {
            return chars.find(x) != std::string_view::npos;
        });
    }

    inline std::string_view trim_left(std::string_view str,
                                      const char *chars) {
        return trim_left(str, std::string_view(chars));
    }

    template <typename Pred>
    void trim_left(std::string *str, Pred p) {
        auto it = std::find_if_not(str->begin(), str->end(), std::move(p));
        str->erase(str->begin(), it);
    }

    inline void trim_left(std::string *str) {
        trim_left(str, AsciiIsSpace{});
    }

    inline void trim_left(std::string *str, char c) {
        trim_left(str, [c](char x) { return x == c; });
    }

    inline void trim_left(std::string *str, std::string_view chars) {
        trim_left(str, [chars](char x) {
            return chars.find(x) != std::string_view::npos;
        });
    }

    inline void trim_left(std::string *str, const char *chars) {
        trim_left(str, std::string_view(chars));
    }

    // trim_right()
    //
    // Strips trailing characters that satisfy the predicate.
    template <typename Pred>
    std::string_view trim_right(std::string_view str, Pred p) {
        auto it = std::find_if_not(str.rbegin(), str.rend(), std::move(p));
        return str.substr(0, static_cast<size_t>(str.rend() - it));
    }

    inline std::string_view trim_right(std::string_view str) {
        return trim_right(str, AsciiIsSpace{});
    }

    inline std::string_view trim_right(std::string_view str, char c) {
        return trim_right(str, [c](char x) { return x == c; });
    }

    inline std::string_view trim_right(std::string_view str,
                                       std::string_view chars) {
        return trim_right(str, [chars](char x) {
            return chars.find(x) != std::string_view::npos;
        });
    }

    inline std::string_view trim_right(std::string_view str,
                                       const char *chars) {
        return trim_right(str, std::string_view(chars));
    }

    template <typename Pred>
    void trim_right(std::string *str, Pred p) {
        auto it = std::find_if_not(str->rbegin(), str->rend(), std::move(p));
        str->erase(static_cast<size_t>(str->rend() - it));
    }

    inline void trim_right(std::string *str) {
        trim_right(str, AsciiIsSpace{});
    }

    inline void trim_right(std::string *str, char c) {
        trim_right(str, [c](char x) { return x == c; });
    }

    inline void trim_right(std::string *str, std::string_view chars) {
        trim_right(str, [chars](char x) {
            return chars.find(x) != std::string_view::npos;
        });
    }

    inline void trim_right(std::string *str, const char *chars) {
        trim_right(str, std::string_view(chars));
    }

    // trim_all()
    //
    // Strips leading and trailing characters that satisfy the predicate.
    template <typename Pred>
    std::string_view trim_all(std::string_view str, Pred p) {
        return trim_right(trim_left(str, p), p);
    }

    inline std::string_view trim_all(std::string_view str) {
        return trim_all(str, AsciiIsSpace{});
    }

    inline std::string_view trim_all(std::string_view str, char c) {
        return trim_all(str, [c](char x) { return x == c; });
    }

    inline std::string_view trim_all(std::string_view str,
                                     std::string_view chars) {
        return trim_all(str, [chars](char x) {
            return chars.find(x) != std::string_view::npos;
        });
    }

    inline std::string_view trim_all(std::string_view str,
                                     const char *chars) {
        return trim_all(str, std::string_view(chars));
    }

    template <typename Pred>
    void trim_all(std::string *str, Pred p) {
        trim_left(str, p);
        trim_right(str, p);
    }

    inline void trim_all(std::string *str) {
        trim_all(str, AsciiIsSpace{});
    }

    inline void trim_all(std::string *str, char c) {
        trim_all(str, [c](char x) { return x == c; });
    }

    inline void trim_all(std::string *str, std::string_view chars) {
        trim_all(str, [chars](char x) {
            return chars.find(x) != std::string_view::npos;
        });
    }

    inline void trim_all(std::string *str, const char *chars) {
        trim_all(str, std::string_view(chars));
    }

    // trim_complete()
    //
    // Strips leading/trailing whitespace and collapses internal whitespace
    // sequences to a single space.
    void trim_complete(std::string *str);

}  // namespace xtext
