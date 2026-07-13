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

#include <xtext/str_replace.h>

#include <cstddef>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

namespace xtext::strings_internal {

using FixedMapping =
    std::initializer_list<std::pair<std::string_view, std::string_view>>;

int ApplySubstitutions(
    std::string_view s,
    std::vector<ViableSubstitution> *subs_ptr,
    std::string *result_ptr) {
  auto& subs = *subs_ptr;
  int substitutions = 0;
  size_t pos = 0;
  while (!subs.empty()) {
    auto& sub = subs.back();
    if (sub.offset >= pos) {
      if (pos <= s.size()) {
        result_ptr->append(s.substr(pos, sub.offset - pos));
        result_ptr->append(sub.replacement);
      }
      pos = sub.offset + sub.old.size();
      substitutions += 1;
    }
    sub.offset = s.find(sub.old, pos);
    if (sub.offset == s.npos) {
      subs.pop_back();
    } else {
      size_t index = subs.size();
      while (--index && subs[index - 1].OccursBefore(subs[index])) {
        std::swap(subs[index], subs[index - 1]);
      }
    }
  }
  result_ptr->append(s.data() + pos, s.size() - pos);
  return substitutions;
}

}  // namespace xtext::strings_internal

std::string xtext::str_replace_all(std::string_view s,
                          strings_internal::FixedMapping replacements) {
  return str_replace_all<strings_internal::FixedMapping>(s, replacements);
}

int xtext::str_replace_all(strings_internal::FixedMapping replacements,
                  std::string *target) {
  return str_replace_all<strings_internal::FixedMapping>(replacements, target);
}
