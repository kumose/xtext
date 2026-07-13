function(add_fastfloat_test NAME)
    kmcmake_cc_test(
        NAME ${NAME}
        MODULE fast_float
        SOURCES ${NAME}.cpp
        LINKS ${PROJECT_NAME}::xtext
        CXXOPTS ${KMCMAKE_CXX_OPTIONS}
    )
endfunction()

add_fastfloat_test(basictest)
add_fastfloat_test(example_test)
add_fastfloat_test(example_comma_test)
add_fastfloat_test(example_integer_times_pow10)
add_fastfloat_test(fast_int)
if(NOT APPLE)
    add_fastfloat_test(random_string)
endif()
add_fastfloat_test(short_random_string)
add_fastfloat_test(string_test)
add_fastfloat_test(supported_chars_test)
add_fastfloat_test(wide_char_test)
