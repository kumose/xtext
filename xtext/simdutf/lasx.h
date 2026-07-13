#ifndef XTEXT_SIMDUTF_LASX_H
#define XTEXT_SIMDUTF_LASX_H

#ifdef XTEXT_SIMDUTF_FALLBACK_H
  #error "lasx.h must be included before fallback.h"
#endif

#include <xtext/simdutf/portability.h>

#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_LASX
  #define XTEXT_SIMDUTF_IMPLEMENTATION_LASX (XTEXT_SIMDUTF_IS_LSX)
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_LASX && XTEXT_SIMDUTF_IS_LASX
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_LASX 1
#else
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_LASX 0
#endif

#define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_FALLBACK (XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK)
#include <xtext/simdutf/internal/isadetection.h>

#if XTEXT_SIMDUTF_IMPLEMENTATION_LASX
  #define XTEXT_SIMDUTF_TARGET_LASX XTEXT_SIMDUTF_TARGET_REGION("lasx,lsx")

  // For runtime dispatching to work, we need the lsxintrin to appear
  // before we call XTEXT_SIMDUTF_TARGET_LASX. It is unclear why.
  #include <lsxintrin.h>

namespace xtext {
/**
 * Implementation for LoongArch ASX.
 */
namespace lasx {} // namespace lasx
} // namespace xtext

  #include <xtext/simdutf/lasx/h/implementation.h>

  #include <xtext/simdutf/lasx/h/begin.h>

  // Declarations
  #include <xtext/simdutf/lasx/h/intrinsics.h>
  #include <xtext/simdutf/lasx/h/bitmanipulation.h>
  #include <xtext/simdutf/lasx/h/simd.h>

  #include <xtext/simdutf/lasx/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_LASX

#endif // XTEXT_SIMDUTF_LASX_H
