#ifndef XTEXT_SIMDUTF_FALLBACK_H
#define XTEXT_SIMDUTF_FALLBACK_H

#include <xtext/simdutf/portability.h>

// Note that fallback.h is always imported last.

// Default Fallback to on unless a builtin implementation has already been
// selected.
#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK
  #if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ARM64 || XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ICELAKE ||        \
      XTEXT_SIMDUTF_CAN_ALWAYS_RUN_HASWELL || XTEXT_SIMDUTF_CAN_ALWAYS_RUN_WESTMERE ||     \
      XTEXT_SIMDUTF_CAN_ALWAYS_RUN_PPC64 || XTEXT_SIMDUTF_CAN_ALWAYS_RUN_RVV ||            \
      XTEXT_SIMDUTF_CAN_ALWAYS_RUN_LSX || XTEXT_SIMDUTF_CAN_ALWAYS_RUN_LASX
    #define XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK 0
  #else
    #define XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK 1
  #endif
#endif

#define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_FALLBACK (XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK)

#if XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK

namespace xtext {
/**
 * Fallback implementation (runs on any machine).
 */
namespace fallback {} // namespace fallback
} // namespace xtext

  #include <xtext/simdutf/fallback/h/implementation.h>

  #include <xtext/simdutf/fallback/h/begin.h>

  // Declarations
  #include <xtext/simdutf/fallback/h/bitmanipulation.h>

  #include <xtext/simdutf/fallback/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK
#endif // XTEXT_SIMDUTF_FALLBACK_H
