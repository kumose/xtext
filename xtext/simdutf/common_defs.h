#ifndef XTEXT_SIMDUTF_COMMON_DEFS_H
#define XTEXT_SIMDUTF_COMMON_DEFS_H

#include <xtext/simdutf/portability.h>
#include <xtext/simdutf/avx512.h>

// Sometimes logging is useful, but we want it disabled by default
// and free of any logging code in release builds.
#ifdef XTEXT_SIMDUTF_LOGGING
  #include <iostream>
  #define xtext_log(msg)                                                     \
    std::cout << "[" << __FUNCTION__ << "]: " << msg << std::endl              \
              << "\t" << __FILE__ << ":" << __LINE__ << std::endl;
  #define xtext_log_assert(cond, msg)                                        \
    do {                                                                       \
      if (!(cond)) {                                                           \
        std::cerr << "[" << __FUNCTION__ << "]: " << msg << std::endl          \
                  << "\t" << __FILE__ << ":" << __LINE__ << std::endl;         \
        std::abort();                                                          \
      }                                                                        \
    } while (0)
#else
  #define xtext_log(msg)
  #define xtext_log_assert(cond, msg)
#endif

#if XTEXT_SIMDUTF_CPLUSPLUS17
  #define xtext_unused [[maybe_unused]]
#endif // XTEXT_SIMDUTF_CPLUSPLUS17

#if defined(XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO)
  #define XTEXT_SIMDUTF_DEPRECATED __declspec(deprecated)

  #define xtext_really_inline __forceinline // really inline in release mode
  #define xtext_always_inline __forceinline // always inline, no matter what
  #define xtext_never_inline __declspec(noinline)

  #ifndef xtext_unused
    #define xtext_unused
  #endif // xtext_unused
  #define xtext_warn_unused

  #ifndef xtext_likely
    #define xtext_likely(x) x
  #endif
  #ifndef xtext_unlikely
    #define xtext_unlikely(x) x
  #endif

  #define XTEXT_SIMDUTF_PUSH_DISABLE_WARNINGS __pragma(warning(push))
  #define XTEXT_SIMDUTF_PUSH_DISABLE_ALL_WARNINGS __pragma(warning(push, 0))
  #define XTEXT_SIMDUTF_DISABLE_VS_WARNING(WARNING_NUMBER)                           \
    __pragma(warning(disable : WARNING_NUMBER))
  // Get rid of Intellisense-only warnings (Code Analysis)
  // Though __has_include is C++17, it is supported in Visual Studio 2017 or
  // better (_MSC_VER>=1910).
  #ifdef __has_include
    #if __has_include(<CppCoreCheck\Warnings.h>)
      #include <CppCoreCheck\Warnings.h>
      #define XTEXT_SIMDUTF_DISABLE_UNDESIRED_WARNINGS                               \
        XTEXT_SIMDUTF_DISABLE_VS_WARNING(ALL_CPPCORECHECK_WARNINGS)
    #endif
  #endif

  #ifndef XTEXT_SIMDUTF_DISABLE_UNDESIRED_WARNINGS
    #define XTEXT_SIMDUTF_DISABLE_UNDESIRED_WARNINGS
  #endif

  #define XTEXT_SIMDUTF_DISABLE_DEPRECATED_WARNING XTEXT_SIMDUTF_DISABLE_VS_WARNING(4996)
  #define XTEXT_SIMDUTF_DISABLE_STRICT_OVERFLOW_WARNING
  #define XTEXT_SIMDUTF_POP_DISABLE_WARNINGS __pragma(warning(pop))
  #define XTEXT_SIMDUTF_DISABLE_UNUSED_WARNING
#else // XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
  #if defined(__OPTIMIZE__) || defined(NDEBUG)
    #define xtext_really_inline inline __attribute__((always_inline))
  #else
    #define xtext_really_inline inline
  #endif
  #define xtext_always_inline                                                \
    inline __attribute__((always_inline)) // always inline, no matter what
  #define XTEXT_SIMDUTF_DEPRECATED __attribute__((deprecated))
  #define xtext_never_inline inline __attribute__((noinline))
  #ifndef xtext_unused
    #define xtext_unused __attribute__((unused))
  #endif // xtext_unused
  #define xtext_warn_unused __attribute__((warn_unused_result))

  #ifndef xtext_likely
    #define xtext_likely(x) __builtin_expect(!!(x), 1)
  #endif
  #ifndef xtext_unlikely
    #define xtext_unlikely(x) __builtin_expect(!!(x), 0)
  #endif
  // clang-format off
  #define XTEXT_SIMDUTF_PUSH_DISABLE_WARNINGS _Pragma("GCC diagnostic push")
  // gcc doesn't seem to disable all warnings with all and extra, add warnings
  // here as necessary
  #define XTEXT_SIMDUTF_PUSH_DISABLE_ALL_WARNINGS                                    \
    XTEXT_SIMDUTF_PUSH_DISABLE_WARNINGS                                              \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Weffc++)                                      \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wall)                                         \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wconversion)                                  \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wextra)                                       \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wattributes)                                  \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wimplicit-fallthrough)                        \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wnon-virtual-dtor)                            \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wreturn-type)                                 \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wshadow)                                      \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wunused-parameter)                            \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wunused-variable)
  #define XTEXT_SIMDUTF_PRAGMA(P) _Pragma(#P)
  #define XTEXT_SIMDUTF_DISABLE_GCC_WARNING(WARNING)                                 \
    XTEXT_SIMDUTF_PRAGMA(GCC diagnostic ignored #WARNING)
  #if defined(XTEXT_SIMDUTF_CLANG_VISUAL_STUDIO)
    #define XTEXT_SIMDUTF_DISABLE_UNDESIRED_WARNINGS                                 \
      XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wmicrosoft-include)
  #else
    #define XTEXT_SIMDUTF_DISABLE_UNDESIRED_WARNINGS
  #endif
  #define XTEXT_SIMDUTF_DISABLE_DEPRECATED_WARNING                                   \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wdeprecated-declarations)
  #define XTEXT_SIMDUTF_DISABLE_STRICT_OVERFLOW_WARNING                              \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wstrict-overflow)
  #define XTEXT_SIMDUTF_POP_DISABLE_WARNINGS _Pragma("GCC diagnostic pop")
  #define XTEXT_SIMDUTF_DISABLE_UNUSED_WARNING                                       \
    XTEXT_SIMDUTF_PUSH_DISABLE_WARNINGS                                              \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wunused-function)                             \
    XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wunused-const-variable)
  // clang-format on

#endif // MSC_VER

// Will evaluate to constexpr in C++23 or later. This makes it possible to mark
// functions constexpr if the "if consteval" feature is available to use.
#if XTEXT_SIMDUTF_CPLUSPLUS23
  #define xtext_constexpr23 constexpr
#else
  #define xtext_constexpr23
#endif

#ifndef XTEXT_SIMDUTF_DLLIMPORTEXPORT
  #if defined(XTEXT_SIMDUTF_VISUAL_STUDIO) // Visual Studio
                                     /**
                                      * Windows users need to do some extra work when building
                                      * or using a dynamic library (DLL). When building, we need
                                      * to set XTEXT_SIMDUTF_DLLIMPORTEXPORT to __declspec(dllexport).
                                      * When *using* the DLL, the user needs to set
                                      * XTEXT_SIMDUTF_DLLIMPORTEXPORT __declspec(dllimport).
                                      *
                                      * Static libraries not need require such work.
                                      *
                                      * It does not matter here whether you are using
                                      * the regular visual studio or clang under visual
                                      * studio, you still need to handle these issues.
                                      *
                                      * Non-Windows systems do not have this complexity.
                                      */
    #if XTEXT_SIMDUTF_BUILDING_WINDOWS_DYNAMIC_LIBRARY

      // We set XTEXT_SIMDUTF_BUILDING_WINDOWS_DYNAMIC_LIBRARY when we build a DLL
      // under Windows. It should never happen that both
      // XTEXT_SIMDUTF_BUILDING_WINDOWS_DYNAMIC_LIBRARY and
      // XTEXT_SIMDUTF_USING_WINDOWS_DYNAMIC_LIBRARY are set.
      #define XTEXT_SIMDUTF_DLLIMPORTEXPORT __declspec(dllexport)
    #elif XTEXT_SIMDUTF_USING_WINDOWS_DYNAMIC_LIBRARY
      // Windows user who call a dynamic library should set
      // XTEXT_SIMDUTF_USING_WINDOWS_DYNAMIC_LIBRARY to 1.

      #define XTEXT_SIMDUTF_DLLIMPORTEXPORT __declspec(dllimport)
    #else
      // We assume by default static linkage
      #define XTEXT_SIMDUTF_DLLIMPORTEXPORT
    #endif
  #else // defined(XTEXT_SIMDUTF_VISUAL_STUDIO)
    // Non-Windows systems do not have this complexity.
    #define XTEXT_SIMDUTF_DLLIMPORTEXPORT
  #endif // defined(XTEXT_SIMDUTF_VISUAL_STUDIO)
#endif

#if XTEXT_SIMDUTF_MAYBE_UNUSED_AVAILABLE
  #define xtext_maybe_unused [[maybe_unused]]
#else
  #define xtext_maybe_unused
#endif

#endif // XTEXT_SIMDUTF_COMMON_DEFS_H
