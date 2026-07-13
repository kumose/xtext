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

#include <xtext/match.h>

#include <algorithm>
#include <cstdint>

#include <xtext/ascii.h>
#include <xtext/macros.h>
#include <xtext/details/memutil.h>

namespace xtext {

    bool equals_ignore_case(std::string_view piece1,
                            std::string_view piece2) noexcept {
        return (piece1.size() == piece2.size() &&
                0 == strings_internal::memcasecmp(piece1.data(), piece2.data(),
                                                  piece1.size()));
    }

    bool str_contains_ignore_case(std::string_view haystack,
                                  std::string_view needle) noexcept {
        while (haystack.size() >= needle.size()) {
            if (starts_with_ignore_case(haystack, needle)) return true;
            haystack.remove_prefix(1);
        }
        return false;
    }

    bool str_contains_ignore_case(std::string_view haystack,
                                  char needle) noexcept {
        char upper_needle = xtext::ascii_toupper(static_cast<unsigned char>(needle));
        char lower_needle = xtext::ascii_tolower(static_cast<unsigned char>(needle));
        if (upper_needle == lower_needle) {
            return str_contains(haystack, needle);
        } else {
            const char both_cstr[3] = {lower_needle, upper_needle, '\0'};
            return haystack.find_first_of(both_cstr) != std::string_view::npos;
        }
    }

    bool starts_with_ignore_case(std::string_view text,
                                 std::string_view prefix) noexcept {
        return (text.size() >= prefix.size()) &&
               equals_ignore_case(text.substr(0, prefix.size()), prefix);
    }

    bool ends_with_ignore_case(std::string_view text,
                               std::string_view suffix) noexcept {
        return (text.size() >= suffix.size()) &&
               equals_ignore_case(text.substr(text.size() - suffix.size()), suffix);
    }

}  // namespace xtext
