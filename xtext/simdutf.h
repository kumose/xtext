#ifndef XTEXT_SIMDUTF_H
#define XTEXT_SIMDUTF_H
#include <cstring>

#include <xtext/simdutf/compiler_check.h>
#include <xtext/simdutf/common_defs.h>
#include <xtext/simdutf/encoding_types.h>
#include <xtext/simdutf/error.h>

XTEXT_SIMDUTF_PUSH_DISABLE_WARNINGS
XTEXT_SIMDUTF_DISABLE_UNDESIRED_WARNINGS

// Public API
#include <xtext/simdutf/simdutf_version.h>
#include <xtext/simdutf/implementation.h>

// Implementation-internal files (must be included before the implementations
// themselves, to keep amalgamation working--otherwise, the first time a file is
// included, it might be put inside the #ifdef
// XTEXT_SIMDUTF_IMPLEMENTATION_ARM64/FALLBACK/etc., which means the other
// implementations can't compile unless that implementation is turned on).
#include <xtext/simdutf/internal/isadetection.h>

XTEXT_SIMDUTF_POP_DISABLE_WARNINGS

#endif // XTEXT_SIMDUTF_H
