#ifndef XTEXT_SIMDUTF_LASX_BITMANIPULATION_H
#define XTEXT_SIMDUTF_LASX_BITMANIPULATION_H

#include <xtext/simdutf.h>
#include <limits>

namespace xtext {
namespace XTEXT_SIMDUTF_IMPLEMENTATION {
namespace {

xtext_really_inline int count_ones(uint64_t input_num) {
  return __lsx_vpickve2gr_w(__lsx_vpcnt_d(__lsx_vreplgr2vr_d(input_num)), 0);
}

#if XTEXT_SIMDUTF_NEED_TRAILING_ZEROES
xtext_really_inline int trailing_zeroes(uint64_t input_num) {
  return __builtin_ctzll(input_num);
}
#endif

} // unnamed namespace
} // namespace XTEXT_SIMDUTF_IMPLEMENTATION
} // namespace xtext

#endif // XTEXT_SIMDUTF_LASX_BITMANIPULATION_H
