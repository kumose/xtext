#include <xtext/simdutf.h>

#include <xtext/simdutf/encoding_types.cc>
#include <xtext/simdutf/error.cc>
// The large tables should be included once and they
// should not depend on a kernel.
#include <xtext/simdutf/tables/utf8_to_utf16_tables.h>
#include <xtext/simdutf/tables/utf16_to_utf8_tables.h>
#include <xtext/simdutf/tables/utf32_to_utf16_tables.h>
// End of tables.

// Implementations: they need to be setup before including
// scalar/* code, as the scalar code is sometimes enabled
// only for peculiar build targets.

// The best choice should always come first!
#ifndef XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
XTEXT_SIMDUTF_DISABLE_UNUSED_WARNING
#endif
#include <xtext/simdutf/arm64.h>
#include <xtext/simdutf/icelake.h>
#include <xtext/simdutf/haswell.h>
#include <xtext/simdutf/westmere.h>
#include <xtext/simdutf/ppc64.h>
#include <xtext/simdutf/rvv.h>
#include <xtext/simdutf/lasx.h>
#include <xtext/simdutf/lsx.h>
#include <xtext/simdutf/fallback.h> // have it always last.
#ifndef XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
XTEXT_SIMDUTF_POP_DISABLE_WARNINGS
#endif

// The scalar routines should be included once.
#include <xtext/simdutf/scalar/swap_bytes.h>
#if XTEXT_SIMDUTF_FEATURE_ASCII
  #include <xtext/simdutf/scalar/ascii.h>
#endif // XTEXT_SIMDUTF_FEATURE_ASCII
#if XTEXT_SIMDUTF_FEATURE_UTF8 || XTEXT_SIMDUTF_FEATURE_DETECT_ENCODING
  #include <xtext/simdutf/scalar/utf8.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 || XTEXT_SIMDUTF_FEATURE_DETECT_ENCODING
#if XTEXT_SIMDUTF_FEATURE_UTF16 || XTEXT_SIMDUTF_FEATURE_DETECT_ENCODING ||                \
    (XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_LATIN1)
  #include <xtext/simdutf/scalar/utf16.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF16 || XTEXT_SIMDUTF_FEATURE_DETECT_ENCODING ||
       // (XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_LATIN1)
#if XTEXT_SIMDUTF_FEATURE_UTF32 || XTEXT_SIMDUTF_FEATURE_DETECT_ENCODING
  #include <xtext/simdutf/scalar/utf32.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF32 || XTEXT_SIMDUTF_FEATURE_DETECT_ENCODING
#if XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/latin1.h>
#endif // XTEXT_SIMDUTF_FEATURE_LATIN1
#if XTEXT_SIMDUTF_FEATURE_BASE64
  #include <xtext/simdutf/scalar/base64.h>
#endif // XTEXT_SIMDUTF_FEATURE_BASE64

#if XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_UTF32
  #include <xtext/simdutf/scalar/utf32_to_utf8/valid_utf32_to_utf8.h>
  #include <xtext/simdutf/scalar/utf32_to_utf8/utf32_to_utf8.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_UTF32

#if XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_UTF32
  #include <xtext/simdutf/scalar/utf32_to_utf16/valid_utf32_to_utf16.h>
  #include <xtext/simdutf/scalar/utf32_to_utf16/utf32_to_utf16.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_UTF32

#if XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_UTF16
  #include <xtext/simdutf/scalar/utf16_to_utf8/valid_utf16_to_utf8.h>
  #include <xtext/simdutf/scalar/utf16_to_utf8/utf16_to_utf8.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_UTF16

#if XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_UTF32
  #include <xtext/simdutf/scalar/utf16_to_utf32/valid_utf16_to_utf32.h>
  #include <xtext/simdutf/scalar/utf16_to_utf32/utf16_to_utf32.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_UTF32

#if XTEXT_SIMDUTF_FEATURE_UTF8 &&                                                    \
    (XTEXT_SIMDUTF_FEATURE_UTF16 || XTEXT_SIMDUTF_FEATURE_UTF32 || XTEXT_SIMDUTF_FEATURE_LATIN1)
  #include <xtext/simdutf/scalar/utf8_to_utf16/valid_utf8_to_utf16.h>
  #include <xtext/simdutf/scalar/utf8_to_utf16/utf8_to_utf16.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 && (XTEXT_SIMDUTF_FEATURE_UTF16 ||
       // XTEXT_SIMDUTF_FEATURE_UTF32 || XTEXT_SIMDUTF_FEATURE_LATIN1)

#if XTEXT_SIMDUTF_FEATURE_UTF8 || XTEXT_SIMDUTF_FEATURE_UTF32
  #include <xtext/simdutf/scalar/utf8_to_utf32/valid_utf8_to_utf32.h>
  #include <xtext/simdutf/scalar/utf8_to_utf32/utf8_to_utf32.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 || XTEXT_SIMDUTF_FEATURE_UTF32

#if XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/latin1_to_utf8/latin1_to_utf8.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_LATIN1
#if XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/latin1_to_utf16/latin1_to_utf16.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_LATIN1
#if XTEXT_SIMDUTF_FEATURE_UTF32 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/latin1_to_utf32/latin1_to_utf32.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF32 && XTEXT_SIMDUTF_FEATURE_LATIN1

#if XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/utf8_to_latin1/utf8_to_latin1.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_LATIN1
#if XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/utf16_to_latin1/utf16_to_latin1.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_LATIN1
#if XTEXT_SIMDUTF_FEATURE_UTF32 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/utf32_to_latin1/utf32_to_latin1.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF32 && XTEXT_SIMDUTF_FEATURE_LATIN1

#if XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/utf8_to_latin1/valid_utf8_to_latin1.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF8 && XTEXT_SIMDUTF_FEATURE_LATIN1
#if XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/utf16_to_latin1/valid_utf16_to_latin1.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF16 && XTEXT_SIMDUTF_FEATURE_LATIN1
#if XTEXT_SIMDUTF_FEATURE_UTF32 && XTEXT_SIMDUTF_FEATURE_LATIN1
  #include <xtext/simdutf/scalar/utf32_to_latin1/valid_utf32_to_latin1.h>
#endif // XTEXT_SIMDUTF_FEATURE_UTF32 && XTEXT_SIMDUTF_FEATURE_LATIN1

#include <xtext/simdutf/implementation.cc>

XTEXT_SIMDUTF_PUSH_DISABLE_WARNINGS
XTEXT_SIMDUTF_DISABLE_UNDESIRED_WARNINGS

#if XTEXT_SIMDUTF_IMPLEMENTATION_ARM64
  #include <xtext/simdutf/arm64/implementation.cc>
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK
  #include <xtext/simdutf/fallback/implementation.cc>
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_ICELAKE
  #include <xtext/simdutf/icelake/implementation.cc>
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_HASWELL
  #include <xtext/simdutf/haswell/implementation.cc>
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_PPC64
  #include <xtext/simdutf/ppc64/implementation.cc>
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_RVV
  #include <xtext/simdutf/rvv/implementation.cc>
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_WESTMERE
  #include <xtext/simdutf/westmere/implementation.cc>
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_LASX
  #include <xtext/simdutf/lasx/implementation.cc>
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_LSX
  #include <xtext/simdutf/lsx/implementation.cc>
#endif

#include "simdutf_c.cc"
XTEXT_SIMDUTF_POP_DISABLE_WARNINGS
