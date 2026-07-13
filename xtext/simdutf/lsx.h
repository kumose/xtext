#ifndef XTEXT_SIMDUTF_LSX_H
#define XTEXT_SIMDUTF_LSX_H

#ifdef XTEXT_SIMDUTF_FALLBACK_H
  #error "lsx.h must be included before fallback.h"
#endif

#ifndef XTEXT_SIMDUTF_CAN_ALWAYS_RUN_LASX
  #error "lsx.h must be included after lasx.h"
#endif

#include <xtext/simdutf/portability.h>

#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_LSX
  #if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_LASX
    #define XTEXT_SIMDUTF_IMPLEMENTATION_LSX 0
  #else
    #define XTEXT_SIMDUTF_IMPLEMENTATION_LSX (XTEXT_SIMDUTF_IS_LSX)
  #endif
#endif
#if XTEXT_SIMDUTF_IMPLEMENTATION_LSX && XTEXT_SIMDUTF_IS_LSX
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_LSX 1
#else
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_LSX 0
#endif

#define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_FALLBACK (XTEXT_SIMDUTF_IMPLEMENTATION_FALLBACK)
#include <xtext/simdutf/internal/isadetection.h>

#if XTEXT_SIMDUTF_IMPLEMENTATION_LSX

namespace xtext {
/**
 * Implementation for LoongArch SX.
 */
namespace lsx {} // namespace lsx
} // namespace xtext

  #include <xtext/simdutf/lsx/h/implementation.h>

  #include <xtext/simdutf/lsx/h/begin.h>

  // Declarations
  #include <xtext/simdutf/lsx/h/intrinsics.h>
  #include <xtext/simdutf/lsx/h/bitmanipulation.h>
  #include <xtext/simdutf/lsx/h/simd.h>

  #include <xtext/simdutf/lsx/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_LSX

#endif // XTEXT_SIMDUTF_LSX_H
