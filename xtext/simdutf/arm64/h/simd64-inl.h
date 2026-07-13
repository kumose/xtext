template <typename T> struct simd64;

template <> struct simd64<uint64_t> {
  uint64x2_t value;

  xtext_really_inline simd64(const uint64x2_t v) : value(v) {}

  template <typename Pointer>
  xtext_really_inline simd64(const Pointer *ptr)
      : value(vld1q_u64(reinterpret_cast<const uint64_t *>(ptr))) {}

  xtext_really_inline uint64_t sum() const { return vaddvq_u64(value); }

  // operators
  xtext_really_inline simd64 &operator+=(const simd64 other) {
    value = vaddq_u64(value, other.value);
    return *this;
  }

  // static members
  xtext_really_inline static simd64<uint64_t> zero() {
    return vdupq_n_u64(0);
  }

  xtext_really_inline static simd64<uint64_t> splat(uint64_t v) {
    return vdupq_n_u64(v);
  }
};
