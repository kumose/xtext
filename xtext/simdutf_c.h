/***
 * xtext_c.h.h - C API for simdutf
 * This is currently experimental.
 * We are committed to keeping the C API, but there might be mistakes in our
 * implementation. Please report any issues you find.
 */

#ifndef XTEXT_SIMDUTF_C_H
#define XTEXT_SIMDUTF_C_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
  #include <cstdint>
#else  // __cplusplus
  #ifdef __has_include
    #if __has_include(<uchar.h>)
      #include <uchar.h>
    #else // __has_include(<uchar.h>)
      #define char16_t uint16_t
      #define char32_t uint32_t
    #endif // __has_include(<uchar.h>)
  #else    // __has_include(<uchar.h>)
    #define char16_t uint16_t
    #define char32_t uint32_t
  #endif // __has_include
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

/* C-friendly subset of simdutf errors */
typedef enum xtext_error_code {
  XTEXT_SIMDUTF_ERROR_SUCCESS = 0,
  XTEXT_SIMDUTF_ERROR_HEADER_BITS,
  XTEXT_SIMDUTF_ERROR_TOO_SHORT,
  XTEXT_SIMDUTF_ERROR_TOO_LONG,
  XTEXT_SIMDUTF_ERROR_OVERLONG,
  XTEXT_SIMDUTF_ERROR_TOO_LARGE,
  XTEXT_SIMDUTF_ERROR_SURROGATE,
  XTEXT_SIMDUTF_ERROR_INVALID_BASE64_CHARACTER,
  XTEXT_SIMDUTF_ERROR_BASE64_INPUT_REMAINDER,
  XTEXT_SIMDUTF_ERROR_BASE64_EXTRA_BITS,
  XTEXT_SIMDUTF_ERROR_OUTPUT_BUFFER_TOO_SMALL,
  XTEXT_SIMDUTF_ERROR_OTHER
} xtext_error_code;

typedef struct xtext_result {
  xtext_error_code error;
  size_t count; /* position of error or number of code units validated */
} xtext_result;

typedef struct xtext_full_result {
  xtext_error_code error;
  size_t input_count;  /* number of input units consumed */
  size_t output_count; /* number of output bytes written */
} xtext_full_result;

typedef enum xtext_encoding_type {
  XTEXT_SIMDUTF_ENCODING_UNSPECIFIED = 0,
  XTEXT_SIMDUTF_ENCODING_UTF8 = 1,
  XTEXT_SIMDUTF_ENCODING_UTF16_LE = 2,
  XTEXT_SIMDUTF_ENCODING_UTF16_BE = 4,
  XTEXT_SIMDUTF_ENCODING_UTF32_LE = 8,
  XTEXT_SIMDUTF_ENCODING_UTF32_BE = 16
} xtext_encoding_type;

/* Validate UTF-8: returns true iff input is valid UTF-8 */
bool xtext_validate_utf8(const char *buf, size_t len);

/* Validate UTF-8 with detailed result */
xtext_result xtext_validate_utf8_with_errors(const char *buf, size_t len);

/* Encoding detection */
xtext_encoding_type xtext_autodetect_encoding(const char *input,
                                                  size_t length);
int xtext_detect_encodings(const char *input, size_t length);

/* ASCII validation */
bool xtext_validate_ascii(const char *buf, size_t len);
xtext_result xtext_validate_ascii_with_errors(const char *buf, size_t len);

/* UTF-16 ASCII checks */
bool xtext_validate_utf16_as_ascii(const char16_t *buf, size_t len);
bool xtext_validate_utf16be_as_ascii(const char16_t *buf, size_t len);
bool xtext_validate_utf16le_as_ascii(const char16_t *buf, size_t len);

/* UTF-16/UTF-8/UTF-32 validation (native/endian-specific) */
bool xtext_validate_utf16(const char16_t *buf, size_t len);
bool xtext_validate_utf16le(const char16_t *buf, size_t len);
bool xtext_validate_utf16be(const char16_t *buf, size_t len);
xtext_result xtext_validate_utf16_with_errors(const char16_t *buf,
                                                  size_t len);
xtext_result xtext_validate_utf16le_with_errors(const char16_t *buf,
                                                    size_t len);
xtext_result xtext_validate_utf16be_with_errors(const char16_t *buf,
                                                    size_t len);

bool xtext_validate_utf32(const char32_t *buf, size_t len);
xtext_result xtext_validate_utf32_with_errors(const char32_t *buf,
                                                  size_t len);

/* to_well_formed UTF-16 helpers */
void xtext_to_well_formed_utf16le(const char16_t *input, size_t len,
                                    char16_t *output);
void xtext_to_well_formed_utf16be(const char16_t *input, size_t len,
                                    char16_t *output);
void xtext_to_well_formed_utf16(const char16_t *input, size_t len,
                                  char16_t *output);

/* Counting */
size_t xtext_count_utf16(const char16_t *input, size_t length);
size_t xtext_count_utf16le(const char16_t *input, size_t length);
size_t xtext_count_utf16be(const char16_t *input, size_t length);
size_t xtext_count_utf8(const char *input, size_t length);

/* Length estimators */
size_t xtext_utf8_length_from_latin1(const char *input, size_t length);
size_t xtext_latin1_length_from_utf8(const char *input, size_t length);
size_t xtext_latin1_length_from_utf16(size_t length);
size_t xtext_latin1_length_from_utf32(size_t length);
size_t xtext_utf16_length_from_utf8(const char *input, size_t length);
size_t xtext_utf32_length_from_utf8(const char *input, size_t length);
size_t xtext_utf8_length_from_utf16(const char16_t *input, size_t length);
size_t xtext_utf8_length_from_utf32(const char32_t *input, size_t length);
xtext_result
xtext_utf8_length_from_utf16_with_replacement(const char16_t *input,
                                                size_t length);
size_t xtext_utf8_length_from_utf16le(const char16_t *input, size_t length);
size_t xtext_utf8_length_from_utf16be(const char16_t *input, size_t length);
xtext_result
xtext_utf8_length_from_utf16le_with_replacement(const char16_t *input,
                                                  size_t length);
xtext_result
xtext_utf8_length_from_utf16be_with_replacement(const char16_t *input,
                                                  size_t length);

/* Conversions: latin1 <-> utf8, utf8 <-> utf16/utf32, utf16 <-> utf8, etc. */
size_t xtext_convert_latin1_to_utf8(const char *input, size_t length,
                                      char *output);
size_t xtext_convert_latin1_to_utf8_safe(const char *input, size_t length,
                                           char *output, size_t utf8_len);
size_t xtext_convert_latin1_to_utf16le(const char *input, size_t length,
                                         char16_t *output);
size_t xtext_convert_latin1_to_utf16be(const char *input, size_t length,
                                         char16_t *output);
size_t xtext_convert_latin1_to_utf16(const char *input, size_t length,
                                       char16_t *output);
size_t xtext_convert_latin1_to_utf32(const char *input, size_t length,
                                       char32_t *output);

size_t xtext_convert_utf8_to_latin1(const char *input, size_t length,
                                      char *output);
size_t xtext_convert_utf8_to_utf16le(const char *input, size_t length,
                                       char16_t *output);
size_t xtext_convert_utf8_to_utf16be(const char *input, size_t length,
                                       char16_t *output);
size_t xtext_convert_utf8_to_utf16(const char *input, size_t length,
                                     char16_t *output);

size_t xtext_convert_utf8_to_utf32(const char *input, size_t length,
                                     char32_t *output);
xtext_result xtext_convert_utf8_to_latin1_with_errors(const char *input,
                                                          size_t length,
                                                          char *output);
xtext_result xtext_convert_utf8_to_utf16_with_errors(const char *input,
                                                         size_t length,
                                                         char16_t *output);
xtext_result xtext_convert_utf8_to_utf16le_with_errors(const char *input,
                                                           size_t length,
                                                           char16_t *output);
xtext_result xtext_convert_utf8_to_utf16be_with_errors(const char *input,
                                                           size_t length,
                                                           char16_t *output);
xtext_result xtext_convert_utf8_to_utf32_with_errors(const char *input,
                                                         size_t length,
                                                         char32_t *output);

/* Conversions assuming valid input */
size_t xtext_convert_valid_utf8_to_latin1(const char *input, size_t length,
                                            char *output);
size_t xtext_convert_valid_utf8_to_utf16le(const char *input, size_t length,
                                             char16_t *output);
size_t xtext_convert_valid_utf8_to_utf16be(const char *input, size_t length,
                                             char16_t *output);
size_t xtext_convert_valid_utf8_to_utf32(const char *input, size_t length,
                                           char32_t *output);

/* UTF-16 -> UTF-8 and related conversions */
size_t xtext_convert_utf16_to_utf8(const char16_t *input, size_t length,
                                     char *output);
size_t xtext_convert_utf16le_to_utf8(const char16_t *input, size_t length,
                                       char *output);
size_t xtext_convert_utf16be_to_utf8(const char16_t *input, size_t length,
                                       char *output);
size_t xtext_convert_utf16_to_utf8_safe(const char16_t *input, size_t length,
                                          char *output, size_t utf8_len);
size_t xtext_convert_utf16_to_latin1(const char16_t *input, size_t length,
                                       char *output);
size_t xtext_convert_utf16le_to_latin1(const char16_t *input, size_t length,
                                         char *output);
size_t xtext_convert_utf16be_to_latin1(const char16_t *input, size_t length,
                                         char *output);
xtext_result
xtext_convert_utf16_to_latin1_with_errors(const char16_t *input,
                                            size_t length, char *output);
xtext_result
xtext_convert_utf16le_to_latin1_with_errors(const char16_t *input,
                                              size_t length, char *output);
xtext_result
xtext_convert_utf16be_to_latin1_with_errors(const char16_t *input,
                                              size_t length, char *output);

xtext_result xtext_convert_utf16_to_utf8_with_errors(const char16_t *input,
                                                         size_t length,
                                                         char *output);
xtext_result
xtext_convert_utf16le_to_utf8_with_errors(const char16_t *input,
                                            size_t length, char *output);
xtext_result
xtext_convert_utf16be_to_utf8_with_errors(const char16_t *input,
                                            size_t length, char *output);

size_t xtext_convert_valid_utf16_to_utf8(const char16_t *input, size_t length,
                                           char *output);
size_t xtext_convert_valid_utf16_to_latin1(const char16_t *input,
                                             size_t length, char *output);
size_t xtext_convert_valid_utf16le_to_latin1(const char16_t *input,
                                               size_t length, char *output);
size_t xtext_convert_valid_utf16be_to_latin1(const char16_t *input,
                                               size_t length, char *output);

size_t xtext_convert_valid_utf16le_to_utf8(const char16_t *input,
                                             size_t length, char *output);
size_t xtext_convert_valid_utf16be_to_utf8(const char16_t *input,
                                             size_t length, char *output);

/* UTF-16 <-> UTF-32 conversions */
size_t xtext_convert_utf16_to_utf32(const char16_t *input, size_t length,
                                      char32_t *output);
size_t xtext_convert_utf16le_to_utf32(const char16_t *input, size_t length,
                                        char32_t *output);
size_t xtext_convert_utf16be_to_utf32(const char16_t *input, size_t length,
                                        char32_t *output);
xtext_result xtext_convert_utf16_to_utf32_with_errors(const char16_t *input,
                                                          size_t length,
                                                          char32_t *output);
xtext_result
xtext_convert_utf16le_to_utf32_with_errors(const char16_t *input,
                                             size_t length, char32_t *output);
xtext_result
xtext_convert_utf16be_to_utf32_with_errors(const char16_t *input,
                                             size_t length, char32_t *output);

/* Valid UTF-16 conversions */
size_t xtext_convert_valid_utf16_to_utf32(const char16_t *input,
                                            size_t length, char32_t *output);
size_t xtext_convert_valid_utf16le_to_utf32(const char16_t *input,
                                              size_t length, char32_t *output);
size_t xtext_convert_valid_utf16be_to_utf32(const char16_t *input,
                                              size_t length, char32_t *output);

/* UTF-32 -> ... conversions */
size_t xtext_convert_utf32_to_utf8(const char32_t *input, size_t length,
                                     char *output);
xtext_result xtext_convert_utf32_to_utf8_with_errors(const char32_t *input,
                                                         size_t length,
                                                         char *output);
size_t xtext_convert_valid_utf32_to_utf8(const char32_t *input, size_t length,
                                           char *output);

size_t xtext_convert_utf32_to_utf16(const char32_t *input, size_t length,
                                      char16_t *output);
size_t xtext_convert_utf32_to_utf16le(const char32_t *input, size_t length,
                                        char16_t *output);
size_t xtext_convert_utf32_to_utf16be(const char32_t *input, size_t length,
                                        char16_t *output);
xtext_result
xtext_convert_utf32_to_latin1_with_errors(const char32_t *input,
                                            size_t length, char *output);

/* --- Find helpers --- */
const char *xtext_find(const char *start, const char *end, char character);
const char16_t *xtext_find_utf16(const char16_t *start, const char16_t *end,
                                   char16_t character);

/* --- Base64 enums and helpers --- */
typedef enum xtext_base64_options {
  XTEXT_SIMDUTF_BASE64_DEFAULT = 0,
  XTEXT_SIMDUTF_BASE64_URL = 1,
  XTEXT_SIMDUTF_BASE64_DEFAULT_NO_PADDING = 2,
  XTEXT_SIMDUTF_BASE64_URL_WITH_PADDING = 3,
  XTEXT_SIMDUTF_BASE64_DEFAULT_ACCEPT_GARBAGE = 4,
  XTEXT_SIMDUTF_BASE64_URL_ACCEPT_GARBAGE = 5,
  XTEXT_SIMDUTF_BASE64_DEFAULT_OR_URL = 8,
  XTEXT_SIMDUTF_BASE64_DEFAULT_OR_URL_ACCEPT_GARBAGE = 12
} xtext_base64_options;

typedef enum xtext_last_chunk_handling_options {
  XTEXT_SIMDUTF_LAST_CHUNK_LOOSE = 0,
  XTEXT_SIMDUTF_LAST_CHUNK_STRICT = 1,
  XTEXT_SIMDUTF_LAST_CHUNK_STOP_BEFORE_PARTIAL = 2,
  XTEXT_SIMDUTF_LAST_CHUNK_ONLY_FULL_CHUNKS = 3
} xtext_last_chunk_handling_options;

/* maximal binary length estimators */
size_t xtext_maximal_binary_length_from_base64(const char *input,
                                                 size_t length);
size_t xtext_maximal_binary_length_from_base64_utf16(const char16_t *input,
                                                       size_t length);

/* base64 decoding/encoding */
xtext_result xtext_base64_to_binary(
    const char *input, size_t length, char *output,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options);
xtext_result xtext_base64_to_binary_utf16(
    const char16_t *input, size_t length, char *output,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options);

size_t xtext_base64_length_from_binary(size_t length,
                                         xtext_base64_options options);
size_t xtext_base64_length_from_binary_with_lines(
    size_t length, xtext_base64_options options, size_t line_length);

size_t xtext_binary_to_base64(const char *input, size_t length, char *output,
                                xtext_base64_options options);
size_t xtext_binary_to_base64_with_lines(const char *input, size_t length,
                                           char *output, size_t line_length,
                                           xtext_base64_options options);

/* safe decoding that provides an in/out outlen parameter */
xtext_result xtext_base64_to_binary_safe(
    const char *input, size_t length, char *output, size_t *outlen,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options,
    bool decode_up_to_bad_char);
xtext_result xtext_base64_to_binary_safe_utf16(
    const char16_t *input, size_t length, char *output, size_t *outlen,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options,
    bool decode_up_to_bad_char);

/* detailed decoding returning input_count and output_count */
xtext_full_result xtext_base64_to_binary_details(
    const char *input, size_t length, char *output,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options);
xtext_full_result xtext_base64_to_binary_details_utf16(
    const char16_t *input, size_t length, char *output,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options);

/* single-character base64 validation */
bool xtext_base64_valid(char input, xtext_base64_options options);
bool xtext_base64_valid_utf16(char16_t input, xtext_base64_options options);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XTEXT_SIMDUTF_C_H */
