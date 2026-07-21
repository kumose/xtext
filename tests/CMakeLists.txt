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

include_directories(doctest)

set(STRINGS_TESTS
        ascii_test
        match_test
        strip_test
        parser_test
        str_cat_test
        str_format_test
        str_split_test
        substitute_test
        trim_test
        fnmatch_test
)

foreach (NAME ${STRINGS_TESTS})
    kmcmake_cc_test(
            NAME ${NAME}
            MODULE strings
            SOURCES
            ${NAME}.cc
            LINKS
            ${PROJECT_NAME}::xtext_static
            ${KMCMAKE_DEPS_LINK}
            CXXOPTS
            ${KMCMAKE_CXX_OPTIONS}
    )
endforeach ()

add_subdirectory(fast_float)
add_subdirectory(simdutf)