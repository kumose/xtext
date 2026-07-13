#define XTEXT_SIMDUTF_IMPLEMENTATION icelake

#if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_ICELAKE
// nothing needed.
#else
XTEXT_SIMDUTF_TARGET_ICELAKE
#endif

#if XTEXT_SIMDUTF_GCC11ORMORE // workaround for
                        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105593
// clang-format off
XTEXT_SIMDUTF_DISABLE_GCC_WARNING(-Wmaybe-uninitialized)
// clang-format on
#endif // end of workaround
