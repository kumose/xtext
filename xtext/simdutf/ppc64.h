#ifndef XTEXT_SIMDUTF_PPC64_H
#define XTEXT_SIMDUTF_PPC64_H

#ifdef XTEXT_SIMDUTF_FALLBACK_H
  #error "ppc64.h must be included before fallback.h"
#endif

#include <xtext/simdutf/portability.h>

#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_PPC64
  #define XTEXT_SIMDUTF_IMPLEMENTATION_PPC64 (XTEXT_SIMDUTF_IS_PPC64)
#endif
#define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_PPC64                                           \
  XTEXT_SIMDUTF_IMPLEMENTATION_PPC64 &&XTEXT_SIMDUTF_IS_PPC64

#include <xtext/simdutf/internal/isadetection.h>

#if XTEXT_SIMDUTF_IMPLEMENTATION_PPC64

namespace xtext {
/**
 * Implementation for ALTIVEC (PPC64).
 */
namespace ppc64 {} // namespace ppc64
} // namespace xtext

  #include <xtext/simdutf/ppc64/h/implementation.h>

  #include <xtext/simdutf/ppc64/h/begin.h>

  // Declarations
  #include <xtext/simdutf/ppc64/h/intrinsics.h>
  #include <xtext/simdutf/ppc64/h/bitmanipulation.h>
  #include <xtext/simdutf/ppc64/h/simd.h>

  #include <xtext/simdutf/ppc64/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_PPC64

#endif // XTEXT_SIMDUTF_PPC64_H
