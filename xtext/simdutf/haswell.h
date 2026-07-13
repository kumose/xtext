#ifndef XTEXT_SIMDUTF_HASWELL_H
#define XTEXT_SIMDUTF_HASWELL_H

#ifdef XTEXT_SIMDUTF_WESTMERE_H
  #error "haswell.h must be included before westmere.h"
#endif
#ifdef XTEXT_SIMDUTF_FALLBACK_H
  #error "haswell.h must be included before fallback.h"
#endif

#include <xtext/simdutf/portability.h>

// Default Haswell to on if this is x86-64. Even if we are not compiled for it,
// it could be selected at runtime.
#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_HASWELL
  //
  // You do not want to restrict it like so: XTEXT_SIMDUTF_IS_X86_64 && __AVX2__
  // because we want to rely on *runtime dispatch*.
  //
  #if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ICELAKE
    #define XTEXT_SIMDUTF_IMPLEMENTATION_HASWELL 0
  #else
    #define XTEXT_SIMDUTF_IMPLEMENTATION_HASWELL (XTEXT_SIMDUTF_IS_X86_64)
  #endif

#endif
// To see why  (__BMI__) && (__LZCNT__) are not part of this next line, see
// https://github.com/simdutf/simdutf/issues/1247
#if ((XTEXT_SIMDUTF_IMPLEMENTATION_HASWELL) && (XTEXT_SIMDUTF_IS_X86_64) && (__AVX2__))
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_HASWELL 1
#else
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_HASWELL 0
#endif

#if XTEXT_SIMDUTF_IMPLEMENTATION_HASWELL

  #define XTEXT_SIMDUTF_TARGET_HASWELL XTEXT_SIMDUTF_TARGET_REGION("avx2,bmi,lzcnt,popcnt")

namespace xtext {
/**
 * Implementation for Haswell (Intel AVX2).
 */
namespace haswell {} // namespace haswell
} // namespace xtext

  //
  // These two need to be included outside XTEXT_SIMDUTF_TARGET_REGION
  //
  #include <xtext/simdutf/haswell/h/implementation.h>
  #include <xtext/simdutf/haswell/h/intrinsics.h>

  //
  // The rest need to be inside the region
  //
  #include <xtext/simdutf/haswell/h/begin.h>
  // Declarations
  #include <xtext/simdutf/haswell/h/bitmanipulation.h>
  #include <xtext/simdutf/haswell/h/simd.h>

  #include <xtext/simdutf/haswell/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_HASWELL
#endif // XTEXT_SIMDUTF_HASWELL_COMMON_H
