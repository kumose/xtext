#include <xtext/simdutf_c.h>
#include <xtext/simdutf/implementation.h>

static xtext_result to_c_result(const xtext::result &r) {
  xtext_result out;
  out.error = static_cast<xtext_error_code>(r.error);
  out.count = r.count;
  return out;
}

/* The C wrapper depends on the library features. Only expose the C API
   when all relevant feature is enabled. This helps the
   single-header generator to omit the C wrapper when features are
   disabled. */
// clang-format off
#if XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_UTF32 && XTEXT_SIMDUTF_FEATURE_LATIN1 && XTEXT_SIMDUTF_FEATURE_ASCII && XTEXT_SIMDUTF_FEATURE_BASE64 && XTEXT_SIMDUTF_FEATURE_DETECT_ENCODING
// clang-format on
extern "C" {

bool xtext_validate_utf8(const char *buf, size_t len) {
  return xtext::validate_utf8(buf, len);
}

xtext_result xtext_validate_utf8_with_errors(const char *buf, size_t len) {
  return to_c_result(xtext::validate_utf8_with_errors(buf, len));
}

xtext_encoding_type xtext_autodetect_encoding(const char *input,
                                                  size_t length) {
  return static_cast<xtext_encoding_type>(
      xtext::autodetect_encoding(input, length));
}

int xtext_detect_encodings(const char *input, size_t length) {
  return xtext::detect_encodings(input, length);
}

bool xtext_validate_ascii(const char *buf, size_t len) {
  return xtext::validate_ascii(buf, len);
}
xtext_result xtext_validate_ascii_with_errors(const char *buf, size_t len) {
  return to_c_result(xtext::validate_ascii_with_errors(buf, len));
}

bool xtext_validate_utf16_as_ascii(const char16_t *buf, size_t len) {
  return xtext::validate_utf16_as_ascii(buf, len);
}
bool xtext_validate_utf16be_as_ascii(const char16_t *buf, size_t len) {
  return xtext::validate_utf16be_as_ascii(buf, len);
}
bool xtext_validate_utf16le_as_ascii(const char16_t *buf, size_t len) {
  return xtext::validate_utf16le_as_ascii(buf, len);
}

bool xtext_validate_utf16(const char16_t *buf, size_t len) {
  return xtext::validate_utf16(buf, len);
}
bool xtext_validate_utf16le(const char16_t *buf, size_t len) {
  return xtext::validate_utf16le(buf, len);
}
bool xtext_validate_utf16be(const char16_t *buf, size_t len) {
  return xtext::validate_utf16be(buf, len);
}
xtext_result xtext_validate_utf16_with_errors(const char16_t *buf,
                                                  size_t len) {
  return to_c_result(xtext::validate_utf16_with_errors(buf, len));
}
xtext_result xtext_validate_utf16le_with_errors(const char16_t *buf,
                                                    size_t len) {
  return to_c_result(xtext::validate_utf16le_with_errors(buf, len));
}
xtext_result xtext_validate_utf16be_with_errors(const char16_t *buf,
                                                    size_t len) {
  return to_c_result(xtext::validate_utf16be_with_errors(buf, len));
}

bool xtext_validate_utf32(const char32_t *buf, size_t len) {
  return xtext::validate_utf32(buf, len);
}
xtext_result xtext_validate_utf32_with_errors(const char32_t *buf,
                                                  size_t len) {
  return to_c_result(xtext::validate_utf32_with_errors(buf, len));
}

void xtext_to_well_formed_utf16le(const char16_t *input, size_t len,
                                    char16_t *output) {
  xtext::to_well_formed_utf16le(input, len, output);
}
void xtext_to_well_formed_utf16be(const char16_t *input, size_t len,
                                    char16_t *output) {
  xtext::to_well_formed_utf16be(input, len, output);
}
void xtext_to_well_formed_utf16(const char16_t *input, size_t len,
                                  char16_t *output) {
  xtext::to_well_formed_utf16(input, len, output);
}

size_t xtext_count_utf16(const char16_t *input, size_t length) {
  return xtext::count_utf16(input, length);
}
size_t xtext_count_utf16le(const char16_t *input, size_t length) {
  return xtext::count_utf16le(input, length);
}
size_t xtext_count_utf16be(const char16_t *input, size_t length) {
  return xtext::count_utf16be(input, length);
}
size_t xtext_count_utf8(const char *input, size_t length) {
  return xtext::count_utf8(input, length);
}

size_t xtext_utf8_length_from_latin1(const char *input, size_t length) {
  return xtext::utf8_length_from_latin1(input, length);
}
size_t xtext_latin1_length_from_utf8(const char *input, size_t length) {
  return xtext::latin1_length_from_utf8(input, length);
}
size_t xtext_latin1_length_from_utf16(size_t length) {
  return xtext::latin1_length_from_utf16(length);
}
size_t xtext_latin1_length_from_utf32(size_t length) {
  return xtext::latin1_length_from_utf32(length);
}
size_t xtext_utf16_length_from_utf8(const char *input, size_t length) {
  return xtext::utf16_length_from_utf8(input, length);
}
size_t xtext_utf32_length_from_utf8(const char *input, size_t length) {
  return xtext::utf32_length_from_utf8(input, length);
}
size_t xtext_utf8_length_from_utf16(const char16_t *input, size_t length) {
  return xtext::utf8_length_from_utf16(input, length);
}
size_t xtext_utf8_length_from_utf32(const char32_t *input, size_t length) {
  return xtext::utf8_length_from_utf32(input, length);
}
xtext_result
xtext_utf8_length_from_utf16_with_replacement(const char16_t *input,
                                                size_t length) {
  return to_c_result(
      xtext::utf8_length_from_utf16_with_replacement(input, length));
}
size_t xtext_utf8_length_from_utf16le(const char16_t *input, size_t length) {
  return xtext::utf8_length_from_utf16le(input, length);
}
size_t xtext_utf8_length_from_utf16be(const char16_t *input, size_t length) {
  return xtext::utf8_length_from_utf16be(input, length);
}
xtext_result
xtext_utf8_length_from_utf16le_with_replacement(const char16_t *input,
                                                  size_t length) {
  return to_c_result(
      xtext::utf8_length_from_utf16le_with_replacement(input, length));
}
xtext_result
xtext_utf8_length_from_utf16be_with_replacement(const char16_t *input,
                                                  size_t length) {
  return to_c_result(
      xtext::utf8_length_from_utf16be_with_replacement(input, length));
}

/* Conversions: latin1 <-> utf8, utf8 <-> utf16/utf32, utf16 <-> utf8, etc. */
size_t xtext_convert_latin1_to_utf8(const char *input, size_t length,
                                      char *output) {
  return xtext::convert_latin1_to_utf8(input, length, output);
}

size_t xtext_convert_latin1_to_utf8_safe(const char *input, size_t length,
                                           char *output, size_t utf8_len) {
  return xtext::convert_latin1_to_utf8_safe(input, length, output, utf8_len);
}
size_t xtext_convert_latin1_to_utf16le(const char *input, size_t length,
                                         char16_t *output) {
  return xtext::convert_latin1_to_utf16le(input, length, output);
}
size_t xtext_convert_latin1_to_utf16be(const char *input, size_t length,
                                         char16_t *output) {
  return xtext::convert_latin1_to_utf16be(input, length, output);
}
size_t xtext_convert_latin1_to_utf16(const char *input, size_t length,
                                       char16_t *output) {
  return xtext::convert_latin1_to_utf16(input, length, output);
}
size_t xtext_convert_latin1_to_utf32(const char *input, size_t length,
                                       char32_t *output) {
  return xtext::convert_latin1_to_utf32(input, length, output);
}

size_t xtext_convert_utf8_to_latin1(const char *input, size_t length,
                                      char *output) {
  return xtext::convert_utf8_to_latin1(input, length, output);
}
size_t xtext_convert_utf8_to_utf16le(const char *input, size_t length,
                                       char16_t *output) {
  return xtext::convert_utf8_to_utf16le(input, length, output);
}
size_t xtext_convert_utf8_to_utf16(const char *input, size_t length,
                                     char16_t *output) {
  return xtext::convert_utf8_to_utf16(input, length, output);
}
size_t xtext_convert_utf8_to_utf16be(const char *input, size_t length,
                                       char16_t *output) {
  return xtext::convert_utf8_to_utf16be(input, length, output);
}
size_t xtext_convert_utf8_to_utf32(const char *input, size_t length,
                                     char32_t *output) {
  return xtext::convert_utf8_to_utf32(input, length, output);
}
xtext_result xtext_convert_utf8_to_latin1_with_errors(const char *input,
                                                          size_t length,
                                                          char *output) {
  return to_c_result(
      xtext::convert_utf8_to_latin1_with_errors(input, length, output));
}
xtext_result xtext_convert_utf8_to_utf16_with_errors(const char *input,
                                                         size_t length,
                                                         char16_t *output) {
  return to_c_result(
      xtext::convert_utf8_to_utf16_with_errors(input, length, output));
}
xtext_result xtext_convert_utf8_to_utf16le_with_errors(const char *input,
                                                           size_t length,
                                                           char16_t *output) {
  return to_c_result(
      xtext::convert_utf8_to_utf16le_with_errors(input, length, output));
}
xtext_result xtext_convert_utf8_to_utf16be_with_errors(const char *input,
                                                           size_t length,
                                                           char16_t *output) {
  return to_c_result(
      xtext::convert_utf8_to_utf16be_with_errors(input, length, output));
}
xtext_result xtext_convert_utf8_to_utf32_with_errors(const char *input,
                                                         size_t length,
                                                         char32_t *output) {
  return to_c_result(
      xtext::convert_utf8_to_utf32_with_errors(input, length, output));
}

/* Conversions assuming valid input */
size_t xtext_convert_valid_utf8_to_latin1(const char *input, size_t length,
                                            char *output) {
  return xtext::convert_valid_utf8_to_latin1(input, length, output);
}
size_t xtext_convert_valid_utf8_to_utf16le(const char *input, size_t length,
                                             char16_t *output) {
  return xtext::convert_valid_utf8_to_utf16le(input, length, output);
}
size_t xtext_convert_valid_utf8_to_utf16be(const char *input, size_t length,
                                             char16_t *output) {
  return xtext::convert_valid_utf8_to_utf16be(input, length, output);
}
size_t xtext_convert_valid_utf8_to_utf32(const char *input, size_t length,
                                           char32_t *output) {
  return xtext::convert_valid_utf8_to_utf32(input, length, output);
}

/* UTF-16 -> UTF-8 and related conversions */
size_t xtext_convert_utf16_to_utf8(const char16_t *input, size_t length,
                                     char *output) {
  return xtext::convert_utf16_to_utf8(input, length, output);
}
size_t xtext_convert_utf16_to_utf8_safe(const char16_t *input, size_t length,
                                          char *output, size_t utf8_len) {
  return xtext::convert_utf16_to_utf8_safe(input, length, output, utf8_len);
}
size_t xtext_convert_utf16_to_latin1(const char16_t *input, size_t length,
                                       char *output) {
  return xtext::convert_utf16_to_latin1(input, length, output);
}
size_t xtext_convert_utf16le_to_latin1(const char16_t *input, size_t length,
                                         char *output) {
  return xtext::convert_utf16le_to_latin1(input, length, output);
}
size_t xtext_convert_utf16be_to_latin1(const char16_t *input, size_t length,
                                         char *output) {
  return xtext::convert_utf16be_to_latin1(input, length, output);
}
xtext_result
xtext_convert_utf16_to_latin1_with_errors(const char16_t *input,
                                            size_t length, char *output) {
  return to_c_result(
      xtext::convert_utf16_to_latin1_with_errors(input, length, output));
}
xtext_result
xtext_convert_utf16le_to_latin1_with_errors(const char16_t *input,
                                              size_t length, char *output) {
  return to_c_result(
      xtext::convert_utf16le_to_latin1_with_errors(input, length, output));
}
xtext_result
xtext_convert_utf16be_to_latin1_with_errors(const char16_t *input,
                                              size_t length, char *output) {
  return to_c_result(
      xtext::convert_utf16be_to_latin1_with_errors(input, length, output));
}

xtext_result xtext_convert_utf16_to_utf8_with_errors(const char16_t *input,
                                                         size_t length,
                                                         char *output) {
  return to_c_result(
      xtext::convert_utf16_to_utf8_with_errors(input, length, output));
}
xtext_result
xtext_convert_utf16le_to_utf8_with_errors(const char16_t *input,
                                            size_t length, char *output) {
  return to_c_result(
      xtext::convert_utf16le_to_utf8_with_errors(input, length, output));
}
xtext_result
xtext_convert_utf16be_to_utf8_with_errors(const char16_t *input,
                                            size_t length, char *output) {
  return to_c_result(
      xtext::convert_utf16be_to_utf8_with_errors(input, length, output));
}

size_t xtext_convert_utf16le_to_utf8(const char16_t *input, size_t length,
                                       char *output) {
  return xtext::convert_utf16le_to_utf8(input, length, output);
}
size_t xtext_convert_utf16be_to_utf8(const char16_t *input, size_t length,
                                       char *output) {
  return xtext::convert_utf16be_to_utf8(input, length, output);
}

size_t xtext_convert_valid_utf16_to_utf8(const char16_t *input, size_t length,
                                           char *output) {
  return xtext::convert_valid_utf16_to_utf8(input, length, output);
}
size_t xtext_convert_valid_utf16_to_latin1(const char16_t *input,
                                             size_t length, char *output) {
  return xtext::convert_valid_utf16_to_latin1(input, length, output);
}
size_t xtext_convert_valid_utf16le_to_latin1(const char16_t *input,
                                               size_t length, char *output) {
  return xtext::convert_valid_utf16le_to_latin1(input, length, output);
}
size_t xtext_convert_valid_utf16be_to_latin1(const char16_t *input,
                                               size_t length, char *output) {
  return xtext::convert_valid_utf16be_to_latin1(input, length, output);
}

size_t xtext_convert_valid_utf16le_to_utf8(const char16_t *input,
                                             size_t length, char *output) {
  return xtext::convert_valid_utf16le_to_utf8(input, length, output);
}
size_t xtext_convert_valid_utf16be_to_utf8(const char16_t *input,
                                             size_t length, char *output) {
  return xtext::convert_valid_utf16be_to_utf8(input, length, output);
}

/* UTF-16 <-> UTF-32 conversions */
size_t xtext_convert_utf16_to_utf32(const char16_t *input, size_t length,
                                      char32_t *output) {
  return xtext::convert_utf16_to_utf32(input, length, output);
}
size_t xtext_convert_utf16le_to_utf32(const char16_t *input, size_t length,
                                        char32_t *output) {
  return xtext::convert_utf16le_to_utf32(input, length, output);
}
size_t xtext_convert_utf16be_to_utf32(const char16_t *input, size_t length,
                                        char32_t *output) {
  return xtext::convert_utf16be_to_utf32(input, length, output);
}
xtext_result xtext_convert_utf16_to_utf32_with_errors(const char16_t *input,
                                                          size_t length,
                                                          char32_t *output) {
  return to_c_result(
      xtext::convert_utf16_to_utf32_with_errors(input, length, output));
}
xtext_result
xtext_convert_utf16le_to_utf32_with_errors(const char16_t *input,
                                             size_t length, char32_t *output) {
  return to_c_result(
      xtext::convert_utf16le_to_utf32_with_errors(input, length, output));
}
xtext_result
xtext_convert_utf16be_to_utf32_with_errors(const char16_t *input,
                                             size_t length, char32_t *output) {
  return to_c_result(
      xtext::convert_utf16be_to_utf32_with_errors(input, length, output));
}

/* Valid UTF-16 conversions */
size_t xtext_convert_valid_utf16_to_utf32(const char16_t *input,
                                            size_t length, char32_t *output) {
  return xtext::convert_valid_utf16_to_utf32(input, length, output);
}
size_t xtext_convert_valid_utf16le_to_utf32(const char16_t *input,
                                              size_t length, char32_t *output) {
  return xtext::convert_valid_utf16le_to_utf32(input, length, output);
}
size_t xtext_convert_valid_utf16be_to_utf32(const char16_t *input,
                                              size_t length, char32_t *output) {
  return xtext::convert_valid_utf16be_to_utf32(input, length, output);
}

/* UTF-32 -> ... conversions */
size_t xtext_convert_utf32_to_utf8(const char32_t *input, size_t length,
                                     char *output) {
  return xtext::convert_utf32_to_utf8(input, length, output);
}
xtext_result xtext_convert_utf32_to_utf8_with_errors(const char32_t *input,
                                                         size_t length,
                                                         char *output) {
  return to_c_result(
      xtext::convert_utf32_to_utf8_with_errors(input, length, output));
}
size_t xtext_convert_valid_utf32_to_utf8(const char32_t *input, size_t length,
                                           char *output) {
  return xtext::convert_valid_utf32_to_utf8(input, length, output);
}

size_t xtext_convert_utf32_to_utf16(const char32_t *input, size_t length,
                                      char16_t *output) {
  return xtext::convert_utf32_to_utf16(input, length, output);
}
size_t xtext_convert_utf32_to_utf16le(const char32_t *input, size_t length,
                                        char16_t *output) {
  return xtext::convert_utf32_to_utf16le(input, length, output);
}
size_t xtext_convert_utf32_to_utf16be(const char32_t *input, size_t length,
                                        char16_t *output) {
  return xtext::convert_utf32_to_utf16be(input, length, output);
}
xtext_result
xtext_convert_utf32_to_latin1_with_errors(const char32_t *input,
                                            size_t length, char *output) {
  return to_c_result(
      xtext::convert_utf32_to_latin1_with_errors(input, length, output));
}

/* --- find helpers --- */
const char *xtext_find(const char *start, const char *end, char character) {
  return xtext::find(start, end, character);
}
const char16_t *xtext_find_utf16(const char16_t *start, const char16_t *end,
                                   char16_t character) {
  return xtext::find(start, end, character);
}

/* --- base64 helpers --- */
size_t xtext_maximal_binary_length_from_base64(const char *input,
                                                 size_t length) {
  return xtext::maximal_binary_length_from_base64(input, length);
}
size_t xtext_maximal_binary_length_from_base64_utf16(const char16_t *input,
                                                       size_t length) {
  return xtext::maximal_binary_length_from_base64(input, length);
}

xtext_result xtext_base64_to_binary(
    const char *input, size_t length, char *output,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options) {
  return to_c_result(xtext::base64_to_binary(
      input, length, output, static_cast<xtext::base64_options>(options),
      static_cast<xtext::last_chunk_handling_options>(last_chunk_options)));
}
xtext_result xtext_base64_to_binary_utf16(
    const char16_t *input, size_t length, char *output,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options) {
  return to_c_result(xtext::base64_to_binary(
      input, length, output, static_cast<xtext::base64_options>(options),
      static_cast<xtext::last_chunk_handling_options>(last_chunk_options)));
}

size_t xtext_base64_length_from_binary(size_t length,
                                         xtext_base64_options options) {
  return xtext::base64_length_from_binary(
      length, static_cast<xtext::base64_options>(options));
}
size_t xtext_base64_length_from_binary_with_lines(
    size_t length, xtext_base64_options options, size_t line_length) {
  return xtext::base64_length_from_binary_with_lines(
      length, static_cast<xtext::base64_options>(options), line_length);
}

size_t xtext_binary_to_base64(const char *input, size_t length, char *output,
                                xtext_base64_options options) {
  return xtext::binary_to_base64(
      input, length, output, static_cast<xtext::base64_options>(options));
}
size_t xtext_binary_to_base64_with_lines(const char *input, size_t length,
                                           char *output, size_t line_length,
                                           xtext_base64_options options) {
  return xtext::binary_to_base64_with_lines(
      input, length, output, line_length,
      static_cast<xtext::base64_options>(options));
}

xtext_result xtext_base64_to_binary_safe(
    const char *input, size_t length, char *output, size_t *outlen,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options,
    bool decode_up_to_bad_char) {
  size_t local_out = outlen ? *outlen : 0;
  xtext::result r = xtext::base64_to_binary_safe(
      input, length, output, local_out,
      static_cast<xtext::base64_options>(options),
      static_cast<xtext::last_chunk_handling_options>(last_chunk_options),
      decode_up_to_bad_char);
  if (outlen)
    *outlen = local_out;
  return to_c_result(r);
}
xtext_result xtext_base64_to_binary_safe_utf16(
    const char16_t *input, size_t length, char *output, size_t *outlen,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options,
    bool decode_up_to_bad_char) {
  size_t local_out = outlen ? *outlen : 0;
  xtext::result r = xtext::base64_to_binary_safe(
      input, length, output, local_out,
      static_cast<xtext::base64_options>(options),
      static_cast<xtext::last_chunk_handling_options>(last_chunk_options),
      decode_up_to_bad_char);
  if (outlen)
    *outlen = local_out;
  return to_c_result(r);
}

static xtext_full_result to_c_full_result(const xtext::full_result &r) {
  xtext_full_result out;
  out.error = static_cast<xtext_error_code>(r.error);
  out.input_count = r.input_count;
  out.output_count = r.output_count;
  return out;
}

xtext_full_result xtext_base64_to_binary_details(
    const char *input, size_t length, char *output,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options) {
  return to_c_full_result(xtext::base64_to_binary_details(
      input, length, output, static_cast<xtext::base64_options>(options),
      static_cast<xtext::last_chunk_handling_options>(last_chunk_options)));
}
xtext_full_result xtext_base64_to_binary_details_utf16(
    const char16_t *input, size_t length, char *output,
    xtext_base64_options options,
    xtext_last_chunk_handling_options last_chunk_options) {
  return to_c_full_result(xtext::base64_to_binary_details(
      input, length, output, static_cast<xtext::base64_options>(options),
      static_cast<xtext::last_chunk_handling_options>(last_chunk_options)));
}

bool xtext_base64_valid(char input, xtext_base64_options options) {
  return xtext::base64_valid(input,
                               static_cast<xtext::base64_options>(options));
}
bool xtext_base64_valid_utf16(char16_t input,
                                xtext_base64_options options) {
  return xtext::base64_valid(input,
                               static_cast<xtext::base64_options>(options));
}

} // extern "C"
// clang-format off
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_UTF32 && XTEXT_SIMDUTF_FEATURE_LATIN1 && XTEXT_SIMDUTF_FEATURE_ASCII && XTEXT_SIMDUTF_FEATURE_BASE64 && XTEXT_SIMDUTF_FEATURE_DETECT_ENCODING
// clang-format on
