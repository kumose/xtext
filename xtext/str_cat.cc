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

#include <xtext/str_cat.h>

#include <algorithm>
#include <cstddef>
#include <string>

#include <xtext/details/resize_uninitialized.h>

namespace xtext::strings_internal {

std::string CatPieces(std::initializer_list<std::string_view> pieces) {
    size_t total_size = 0;
    for (const auto &piece : pieces) {
        total_size += piece.size();
    }
    std::string result;
    STLStringResizeUninitialized(&result, total_size);
    char *pos = &result[0];
    for (const auto &piece : pieces) {
        if (!piece.empty()) {
            std::memcpy(pos, piece.data(), piece.size());
            pos += piece.size();
        }
    }
    return result;
}

void AppendPieces(std::string *dest,
                  std::initializer_list<std::string_view> pieces) {
    size_t total_size = dest->size();
    for (const auto &piece : pieces) {
        total_size += piece.size();
    }
    dest->reserve(total_size);
    for (const auto &piece : pieces) {
        dest->append(piece.data(), piece.size());
    }
}

}  // namespace xtext::strings_internal
