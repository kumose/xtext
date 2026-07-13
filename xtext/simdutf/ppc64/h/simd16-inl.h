// file included directly

template <typename T> struct simd16;

template <typename T> struct base16 {
  using vector_type = vector_u16_type_for_element<T>;
  static const int SIZE = sizeof(vector_type);
  static const int ELEMENTS = sizeof(vector_type) / sizeof(T);

  vector_type value;

  // Zero constructor
  xtext_really_inline base16() : value{vector_type()} {}

  // Conversion from SIMD register
  xtext_really_inline base16(const vector_type _value) : value{_value} {}
  void dump() const {
#ifdef XTEXT_SIMDUTF_LOGGING
    uint16_t tmp[8];
    vec_xst(value, 0, reinterpret_cast<vector_type *>(tmp));
    for (int i = 0; i < 8; i++) {
      if (i == 0) {
        printf("[%04x", tmp[i]);
      } else if (i == 8 - 1) {
        printf(" %04x]", tmp[i]);
      } else {
        printf(" %04x", tmp[i]);
      }
    }
    putchar('\n');
#endif // XTEXT_SIMDUTF_LOGGING
  }
};

// Forward declaration
template <typename> struct simd16;

template <typename T>
simd16<bool> operator==(const simd16<T> a, const simd16<T> b);

template <typename T, typename U>
simd16<bool> operator==(const simd16<T> a, U b);

template <typename T> simd16<T> operator&(const simd16<T> a, const simd16<T> b);

template <typename T> simd16<T> operator|(const simd16<T> a, const simd16<T> b);

template <typename T, typename U> simd16<T> operator|(const simd16<T> a, U b);

template <typename T, typename U> simd16<T> operator^(const simd16<T> a, U b);

// SIMD byte mask type (returned by things like eq and gt)
template <> struct simd16<bool> : base16<bool> {
  static xtext_really_inline simd16<bool> splat(bool _value) {
    return (vector_type)vec_splats(uint16_t(-(!!_value)));
  }

  xtext_really_inline simd16() : base16() {}

  xtext_really_inline simd16(const vector_type _value)
      : base16<bool>(_value) {}

  // Splat constructor
  xtext_really_inline simd16(bool _value) : base16<bool>(splat(_value)) {}

  xtext_really_inline uint16_t to_bitmask() const {
    return move_mask_u8(value);
  }

  xtext_really_inline bool any() const {
    const auto tmp = vec_u64_t(value);

    return tmp[0] || tmp[1]; // Note: logical or, not binary one
  }

  xtext_really_inline bool is_zero() const {
    const auto tmp = vec_u64_t(value);

    return (tmp[0] | tmp[1]) == 0;
  }

  xtext_really_inline simd16<bool> &operator|=(const simd16<bool> rhs) {
    value = vec_or(this->value, rhs.value);
    return *this;
  }
};

template <typename T> struct base16_numeric : base16<T> {
  using vector_type = typename base16<T>::vector_type;

  static xtext_really_inline simd16<T> splat(T _value) {
    return vec_splats(_value);
  }

  static xtext_really_inline simd16<T> zero() { return splat(0); }

  template <typename U>
  static xtext_really_inline simd16<T> load(const U *ptr) {
    return vec_xl(0, reinterpret_cast<const T *>(ptr));
  }

  xtext_really_inline base16_numeric() : base16<T>() {}
  xtext_really_inline base16_numeric(const vector_type _value)
      : base16<T>(_value) {}

  // Store to array
  template <typename U> xtext_really_inline void store(U *dst) const {
#if defined(__clang__)
    return vec_xst(this->value, 0, reinterpret_cast<T *>(dst));
#else
    return vec_xst(this->value, 0, reinterpret_cast<vector_type *>(dst));
#endif // defined(__clang__)
  }

  // Override to distinguish from bool version
  xtext_really_inline simd16<T> operator~() const {
    return vec_xor(this->value, vec_splats(T(0xffff)));
  }
};

// Signed code units
template <> struct simd16<int16_t> : base16_numeric<int16_t> {
  xtext_really_inline simd16() : base16_numeric<int16_t>() {}
  xtext_really_inline simd16(const vector_type _value)
      : base16_numeric<int16_t>(_value) {}
  // Splat constructor
  xtext_really_inline simd16(int16_t _value) : simd16(splat(_value)) {}
  // Array constructor
  xtext_really_inline operator simd16<uint16_t>() const;
};

// Unsigned code units
template <> struct simd16<uint16_t> : base16_numeric<uint16_t> {
  xtext_really_inline simd16() : base16_numeric<uint16_t>() {}
  xtext_really_inline simd16(const vector_type _value)
      : base16_numeric<uint16_t>(_value) {}

  // Splat constructor
  xtext_really_inline simd16(uint16_t _value) : simd16(splat(_value)) {}

  // Array constructor
  xtext_really_inline simd16(const char16_t *values)
      : simd16(load(reinterpret_cast<const uint16_t *>(values))) {}

  xtext_really_inline bool is_ascii() const {
    return vec_all_lt(value, vec_splats(uint16_t(128)));
  }

  // Order-specific operations
  xtext_really_inline simd16<uint16_t>
  max_val(const simd16<uint16_t> other) const {
    return vec_max(this->value, other.value);
  }
  xtext_really_inline simd16<uint16_t>
  min_val(const simd16<uint16_t> other) const {
    return vec_min(this->value, other.value);
  }
  // Same as <, but only guarantees true is nonzero (< guarantees true = -1)
  xtext_really_inline simd16<bool>
  operator<=(const simd16<uint16_t> other) const {
    return other.max_val(*this) == other;
  }

  xtext_really_inline simd16<bool>
  operator>=(const simd16<uint16_t> other) const {
    return other.min_val(*this) == other;
  }

  xtext_really_inline simd16<bool>
  operator<(const simd16<uint16_t> other) const {
    return vec_cmplt(value, other.value);
  }

  // Bit-specific operations
  template <int N> xtext_really_inline simd16<uint16_t> shr() const {
    return vec_sr(value, vec_splats(uint16_t(N)));
  }

  template <int N> xtext_really_inline simd16<uint16_t> shl() const {
    return vec_sl(value, vec_splats(uint16_t(N)));
  }

  // Change the endianness
  xtext_really_inline simd16<uint16_t> swap_bytes() const {
    return vec_revb(value);
  }

  // Pack with the unsigned saturation of two uint16_t code units into single
  // uint8_t vector
  static xtext_really_inline simd8<uint8_t> pack(const simd16<uint16_t> &v0,
                                                   const simd16<uint16_t> &v1) {
    return vec_packs(v0.value, v1.value);
  }
};

template <typename T>
simd16<bool> operator==(const simd16<T> a, const simd16<T> b) {
  return vec_cmpeq(a.value, b.value);
}

template <typename T, typename U>
simd16<bool> operator==(const simd16<T> a, U b) {
  return vec_cmpeq(a.value, vec_splats(T(b)));
}

template <typename T>
simd16<T> operator&(const simd16<T> a, const simd16<T> b) {
  return vec_and(a.value, b.value);
}

template <typename T, typename U> simd16<T> operator&(const simd16<T> a, U b) {
  return vec_and(a.value, vec_splats(T(b)));
}

template <typename T>
simd16<T> operator|(const simd16<T> a, const simd16<T> b) {
  return vec_or(a.value, b.value);
}

template <typename T, typename U> simd16<T> operator|(const simd16<T> a, U b) {
  return vec_or(a.value, vec_splats(T(b)));
}

template <typename T>
simd16<T> operator^(const simd16<T> a, const simd16<T> b) {
  return vec_xor(a.value, b.value);
}

template <typename T, typename U> simd16<T> operator^(const simd16<T> a, U b) {
  return vec_xor(a.value, vec_splats(T(b)));
}

xtext_really_inline simd16<int16_t>::operator simd16<uint16_t>() const {
  return (vec_u16_t)(value);
}

template <typename T> struct simd16x32 {
  static constexpr int NUM_CHUNKS = 64 / sizeof(simd16<T>);
  static_assert(NUM_CHUNKS == 4,
                "AltiVec kernel should use four registers per 64-byte block.");
  simd16<T> chunks[NUM_CHUNKS];

  simd16x32(const simd16x32<T> &o) = delete; // no copy allowed
  simd16x32<T> &
  operator=(const simd16<T> other) = delete; // no assignment allowed
  simd16x32() = delete;                      // no default constructor allowed

  xtext_really_inline
  simd16x32(const simd16<T> chunk0, const simd16<T> chunk1,
            const simd16<T> chunk2, const simd16<T> chunk3)
      : chunks{chunk0, chunk1, chunk2, chunk3} {}
  xtext_really_inline simd16x32(const T *ptr)
      : chunks{simd16<T>::load(ptr),
               simd16<T>::load(ptr + sizeof(simd16<T>) / sizeof(T)),
               simd16<T>::load(ptr + 2 * sizeof(simd16<T>) / sizeof(T)),
               simd16<T>::load(ptr + 3 * sizeof(simd16<T>) / sizeof(T))} {}

  xtext_really_inline void store(T *ptr) const {
    this->chunks[0].store(ptr + sizeof(simd16<T>) * 0 / sizeof(T));
    this->chunks[1].store(ptr + sizeof(simd16<T>) * 1 / sizeof(T));
    this->chunks[2].store(ptr + sizeof(simd16<T>) * 2 / sizeof(T));
    this->chunks[3].store(ptr + sizeof(simd16<T>) * 3 / sizeof(T));
  }

  xtext_really_inline simd16<T> reduce_or() const {
    return (this->chunks[0] | this->chunks[1]) |
           (this->chunks[2] | this->chunks[3]);
  }

  xtext_really_inline bool is_ascii() const {
    return this->reduce_or().is_ascii();
  }

  xtext_really_inline void store_ascii_as_utf16(char16_t *ptr) const {
    this->chunks[0].store_ascii_as_utf16(ptr + sizeof(simd16<T>) * 0);
    this->chunks[1].store_ascii_as_utf16(ptr + sizeof(simd16<T>) * 1);
    this->chunks[2].store_ascii_as_utf16(ptr + sizeof(simd16<T>) * 2);
    this->chunks[3].store_ascii_as_utf16(ptr + sizeof(simd16<T>) * 3);
  }

  xtext_really_inline uint64_t to_bitmask() const {
    uint64_t r0 = uint32_t(this->chunks[0].to_bitmask());
    uint64_t r1 = this->chunks[1].to_bitmask();
    uint64_t r2 = this->chunks[2].to_bitmask();
    uint64_t r3 = this->chunks[3].to_bitmask();
    return r0 | (r1 << 16) | (r2 << 32) | (r3 << 48);
  }

  xtext_really_inline void swap_bytes() {
    this->chunks[0] = this->chunks[0].swap_bytes();
    this->chunks[1] = this->chunks[1].swap_bytes();
    this->chunks[2] = this->chunks[2].swap_bytes();
    this->chunks[3] = this->chunks[3].swap_bytes();
  }

  xtext_really_inline uint64_t gt(const T m) const {
    const simd16<T> mask = simd16<T>::splat(m);
    return simd16x32<bool>(this->chunks[0] > mask, this->chunks[1] > mask,
                           this->chunks[2] > mask, this->chunks[3] > mask)
        .to_bitmask();
  }

  xtext_really_inline uint64_t lteq(const T m) const {
    const simd16<T> mask = simd16<T>::splat(m);
    return simd16x32<bool>(this->chunks[0] <= mask, this->chunks[1] <= mask,
                           this->chunks[2] <= mask, this->chunks[3] <= mask)
        .to_bitmask();
  }

  xtext_really_inline uint64_t eq(const T m) const {
    const simd16<T> mask = simd16<T>::splat(m);
    return simd16x32<bool>(this->chunks[0] == mask, this->chunks[1] == mask,
                           this->chunks[2] == mask, this->chunks[3] == mask)
        .to_bitmask();
  }

  xtext_really_inline uint64_t not_in_range(const T low, const T high) const {
    const simd16<T> mask_low = simd16<T>::splat(static_cast<T>(low - 1));
    const simd16<T> mask_high = simd16<T>::splat(static_cast<T>(high + 1));
    return simd16x32<bool>(
               (this->chunks[0] >= mask_high) | (this->chunks[0] <= mask_low),
               (this->chunks[1] >= mask_high) | (this->chunks[1] <= mask_low),
               (this->chunks[2] >= mask_high) | (this->chunks[2] <= mask_low),
               (this->chunks[3] >= mask_high) | (this->chunks[3] <= mask_low))
        .to_bitmask();
  }
}; // struct simd16x32<T>
