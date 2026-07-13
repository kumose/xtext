#ifndef XTEXT_SIMDUTF_WESTMERE_H
#define XTEXT_SIMDUTF_WESTMERE_H

#ifdef XTEXT_SIMDUTF_FALLBACK_H
  #error "westmere.h must be included before fallback.h"
#endif

#include <xtext/simdutf/portability.h>

// Default Westmere to on if this is x86-64, unless we'll always select Haswell.
#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_WESTMERE
  //
  // You do not want to set it to (XTEXT_SIMDUTF_IS_X86_64 &&
  // !XTEXT_SIMDUTF_REQUIRES_HASWELL) because you want to rely on runtime dispatch!
  //
  #if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ICELAKE || XTEXT_SIMDUTF_CAN_ALWAYS_RUN_HASWELL
    #define XTEXT_SIMDUTF_IMPLEMENTATION_WESTMERE 0
  #else
    #define XTEXT_SIMDUTF_IMPLEMENTATION_WESTMERE (XTEXT_SIMDUTF_IS_X86_64)
  #endif

#endif

#if (XTEXT_SIMDUTF_IMPLEMENTATION_WESTMERE && XTEXT_SIMDUTF_IS_X86_64 && __SSE4_2__)
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_WESTMERE 1
#else
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_WESTMERE 0
#endif

#if XTEXT_SIMDUTF_IMPLEMENTATION_WESTMERE

  #define XTEXT_SIMDUTF_TARGET_WESTMERE XTEXT_SIMDUTF_TARGET_REGION("sse4.2,popcnt")

namespace xtext {
/**
 * Implementation for Westmere (Intel SSE4.2).
 */
namespace westmere {} // namespace westmere
} // namespace xtext

  //
  // These two need to be included outside XTEXT_SIMDUTF_TARGET_REGION
  //
  #include <xtext/simdutf/westmere/h/implementation.h>
  #include <xtext/simdutf/westmere/h/intrinsics.h>

  //
  // The rest need to be inside the region
  //
  #include <xtext/simdutf/westmere/h/begin.h>

  // Declarations
  #include <xtext/simdutf/westmere/h/bitmanipulation.h>
  #include <xtext/simdutf/westmere/h/simd.h>

  #include <xtext/simdutf/westmere/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_WESTMERE
#endif // XTEXT_SIMDUTF_WESTMERE_COMMON_H
