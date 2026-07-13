// Formatting library for C++
//
// Copyright (c) 2012 - 2016, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#include "format-inl.h"

XTEXT_FMT_BEGIN_NAMESPACE

#if XTEXT_FMT_USE_LOCALE
template XTEXT_FMT_API locale_ref::locale_ref(const std::locale& loc);  // DEPRECATED!
template XTEXT_FMT_API auto locale_ref::get<std::locale>() const -> std::locale;
#endif

namespace detail {

template XTEXT_FMT_API auto dragonbox::to_decimal(float x) noexcept
    -> dragonbox::decimal_fp<float>;
template XTEXT_FMT_API auto dragonbox::to_decimal(double x) noexcept
    -> dragonbox::decimal_fp<double>;

// Explicit instantiations for char.

template XTEXT_FMT_API auto thousands_sep_impl(locale_ref)
    -> thousands_sep_result<char>;
template XTEXT_FMT_API auto decimal_point_impl(locale_ref) -> char;

// DEPRECATED!
template XTEXT_FMT_API void buffer<char>::append(const char*, const char*);

// Explicit instantiations for wchar_t.

template XTEXT_FMT_API auto thousands_sep_impl(locale_ref)
    -> thousands_sep_result<wchar_t>;
template XTEXT_FMT_API auto decimal_point_impl(locale_ref) -> wchar_t;

// DEPRECATED!
template XTEXT_FMT_API void buffer<wchar_t>::append(const wchar_t*, const wchar_t*);

}  // namespace detail
XTEXT_FMT_END_NAMESPACE
