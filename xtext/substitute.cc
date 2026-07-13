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

#include <xtext/substitute.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <string>

#include <xtext/ascii.h>
#include <xtext/details/resize_uninitialized.h>
#include <xtext/str_cat.h>

namespace xtext::substitute_internal {

void SubstituteAndAppendArray(
    std::string *output, std::string_view format,
    const std::string_view *args_array, size_t num_args) {
  size_t size = 0;
  for (size_t i = 0; i < format.size(); i++) {
    if (format[i] == '$') {
      if (i + 1 >= format.size()) {
#ifndef NDEBUG
        abort();
#endif
        return;
      } else if (xtext::ascii_isdigit(
                     static_cast<unsigned char>(format[i + 1]))) {
        int index = format[i + 1] - '0';
        if (static_cast<size_t>(index) >= num_args) {
#ifndef NDEBUG
          abort();
#endif
          return;
        }
        size += args_array[index].size();
        ++i;
      } else if (format[i + 1] == '$') {
        ++size;
        ++i;
      } else {
#ifndef NDEBUG
        abort();
#endif
        return;
      }
    } else {
      ++size;
    }
  }

  if (size == 0) return;

  size_t original_size = output->size();
  strings_internal::STLStringResizeUninitializedAmortized(
      output, original_size + size);
  char *target = &(*output)[original_size];
  for (size_t i = 0; i < format.size(); i++) {
    if (format[i] == '$') {
      if (xtext::ascii_isdigit(static_cast<unsigned char>(format[i + 1]))) {
        const std::string_view src = args_array[format[i + 1] - '0'];
        target = std::copy(src.begin(), src.end(), target);
        ++i;
      } else if (format[i + 1] == '$') {
        *target++ = '$';
        ++i;
      }
    } else {
      *target++ = format[i];
    }
  }

  assert(target == output->data() + output->size());
}

namespace {

const char kHexChar[17] = "0123456789abcdef";

}  // namespace

std::string_view Arg::ConvertSigned(int64_t value) {
  auto end = fmt::format_to(scratch_, "{}", value);
  return std::string_view(scratch_, static_cast<size_t>(end - scratch_));
}

std::string_view Arg::ConvertUnsigned(uint64_t value) {
  auto end = fmt::format_to(scratch_, "{}", value);
  return std::string_view(scratch_, static_cast<size_t>(end - scratch_));
}

std::string_view Arg::ConvertFloat(double value) {
  auto end = fmt::format_to(scratch_, "{:.6g}", value);
  return std::string_view(scratch_, static_cast<size_t>(end - scratch_));
}

std::string_view Arg::ConvertPtr(const void *value) {
  if (value == nullptr) {
    return std::string_view("NULL");
  }
  char *ptr = scratch_ + sizeof(scratch_);
  uintptr_t num = reinterpret_cast<uintptr_t>(value);
  do {
    *--ptr = kHexChar[num & 0xf];
    num >>= 4;
  } while (num != 0);
  *--ptr = 'x';
  *--ptr = '0';
  return std::string_view(
      ptr, static_cast<size_t>(scratch_ + sizeof(scratch_) - ptr));
}

}  // namespace xtext::substitute_internal
