function(add_fastfloat_test NAME)
    kmcmake_cc_test(
        NAME ${NAME}
        MODULE fast_float
        SOURCES ${NAME}.cpp
        LINKS ${PROJECT_NAME}::xtext_builtin
        CXXOPTS ${KMCMAKE_CXX_OPTIONS}
    )
endfunction()

add_fastfloat_test(basictest)
add_fastfloat_test(example_test)
add_fastfloat_test(example_comma_test)
add_fastfloat_test(example_integer_times_pow10)
add_fastfloat_test(fast_int)
add_fastfloat_test(random_string)
add_fastfloat_test(short_random_string)
add_fastfloat_test(string_test)
add_fastfloat_test(supported_chars_test)
add_fastfloat_test(wide_char_test)
