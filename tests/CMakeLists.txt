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

include(doctest/doctest.cmake)

function(add_doctest_test NAME)
    add_executable(${NAME} ${NAME}.cc)
    target_link_libraries(${NAME} ${PROJECT_NAME}::xtext)
    target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/doctest)
    target_compile_options(${NAME} PRIVATE ${KMCMAKE_CXX_OPTIONS})
    doctest_discover_tests(${NAME})
endfunction()

add_doctest_test(ascii_test)
add_doctest_test(match_test)
add_doctest_test(strip_test)
add_doctest_test(parser_test)
add_doctest_test(str_cat_test)
add_doctest_test(str_format_test)
add_doctest_test(str_split_test)
add_doctest_test(substitute_test)
add_doctest_test(trim_test)

add_subdirectory(simdutf/reference)
add_subdirectory(simdutf/helpers)
add_subdirectory(fast_float)

set(SIMDUTF_TEST_LIBS
    ${PROJECT_NAME}::xtext
    simdutf_tests_helpers_static
    simdutf_tests_reference_static
)

function(add_simdutf_test NAME)
    kmcmake_cc_test(
        NAME ${NAME}
        MODULE simdutf
        SOURCES simdutf/${NAME}.cpp
        DEFINES XTEXT_SIMDUTF_BASE64_TEST_MAXLEN=100
        LINKS ${SIMDUTF_TEST_LIBS}
        CXXOPTS ${KMCMAKE_CXX_OPTIONS}
    )
endfunction()

add_simdutf_test(validate_ascii_basic_tests)
add_simdutf_test(validate_ascii_with_errors_tests)
add_simdutf_test(validate_utf8_basic_tests)
add_simdutf_test(validate_utf8_with_errors_tests)
add_simdutf_test(validate_utf16le_basic_tests)
add_simdutf_test(validate_utf16be_basic_tests)
add_simdutf_test(validate_utf32_basic_tests)
add_simdutf_test(readme_tests)
add_simdutf_test(count_utf8)
add_simdutf_test(count_utf16le)
add_simdutf_test(count_utf16be)
add_simdutf_test(detect_encodings_tests)
add_simdutf_test(bele_tests)
add_simdutf_test(convert_utf8_to_utf16le_tests)
add_simdutf_test(convert_utf8_to_utf16be_tests)
add_simdutf_test(convert_utf8_to_utf32_tests)
add_simdutf_test(convert_utf16le_to_utf8_tests)
add_simdutf_test(convert_utf16be_to_utf8_tests)
add_simdutf_test(convert_utf32_to_utf8_tests)
add_simdutf_test(convert_utf32_to_utf16le_tests)
add_simdutf_test(convert_utf32_to_utf16be_tests)
add_simdutf_test(convert_latin1_to_utf8_tests)
add_simdutf_test(convert_utf8_to_latin1_tests)
add_simdutf_test(text_encoding_tests)
add_simdutf_test(base64_tests)
add_simdutf_test(simdutf_c_tests)


