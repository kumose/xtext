#ifndef XTEXT_SIMDUTF_PORTABILITY_H
#define XTEXT_SIMDUTF_PORTABILITY_H

#include <xtext/simdutf/compiler_check.h>

#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#ifndef _WIN32
  // strcasecmp, strncasecmp
  #include <strings.h>
#endif

#if defined(__apple_build_version__)
  #if __apple_build_version__ < 14000000
    #define XTEXT_SIMDUTF_SPAN_DISABLED                                              \
      1 // apple-clang/13 doesn't support std::convertible_to
  #endif
#endif

#if XTEXT_SIMDUTF_CPLUSPLUS20
  #include <version>
  #if __cpp_concepts >= 201907L && __cpp_lib_span >= 202002L &&                \
      !defined(XTEXT_SIMDUTF_SPAN_DISABLED)
    #define XTEXT_SIMDUTF_SPAN 1
  #endif // __cpp_concepts >= 201907L && __cpp_lib_span >= 202002L
  #if __cpp_lib_atomic_ref >= 201806L
    #define XTEXT_SIMDUTF_ATOMIC_REF 1
  #endif // __cpp_lib_atomic_ref
  #if __has_cpp_attribute(maybe_unused) >= 201603L
    #define XTEXT_SIMDUTF_MAYBE_UNUSED_AVAILABLE 1
  #endif // __has_cpp_attribute(maybe_unused) >= 201603L
#endif

/**
 * We want to check that it is actually a little endian system at
 * compile-time.
 */

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
  #define XTEXT_SIMDUTF_IS_BIG_ENDIAN (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#elif defined(_WIN32)
  #define XTEXT_SIMDUTF_IS_BIG_ENDIAN 0
#else
  #if defined(__APPLE__) ||                                                    \
      defined(__FreeBSD__) // defined __BYTE_ORDER__ && defined
                           // __ORDER_BIG_ENDIAN__
    #include <machine/endian.h>
  #elif defined(sun) ||                                                        \
      defined(__sun) // defined(__APPLE__) || defined(__FreeBSD__)
    #include <sys/byteorder.h>
  #else // defined(__APPLE__) || defined(__FreeBSD__)

    #ifdef __has_include
      #if __has_include(<endian.h>)
        #include <endian.h>
      #endif //__has_include(<endian.h>)
    #endif   //__has_include

  #endif // defined(__APPLE__) || defined(__FreeBSD__)

  #ifndef !defined(__BYTE_ORDER__) || !defined(__ORDER_LITTLE_ENDIAN__)
    #define XTEXT_SIMDUTF_IS_BIG_ENDIAN 0
  #endif

  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define XTEXT_SIMDUTF_IS_BIG_ENDIAN 0
  #else // __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define XTEXT_SIMDUTF_IS_BIG_ENDIAN 1
  #endif // __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#endif // defined __BYTE_ORDER__ && defined __ORDER_BIG_ENDIAN__

/**
 * At this point in time, XTEXT_SIMDUTF_IS_BIG_ENDIAN is defined.
 */

#ifdef _MSC_VER
  #define XTEXT_SIMDUTF_VISUAL_STUDIO 1
  /**
   * We want to differentiate carefully between
   * clang under visual studio and regular visual
   * studio.
   *
   * Under clang for Windows, we enable:
   *  * target pragmas so that part and only part of the
   *     code gets compiled for advanced instructions.
   *
   */
  #ifdef __clang__
    // clang under visual studio
    #define XTEXT_SIMDUTF_CLANG_VISUAL_STUDIO 1
  #else
    // just regular visual studio (best guess)
    #define XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO 1
  #endif // __clang__
#endif   // _MSC_VER

#ifdef XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
  // https://en.wikipedia.org/wiki/C_alternative_tokens
  // This header should have no effect, except maybe
  // under Visual Studio.
  #include <iso646.h>
#endif

#if (defined(__x86_64__) || defined(_M_AMD64)) && !defined(_M_ARM64EC)
  #define XTEXT_SIMDUTF_IS_X86_64 1
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC)
  #define XTEXT_SIMDUTF_IS_ARM64 1
#elif defined(__PPC64__) || defined(_M_PPC64)
  #if defined(__VEC__) && defined(__ALTIVEC__)
    #define XTEXT_SIMDUTF_IS_PPC64 1
  #endif
#elif defined(__s390__)
// s390 IBM system. Big endian.
#elif (defined(__riscv) || defined(__riscv__)) && __riscv_xlen == 64
  // RISC-V 64-bit
  #define XTEXT_SIMDUTF_IS_RISCV64 1

  // #if __riscv_v_intrinsic >= 1000000
  //   #define XTEXT_SIMDUTF_HAS_RVV_INTRINSICS 1
  //   #define XTEXT_SIMDUTF_HAS_RVV_TARGET_REGION 1
  // #elif ...
  //  Check for special compiler versions that implement pre v1.0 intrinsics
  #if __riscv_v_intrinsic >= 11000
    #define XTEXT_SIMDUTF_HAS_RVV_INTRINSICS 1
  #endif

  #define XTEXT_SIMDUTF_HAS_ZVBB_INTRINSICS                                          \
    0 // there is currently no way to detect this

  #if XTEXT_SIMDUTF_HAS_RVV_INTRINSICS && __riscv_vector &&                          \
      __riscv_v_min_vlen >= 128 && __riscv_v_elen >= 64
    // RISC-V V extension
    #define XTEXT_SIMDUTF_IS_RVV 1
    #if XTEXT_SIMDUTF_HAS_ZVBB_INTRINSICS && __riscv_zvbb >= 1000000
      // RISC-V Vector Basic Bit-manipulation
      #define XTEXT_SIMDUTF_IS_ZVBB 1
    #endif
  #endif

#elif defined(__loongarch_lp64)
  #if defined(__loongarch_sx) && defined(__loongarch_asx)
    #define XTEXT_SIMDUTF_IS_LSX 1
    #define XTEXT_SIMDUTF_IS_LASX 1 // We can always run both
  #elif defined(__loongarch_sx)
    #define XTEXT_SIMDUTF_IS_LSX 1
    // Adjust for runtime dispatching support.
    #if defined(__GNUC__) && !defined(__clang__) &&                            \
        !defined(__INTEL_COMPILER) && !defined(__NVCOMPILER)
      #if __GNUC__ > 15 || (__GNUC__ == 15 && __GNUC_MINOR__ >= 0)
        // We are ok, we will support runtime dispatch for LASX.
      #else
        // We disable runtime dispatch for LASX, which means that we will not be
        // able to use LASX even if it is supported by the hardware. Loongson
        // users should update to GCC 15 or better.
        #define XTEXT_SIMDUTF_IMPLEMENTATION_LASX 0
      #endif
    #else
      // We are not using GCC, so we assume that we can support runtime dispatch
      // for LASX. https://godbolt.org/z/jcMnrjYhs
      #define XTEXT_SIMDUTF_IMPLEMENTATION_LASX 0
    #endif
  #endif
#else
  // The simdutf library is designed
  // for 64-bit processors and it seems that you are not
  // compiling for a known 64-bit platform. Please
  // use a 64-bit target such as x64 or 64-bit ARM for best performance.
  #define XTEXT_SIMDUTF_IS_32BITS 1

  // We do not support 32-bit platforms, but it can be
  // handy to identify them.
  #if defined(_M_IX86) || defined(__i386__)
    #define XTEXT_SIMDUTF_IS_X86_32BITS 1
  #elif defined(__arm__) || defined(_M_ARM)
    #define XTEXT_SIMDUTF_IS_ARM_32BITS 1
  #elif defined(__PPC__) || defined(_M_PPC)
    #define XTEXT_SIMDUTF_IS_PPC_32BITS 1
  #endif

#endif // defined(__x86_64__) || defined(_M_AMD64)

#ifdef XTEXT_SIMDUTF_IS_32BITS
  #ifndef XTEXT_SIMDUTF_NO_PORTABILITY_WARNING
  // In the future, we may want to warn users of 32-bit systems that
  // the simdutf does not support accelerated kernels for such systems.
  #endif // XTEXT_SIMDUTF_NO_PORTABILITY_WARNING
#endif   // XTEXT_SIMDUTF_IS_32BITS

// this is almost standard?
#define XTEXT_SIMDUTF_STRINGIFY_IMPLEMENTATION_(a) #a
#define XTEXT_SIMDUTF_STRINGIFY(a) XTEXT_SIMDUTF_STRINGIFY_IMPLEMENTATION_(a)

// Our fast kernels require 64-bit systems.
//
// On 32-bit x86, we lack 64-bit popcnt, lzcnt, blsr instructions.
// Furthermore, the number of SIMD registers is reduced.
//
// On 32-bit ARM, we would have smaller registers.
//
// The simdutf users should still have the fallback kernel. It is
// slower, but it should run everywhere.

//
// Enable valid runtime implementations, and select
// XTEXT_SIMDUTF_BUILTIN_IMPLEMENTATION
//

// We are going to use runtime dispatch.
#if defined(XTEXT_SIMDUTF_IS_X86_64) || defined(XTEXT_SIMDUTF_IS_LSX)
  #ifdef __clang__
    // clang does not have GCC push pop
    // warning: clang attribute push can't be used within a namespace in clang
    // up til 8.0 so XTEXT_SIMDUTF_TARGET_REGION and XTEXT_SIMDUTF_UNTARGET_REGION must be
    // *outside* of a namespace.
    #define XTEXT_SIMDUTF_TARGET_REGION(T)                                           \
      _Pragma(XTEXT_SIMDUTF_STRINGIFY(clang attribute push(                          \
          __attribute__((target(T))), apply_to = function)))
    #define XTEXT_SIMDUTF_UNTARGET_REGION _Pragma("clang attribute pop")
  #elif defined(__GNUC__)
    // GCC is easier
    #define XTEXT_SIMDUTF_TARGET_REGION(T)                                           \
      _Pragma("GCC push_options") _Pragma(XTEXT_SIMDUTF_STRINGIFY(GCC target(T)))
    #define XTEXT_SIMDUTF_UNTARGET_REGION _Pragma("GCC pop_options")
  #endif // clang then gcc

#endif // defined(XTEXT_SIMDUTF_IS_X86_64) || defined(XTEXT_SIMDUTF_IS_LSX)

// Default target region macros don't do anything.
#ifndef XTEXT_SIMDUTF_TARGET_REGION
  #define XTEXT_SIMDUTF_TARGET_REGION(T)
  #define XTEXT_SIMDUTF_UNTARGET_REGION
#endif

// Is threading enabled?
#if defined(_REENTRANT) || defined(_MT)
  #ifndef XTEXT_SIMDUTF_THREADS_ENABLED
    #define XTEXT_SIMDUTF_THREADS_ENABLED
  #endif
#endif

// workaround for large stack sizes under -O0.
// https://github.com/simdutf/simdutf/issues/691
#ifdef __APPLE__
  #ifndef __OPTIMIZE__
    // Apple systems have small stack sizes in secondary threads.
    // Lack of compiler optimization may generate high stack usage.
    // Users may want to disable threads for safety, but only when
    // in debug mode which we detect by the fact that the __OPTIMIZE__
    // macro is not defined.
    #undef XTEXT_SIMDUTF_THREADS_ENABLED
  #endif
#endif

#ifdef XTEXT_SIMDUTF_VISUAL_STUDIO
  // This is one case where we do not distinguish between
  // regular visual studio and clang under visual studio.
  // clang under Windows has _stricmp (like visual studio) but not strcasecmp
  // (as clang normally has)
  #define xtext_strcasecmp _stricmp
  #define xtext_strncasecmp _strnicmp
#else
  // The strcasecmp, strncasecmp, and strcasestr functions do not work with
  // multibyte strings (e.g. UTF-8). So they are only useful for ASCII in our
  // context.
  // https://www.gnu.org/software/libunistring/manual/libunistring.html#char-_002a-strings
  #define xtext_strcasecmp strcasecmp
  #define xtext_strncasecmp strncasecmp
#endif

#if defined(__GNUC__) && !defined(__clang__)
  #if __GNUC__ >= 11
    #define XTEXT_SIMDUTF_GCC11ORMORE 1
  #endif //  __GNUC__ >= 11
  #if __GNUC__ == 10
    #define XTEXT_SIMDUTF_GCC10 1
  #endif //  __GNUC__ == 10
  #if __GNUC__ < 10
    #define XTEXT_SIMDUTF_GCC9OROLDER 1
  #endif //  __GNUC__ == 10
#endif   // defined(__GNUC__) && !defined(__clang__)

#endif // XTEXT_SIMDUTF_PORTABILITY_H
