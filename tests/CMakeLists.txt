add_subdirectory(simdutf/reference)
add_subdirectory(simdutf/helpers)
add_subdirectory(fast_float)

set(SIMDUTF_TEST_LIBS
    ${PROJECT_NAME}::xtext_builtin
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
