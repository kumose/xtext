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

#include <algorithm>
#include <string>
#include <type_traits>
#include <utility>

namespace xtext::strings_internal {

    template <typename string_type, typename = void>
    struct ResizeUninitializedTraits {
        using HasMember = std::false_type;
        static void Resize(string_type *s, size_t new_size) { s->resize(new_size); }
    };

    template <typename string_type>
    struct ResizeUninitializedTraits<
            string_type, std::void_t<decltype(std::declval<string_type &>().__resize_default_init(237))>> {
        using HasMember = std::true_type;
        static void Resize(string_type *s, size_t new_size) {
            s->__resize_default_init(new_size);
        }
    };

    template <typename string_type>
    inline void STLStringResizeUninitialized(string_type *s, size_t new_size) {
        ResizeUninitializedTraits<string_type>::Resize(s, new_size);
    }

    template <typename string_type>
    inline void STLStringReserveAmortized(string_type *s, size_t new_size) {
        if (new_size > s->capacity()) {
            s->reserve((std::max)(new_size, 2 * s->capacity()));
        }
    }

    template <typename string_type, typename = void>
    struct AppendUninitializedTraits {
        static void Append(string_type *s, size_t n) {
            s->append(n, typename string_type::value_type());
        }
    };

    template <typename string_type>
    struct AppendUninitializedTraits<
            string_type, std::void_t<decltype(std::declval<string_type &>()
                    .__append_default_init(237))>> {
        static void Append(string_type *s, size_t n) {
            s->__append_default_init(n);
        }
    };

    template <typename string_type>
    inline void STLStringResizeUninitializedAmortized(
            string_type *s, size_t new_size) {
        size_t size = s->size();
        if (new_size > size) {
            AppendUninitializedTraits<string_type>::Append(s, new_size - size);
        } else {
            s->erase(new_size);
        }
    }

}  // namespace xtext::strings_internal
