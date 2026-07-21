# Copyright (C) Kumo inc. and its affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set(FAST_FLOAT_TESTS
        basictest
        example_test
        example_comma_test
        example_integer_times_pow10
        fast_int
        short_random_string
        string_test
        supported_chars_test
        wide_char_test
)

foreach (NAME ${FAST_FLOAT_TESTS})
    kmcmake_cc_test(
            NAME ${NAME}
            MODULE fast_float
            SOURCES
            ${NAME}.cc
            LINKS
            ${PROJECT_NAME}::xtext_static
            ${KMCMAKE_DEPS_LINK}
            CXXOPTS
            ${KMCMAKE_CXX_OPTIONS}
    )
endforeach ()


if (NOT APPLE)
    kmcmake_cc_test(
            NAME random_string
            MODULE utf
            SOURCES random_string.cc
            LINKS
            ${PROJECT_NAME}::xtext_static
            ${KMCMAKE_DEPS_LINK}
            CXXOPTS
            ${KMCMAKE_CXX_OPTIONS}
    )
endif ()
