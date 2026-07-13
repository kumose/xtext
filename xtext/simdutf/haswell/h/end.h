#if XTEXT_SIMDUTF_CAN_ALWAYS_RUN_HASWELL
// nothing needed.
#else
XTEXT_SIMDUTF_UNTARGET_REGION
#endif

#undef XTEXT_SIMDUTF_IMPLEMENTATION
#undef XTEXT_SIMDUTF_SIMD_HAS_BYTEMASK

#if XTEXT_SIMDUTF_GCC11ORMORE // workaround for
                        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105593
XTEXT_SIMDUTF_POP_DISABLE_WARNINGS
#endif // end of workaround
