#ifndef XTEXT_SIMDUTF_ARM64_H
#define XTEXT_SIMDUTF_ARM64_H

#ifdef XTEXT_SIMDUTF_FALLBACK_H
  #error "arm64.h must be included before fallback.h"
#endif

#include <xtext/simdutf/portability.h>

#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_ARM64
  #define XTEXT_SIMDUTF_IMPLEMENTATION_ARM64 (XTEXT_SIMDUTF_IS_ARM64)
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_ARM64 && XTEXT_SIMDUTF_IS_ARM64
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ARM64 1
#else
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ARM64 0
#endif

#include <xtext/simdutf/internal/isadetection.h>

#if XTEXT_SIMDUTF_IMPLEMENTATION_ARM64

namespace xtext {
/**
 * Implementation for NEON (ARMv8).
 */
namespace arm64 {} // namespace arm64
} // namespace xtext

  #include <xtext/simdutf/arm64/h/implementation.h>

  #include <xtext/simdutf/arm64/h/begin.h>

  // Declarations
  #include <xtext/simdutf/arm64/h/intrinsics.h>
  #include <xtext/simdutf/arm64/h/bitmanipulation.h>
  #include <xtext/simdutf/arm64/h/simd.h>

  #include <xtext/simdutf/arm64/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_ARM64

#endif // XTEXT_SIMDUTF_ARM64_H
