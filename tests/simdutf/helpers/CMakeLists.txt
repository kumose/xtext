kmcmake_cc_library(
    NAME simdutf_tests_helpers
    SOURCES
    test.cpp
    random_int.cpp
    random_utf8.cpp
    random_utf16.cpp
    random_utf32.cpp
    transcode_test_base.cpp
    HEADERS
    test.h
    compiletime_conversions.h
    fixed_string.h
    random_int.h
    random_utf8.h
    random_utf16.h
    random_utf32.h
    transcode_test_base.h
    utf16.h
    CXXOPTS ${KMCMAKE_CXX_OPTIONS}
    DEFINES XTEXT_SIMDUTF_TEST_LOOP_TRIALS=100
    LINKS ${PROJECT_NAME}::xtext_builtin simdutf_tests_reference_static
    EXCLUDE_SYSTEM
)
