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
// File: predicate.h
// -----------------------------------------------------------------------------
//
// This file provides character predicates for use with `trim` and other
// algorithms. Each predicate has a `bool operator()(char) const` that returns
// true when the character matches the condition.

#pragma once

#include <xtext/ascii.h>

namespace xtext {

    struct AsciiIsSpace {
        bool operator()(char c) const noexcept {
            return ascii_isspace(static_cast<unsigned char>(c));
        }
    };

    struct AsciiIsDigit {
        bool operator()(char c) const noexcept {
            return ascii_isdigit(static_cast<unsigned char>(c));
        }
    };

    struct AsciiIsAlpha {
        bool operator()(char c) const noexcept {
            return ascii_isalpha(static_cast<unsigned char>(c));
        }
    };

    struct AsciiIsAlnum {
        bool operator()(char c) const noexcept {
            return ascii_isalnum(static_cast<unsigned char>(c));
        }
    };

    struct AsciiIsUpper {
        bool operator()(char c) const noexcept {
            return ascii_isupper(static_cast<unsigned char>(c));
        }
    };

    struct AsciiIsLower {
        bool operator()(char c) const noexcept {
            return ascii_islower(static_cast<unsigned char>(c));
        }
    };

}  // namespace xtext
