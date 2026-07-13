#include <xtext/simdutf.h>

#include "tests/simdutf/helpers/test.h"

#ifdef XTEXT_SIMDUTF_HAS_STD_TEXT_ENCODING

TEST(to_std_encoding_unspecified) {
  constexpr auto enc = xtext::to_std_encoding(xtext::unspecified);
  static_assert(enc.mib() == std::text_encoding::id::unknown,
                "unexpected unknown encoding");
}

TEST(from_std_encoding_unsupported) {
  constexpr std::text_encoding enc(std::text_encoding::id::ShiftJIS);
  constexpr auto result = xtext::from_std_encoding(enc);
  static_assert(result == xtext::unspecified,
                "unsupported encodings map to unspecified");
}

TEST(roundtrip_conversions) {
  static_assert(xtext::from_std_encoding(
                    xtext::to_std_encoding(xtext::UTF8)) == xtext::UTF8,
                "UTF8 roundtrip failed");
  static_assert(xtext::from_std_encoding(xtext::to_std_encoding(
                    xtext::UTF16_LE)) == xtext::UTF16_LE,
                "UTF16_LE roundtrip failed");
  static_assert(xtext::from_std_encoding(xtext::to_std_encoding(
                    xtext::UTF16_BE)) == xtext::UTF16_BE,
                "UTF16_BE roundtrip failed");
  static_assert(xtext::from_std_encoding(xtext::to_std_encoding(
                    xtext::UTF32_LE)) == xtext::UTF32_LE,
                "UTF32_LE roundtrip failed");
  static_assert(xtext::from_std_encoding(xtext::to_std_encoding(
                    xtext::UTF32_BE)) == xtext::UTF32_BE,
                "UTF32_BE roundtrip failed");
  static_assert(xtext::from_std_encoding(xtext::to_std_encoding(
                    xtext::Latin1)) == xtext::Latin1,
                "Latin1 roundtrip failed");
}

TEST(from_std_encoding_native_utf16) {
  constexpr std::text_encoding enc(std::text_encoding::id::UTF16);
  constexpr auto result = xtext::from_std_encoding_native(enc);
  #if XTEXT_SIMDUTF_IS_BIG_ENDIAN
  static_assert(result == xtext::UTF16_BE, "native UTF16 mapping mismatch");
  #else
  static_assert(result == xtext::UTF16_LE, "native UTF16 mapping mismatch");
  #endif
}

TEST(from_std_encoding_native_utf32) {
  constexpr std::text_encoding enc(std::text_encoding::id::UTF32);
  constexpr auto result = xtext::from_std_encoding_native(enc);
  #if XTEXT_SIMDUTF_IS_BIG_ENDIAN
  static_assert(result == xtext::UTF32_BE, "native UTF32 mapping mismatch");
  #else
  static_assert(result == xtext::UTF32_LE, "native UTF32 mapping mismatch");
  #endif
}

TEST(from_std_encoding_native_explicit_endian) {
  constexpr std::text_encoding enc_le(std::text_encoding::id::UTF16LE);
  constexpr std::text_encoding enc_be(std::text_encoding::id::UTF16BE);
  static_assert(xtext::from_std_encoding_native(enc_le) == xtext::UTF16_LE,
                "UTF16LE mapping mismatch");
  static_assert(xtext::from_std_encoding_native(enc_be) == xtext::UTF16_BE,
                "UTF16BE mapping mismatch");
}

#else
TEST(text_encoding_not_available) {
  // this test passes when c++26 not available
  ASSERT_TRUE(true);
}
#endif // XTEXT_SIMDUTF_HAS_STD_TEXT_ENCODING

TEST_MAIN
