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

#if defined(__GNUC__) || defined(__clang__)
#define XTEXT_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define XTEXT_LIKELY(x)   __builtin_expect(!!(x), 1)
#else
#define XTEXT_UNLIKELY(x) (x)
#define XTEXT_LIKELY(x)   (x)
#endif

#if defined(__clang__) && defined(__has_attribute) && \
    __has_attribute(warn_unused_result)
#define XTEXT_MUST_USE_RESULT __attribute__((warn_unused_result))
#else
#define XTEXT_MUST_USE_RESULT
#endif
