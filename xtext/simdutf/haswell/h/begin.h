#define XTEXT_SIMDUTF_IMPLEMENTATION haswell
#define XTEXT_SIMDUTF_SIMD_HAS_BYTEMASK 1

#if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_HASWELL
// nothing needed.
#else
XTEXT_SIMDUTF_TARGET_HASWELL
#endif

#if XTEXT_SIMDUTF_GCC11ORMORE // workaround for
                        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105593
// clang-format off
XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wmaybe-uninitialized)
// clang-format on
#endif // end of workaround
