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

#include <xtext/details/memutil.h>
#include <xtext/ascii.h>

namespace xtext::strings_internal {

    int memcasecmp(const char *s1, const char *s2, size_t len) {
        const unsigned char *us1 = reinterpret_cast<const unsigned char *>(s1);
        const unsigned char *us2 = reinterpret_cast<const unsigned char *>(s2);
        for (size_t i = 0; i < len; ++i) {
            unsigned char c1 = us1[i];
            unsigned char c2 = us2[i];
            if (c1 != c2) {
                unsigned char u1 = xtext::ascii_tolower(c1);
                unsigned char u2 = xtext::ascii_tolower(c2);
                if (u1 != u2) {
                    return u1 - u2;
                }
            }
        }
        return 0;
    }

}  // namespace xtext::strings_internal
