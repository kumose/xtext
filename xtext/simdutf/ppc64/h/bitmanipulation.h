#ifndef XTEXT_SIMDUTF_PPC64_BITMANIPULATION_H
#define XTEXT_SIMDUTF_PPC64_BITMANIPULATION_H

namespace xtext {
namespace XTEXT_SIMDUTF_IMPLEMENTATION {
namespace {

#ifdef XTEXT_SIMDUTF_REGULAR_VISUAL_STUDIO
xtext_really_inline int count_ones(uint64_t input_num) {
  // note: we do not support legacy 32-bit Windows
  return __popcnt64(input_num); // Visual Studio wants two underscores
}
#else
xtext_really_inline int count_ones(uint64_t input_num) {
  return __builtin_popcountll(input_num);
}
#endif

#if XTEXT_SIMDUTF_NEED_TRAILING_ZEROES
xtext_really_inline int trailing_zeroes(uint64_t input_num) {
  return __builtin_ctzll(input_num);
}
#endif

} // unnamed namespace
} // namespace XTEXT_SIMDUTF_IMPLEMENTATION
} // namespace xtext

#endif // XTEXT_SIMDUTF_PPC64_BITMANIPULATION_H
