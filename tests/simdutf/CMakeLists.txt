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

kmcmake_cc_library(
        NAME simdutf_tests_reference
        SOURCES
        reference/encode_utf8.cc
        reference/encode_utf16.cc
        reference/encode_utf32.cc
        reference/encode_latin1.cc
        reference/validate_utf8.cc
        reference/validate_utf16.cc
        reference/validate_utf32.cc
        reference/validate_utf8_to_latin1.cc
        reference/validate_utf16_to_latin1.cc
        reference/validate_utf32_to_latin1.cc
        CXXOPTS
        ${KMCMAKE_CXX_OPTIONS}
        LINKS
        ${PROJECT_NAME}::xtext_static
        ${KMCMAKE_DEPS_LINK}
)

kmcmake_cc_library(
        NAME simdutf_tests_helpers
        SOURCES
        helpers/test.cc
        helpers/random_int.cc
        helpers/random_utf8.cc
        helpers/random_utf16.cc
        helpers/random_utf32.cc
        helpers/transcode_test_base.cc
        CXXOPTS ${KMCMAKE_CXX_OPTIONS}
        DEFINES XTEXT_SIMDUTF_TEST_LOOP_TRIALS=100
        LINKS
        ${PROJECT_NAME}::xtext_static
        simdutf_tests_reference_static
        ${KMCMAKE_DEPS_LINK}

)


set(UTF_TESTS
        validate_ascii_basic_tests
        validate_ascii_with_errors_tests
        validate_utf8_basic_tests
        validate_utf8_with_errors_tests
        validate_utf16le_basic_tests
        validate_utf16be_basic_tests
        validate_utf32_basic_tests
        readme_tests
        count_utf8
        count_utf16le
        count_utf16be
        detect_encodings_tests
        bele_tests
        convert_utf8_to_utf16le_tests
        convert_utf8_to_utf16be_tests
        convert_utf8_to_utf32_tests
        convert_utf16le_to_utf8_tests
        convert_utf16be_to_utf8_tests
        convert_utf32_to_utf8_tests
        convert_utf32_to_utf16le_tests
        convert_utf32_to_utf16be_tests
        convert_latin1_to_utf8_tests
        convert_utf8_to_latin1_tests
        text_encoding_tests
        base64_tests
        simdutf_c_tests
)

foreach (NAME ${UTF_TESTS})
    kmcmake_cc_test(
            NAME ${NAME}
            MODULE uft
            SOURCES
            ${NAME}.cc
            LINKS
            ${PROJECT_NAME}::simdutf_tests_helpers_static
            ${PROJECT_NAME}::simdutf_tests_reference_static
            ${PROJECT_NAME}::xtext_static
            ${KMCMAKE_DEPS_LINK}
            CXXOPTS
            ${KMCMAKE_CXX_OPTIONS}
    )
endforeach ()
