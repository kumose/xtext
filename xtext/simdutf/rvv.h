#ifndef XTEXT_SIMDUTF_RVV_H
#define XTEXT_SIMDUTF_RVV_H

#ifdef XTEXT_SIMDUTF_FALLBACK_H
  #error "rvv.h must be included before fallback.h"
#endif

#include <xtext/simdutf/portability.h>

#define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_RVV XTEXT_SIMDUTF_IS_RVV

#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_RVV
  #define XTEXT_SIMDUTF_IMPLEMENTATION_RVV                                           \
    (XTEXT_SIMDUTF_CAN_ALWAYS_RUN_RVV ||                                             \
     (XTEXT_SIMDUTF_IS_RISCV64 && XTEXT_SIMDUTF_HAS_RVV_INTRINSICS &&                      \
      XTEXT_SIMDUTF_HAS_RVV_TARGET_REGION))
#endif

#if XTEXT_SIMDUTF_IMPLEMENTATION_RVV

  #if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_RVV
    #define XTEXT_SIMDUTF_TARGET_RVV
  #else
    #define XTEXT_SIMDUTF_TARGET_RVV XTEXT_SIMDUTF_TARGET_REGION("arch=+v")
  #endif
  #if !XTEXT_SIMDUTF_IS_ZVBB && XTEXT_SIMDUTF_HAS_ZVBB_INTRINSICS
    #define XTEXT_SIMDUTF_TARGET_ZVBB XTEXT_SIMDUTF_TARGET_REGION("arch=+v,+zvbb")
  #endif

namespace xtext {
namespace rvv {} // namespace rvv
} // namespace xtext

  #include <xtext/simdutf/rvv/h/implementation.h>
  #include <xtext/simdutf/rvv/h/begin.h>
  #include <xtext/simdutf/rvv/h/intrinsics.h>
  #include <xtext/simdutf/rvv/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_RVV

#endif // XTEXT_SIMDUTF_RVV_H
