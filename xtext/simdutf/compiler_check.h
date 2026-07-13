#ifndef XTEXT_SIMDUTF_COMPILER_CHECK_H
#define XTEXT_SIMDUTF_COMPILER_CHECK_H

#ifndef __cplusplus
  #error simdutf requires a C++ compiler
#endif

#ifndef XTEXT_SIMDUTF_CPLUSPLUS
  #if defined(_MSVC_LANG) && !defined(__clang__)
    #define XTEXT_SIMDUTF_CPLUSPLUS (_MSC_VER == 1900 ? 201103L : _MSVC_LANG)
  #else
    #define XTEXT_SIMDUTF_CPLUSPLUS __cplusplus
  #endif
#endif

// C++ 26
#if !defined(XTEXT_SIMDUTF_CPLUSPLUS26) && (XTEXT_SIMDUTF_CPLUSPLUS >= 202602L)
  #define XTEXT_SIMDUTF_CPLUSPLUS26 1
#endif

// C++ 23
#if !defined(XTEXT_SIMDUTF_CPLUSPLUS23) && (XTEXT_SIMDUTF_CPLUSPLUS >= 202302L)
  #define XTEXT_SIMDUTF_CPLUSPLUS23 1
#endif

// C++ 20
#if !defined(XTEXT_SIMDUTF_CPLUSPLUS20) && (XTEXT_SIMDUTF_CPLUSPLUS >= 202002L)
  #define XTEXT_SIMDUTF_CPLUSPLUS20 1
#endif

// C++ 17
#if !defined(XTEXT_SIMDUTF_CPLUSPLUS17) && (XTEXT_SIMDUTF_CPLUSPLUS >= 201703L)
  #define XTEXT_SIMDUTF_CPLUSPLUS17 1
#endif

// C++ 14
#if !defined(XTEXT_SIMDUTF_CPLUSPLUS14) && (XTEXT_SIMDUTF_CPLUSPLUS >= 201402L)
  #define XTEXT_SIMDUTF_CPLUSPLUS14 1
#endif

// C++ 11
#if !defined(XTEXT_SIMDUTF_CPLUSPLUS11) && (XTEXT_SIMDUTF_CPLUSPLUS >= 201103L)
  #define XTEXT_SIMDUTF_CPLUSPLUS11 1
#endif

#ifndef XTEXT_SIMDUTF_CPLUSPLUS17
  #error simdutf requires a compiler compliant with the C++17 standard
#endif

#endif // XTEXT_SIMDUTF_COMPILER_CHECK_H
