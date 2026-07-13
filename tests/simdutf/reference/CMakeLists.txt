kmcmake_cc_library(
    NAME simdutf_tests_reference
    SOURCES
    encode_utf8.cpp
    encode_utf16.cpp
    encode_utf32.cpp
    encode_latin1.cpp
    validate_utf8.cpp
    validate_utf16.cpp
    validate_utf32.cpp
    validate_utf8_to_latin1.cpp
    validate_utf16_to_latin1.cpp
    validate_utf32_to_latin1.cpp
    HEADERS
    encode_utf8.h
    encode_utf16.h
    encode_utf32.h
    encode_latin1.h
    decode_utf16.h
    decode_utf32.h
    validate_utf8.h
    validate_utf16.h
    validate_utf32.h
    validate_utf8_to_latin1.h
    validate_utf16_to_latin1.h
    validate_utf32_to_latin1.h
    CXXOPTS ${KMCMAKE_CXX_OPTIONS}
    LINKS ${PROJECT_NAME}::xtext_builtin
    EXCLUDE_SYSTEM
)
