#ifndef XTEXT_SIMDUTF_ICELAKE_H
#define XTEXT_SIMDUTF_ICELAKE_H

#include <xtext/simdutf/portability.h>

#ifdef __has_include
  // How do we detect that a compiler supports vbmi2?
  // For sure if the following header is found, we are ok?
  #if __has_include(<avx512vbmi2intrin.h>)
    #define XTEXT_SIMDUTF_COMPILER_SUPPORTS_VBMI2 1
  #endif
#endif

#ifdef _MSC_VER
  #if _MSC_VER >= 1930
    // Visual Studio 2022 and up support VBMI2 under x64 even if the header
    // avx512vbmi2intrin.h is not found.
    // Visual Studio 2019 technically supports VBMI2, but the implementation
    // might be unreliable. Search for visualstudio2019icelakeissue in our
    // tests.
    #ifndef XTEXT_SIMDUTF_COMPILER_SUPPORTS_VBMI2
      #define XTEXT_SIMDUTF_COMPILER_SUPPORTS_VBMI2 1
    #endif
  #endif
#endif

#if XTEXT_SIMDUTF_GCC9OROLDER && XTEXT_SIMDUTF_IS_X86_64
  #define XTEXT_SIMDUTF_IMPLEMENTATION_ICELAKE 0
  #warning                                                                     \
      "You are using a legacy GCC compiler, we are disabling AVX-512 support"
#endif

// We allow icelake on x64 as long as the compiler is known to support VBMI2.
#ifndef XTEXT_SIMDUTF_IMPLEMENTATION_ICELAKE
  #define XTEXT_SIMDUTF_IMPLEMENTATION_ICELAKE                                       \
    ((XTEXT_SIMDUTF_IS_X86_64) && (XTEXT_SIMDUTF_COMPILER_SUPPORTS_VBMI2))
#endif

// To see why  (__BMI__) && (__LZCNT__) are not part of this next line, see
// https://github.com/simdutf/simdutf/issues/1247
#if ((XTEXT_SIMDUTF_IMPLEMENTATION_ICELAKE) && (XTEXT_SIMDUTF_IS_X86_64) && (__AVX2__) &&  \
     (XTEXT_SIMDUTF_HAS_AVX512F && XTEXT_SIMDUTF_HAS_AVX512DQ && XTEXT_SIMDUTF_HAS_AVX512VL &&   \
      XTEXT_SIMDUTF_HAS_AVX512VBMI2) &&                                              \
     (!XTEXT_SIMDUTF_IS_32BITS))
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ICELAKE 1
#else
  #define XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ICELAKE 0
#endif

#if XTEXT_SIMDUTF_IMPLEMENTATION_ICELAKE
  #if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ICELAKE
    #define XTEXT_SIMDUTF_TARGET_ICELAKE
  #else
    #define XTEXT_SIMDUTF_TARGET_ICELAKE                                             \
      XTEXT_SIMDUTF_TARGET_REGION(                                                   \
          "avx512f,avx512dq,avx512cd,avx512bw,avx512vbmi,avx512vbmi2,"         \
          "avx512vl,avx2,bmi,bmi2,pclmul,lzcnt,popcnt,avx512vpopcntdq")
  #endif

namespace xtext {
namespace icelake {} // namespace icelake
} // namespace xtext

  //
  // These two need to be included outside XTEXT_SIMDUTF_TARGET_REGION
  //
  #include <xtext/simdutf/icelake/h/intrinsics.h>
  #include <xtext/simdutf/icelake/h/implementation.h>

  //
  // The rest need to be inside the region
  //
  #include <xtext/simdutf/icelake/h/begin.h>
  // Declarations
  #include <xtext/simdutf/icelake/h/bitmanipulation.h>
  #include <xtext/simdutf/icelake/h/simd.h>

  #include <xtext/simdutf/icelake/h/end.h>

#endif // XTEXT_SIMDUTF_IMPLEMENTATION_ICELAKE
#endif // XTEXT_SIMDUTF_ICELAKE_H
