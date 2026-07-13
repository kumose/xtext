#ifndef XTEXT_SIMDUTF_ICELAKE_BITMANIPULATION_H
#define XTEXT_SIMDUTF_ICELAKE_BITMANIPULATION_H

namespace xtext {
namespace XTEXT_SIMDUTF_IMPLEMENTATION {
namespace {

#ifdef XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
xtext_really_inline unsigned __int64 count_ones(uint64_t input_num) {
  // note: we do not support legacy 32-bit Windows
  return __popcnt64(input_num); // Visual Studio wants two underscores
}
#else
xtext_really_inline long long int count_ones(uint64_t input_num) {
  return _popcnt64(input_num);
}
#endif

#ifdef XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
xtext_really_inline unsigned __int64 count_ones32(uint32_t input_num) {
  // note: we do not support legacy 32-bit Windows
  return __popcnt(input_num); // Visual Studio wants two underscores
}
#else
xtext_really_inline long long int count_ones32(uint32_t input_num) {
  return _popcnt32(input_num);
}
#endif

#if XTEXT_SIMDUTF_NEED_TRAILING_ZEROES
// xtext_really_inline int trailing_zeroes(uint64_t input_num) {
//   #if XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
//   return (int)_tzcnt_u64(input_num);
//   #else  // XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
//   return __builtin_ctzll(input_num);
//   #endif // XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
// }
#endif

} // unnamed namespace
} // namespace XTEXT_SIMDUTF_IMPLEMENTATION
} // namespace xtext

#endif // XTEXT_SIMDUTF_ICELAKE_BITMANIPULATION_H
