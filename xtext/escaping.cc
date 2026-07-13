// Copyright (C) 2026 Kumo inc. and its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <xtext/escaping.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#include <utility>

#include <xtext/ascii.h>
#include <xtext/charset.h>
#include <xtext/details/resize_uninitialized.h>
#include <xtext/details/utf8.h>
#include <xtext/simdutf.h>
#include <xtext/str_cat.h>

namespace xtext {

    namespace {

        constexpr bool kUnescapeNulls = false;

        inline bool is_octal_digit(char c) { return ('0' <= c) && (c <= '7'); }

        inline unsigned int hex_digit_to_int(char c) {
            static_assert('0' == 0x30 && 'A' == 0x41 && 'a' == 0x61,
                          "Character set must be ASCII.");
            assert(xtext::ascii_isxdigit(static_cast<unsigned char>(c)));
            unsigned int x = static_cast<unsigned char>(c);
            if (x > '9') {
                x += 9;
            }
            return x & 0xf;
        }

        inline bool IsSurrogate(char32_t c, std::string_view src,
                                std::string *error) {
            if (c >= 0xD800 && c <= 0xDFFF) {
                if (error) {
                    *error = xtext::str_cat(
                        "invalid surrogate character (0xD800-DFFF): \\", src);
                }
                return true;
            }
            return false;
        }

        bool CUnescapeInternal(std::string_view source, bool leave_nulls_escaped,
                               char *dest, ptrdiff_t *dest_len,
                               std::string *error) {
            char *d = dest;
            const char *p = source.data();
            const char *end = p + source.size();
            const char *last_byte = end - 1;

            while (p == d && p < end && *p != '\\') p++, d++;

            while (p < end) {
                if (*p != '\\') {
                    *d++ = *p++;
                } else {
                    if (++p > last_byte) {
                        if (error) *error = "String cannot end with \\";
                        return false;
                    }
                    switch (*p) {
                        case 'a':  *d++ = '\a'; break;
                        case 'b':  *d++ = '\b'; break;
                        case 'f':  *d++ = '\f'; break;
                        case 'n':  *d++ = '\n'; break;
                        case 'r':  *d++ = '\r'; break;
                        case 't':  *d++ = '\t'; break;
                        case 'v':  *d++ = '\v'; break;
                        case '\\': *d++ = '\\'; break;
                        case '\'': *d++ = '\''; break;
                        case '"':  *d++ = '"';  break;
                        case '?':  *d++ = '?';  break;

                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7': {
                            char octal_digit = *p;
                            uint32_t res = *p - '0';
                            if (p < last_byte && is_octal_digit(p[1])) {
                                ++p;
                                res = res * 8 + *p - '0';
                            }
                            if (p < last_byte && is_octal_digit(p[1])) {
                                ++p;
                                res = res * 8 + *p - '0';
                            }
                            if (res > 0xff) {
                                if (error) {
                                    *error = "Octal escape sequence out of range";
                                }
                                return false;
                            }
                            if (res == 0 && leave_nulls_escaped) {
                                *d++ = '\\';
                                *d++ = octal_digit;
                                if (p - source.data() != 2) *d++ = *p++;
                                if (p - source.data() != 2) *d++ = *p++;
                            } else {
                                *d++ = static_cast<char>(res);
                            }
                            break;
                        }
                        case 'x': {
                            if (p + 1 > last_byte) {
                                if (error) *error = "\\x cannot be followed by a non-hex digit";
                                return false;
                            }
                            if (!xtext::ascii_isxdigit(
                                    static_cast<unsigned char>(p[1]))) {
                                if (error) *error = "\\x cannot be followed by a non-hex digit";
                                return false;
                            }
                            uint32_t res = 0;
                            ++p;
                            while (p <= last_byte &&
                                   xtext::ascii_isxdigit(static_cast<unsigned char>(*p))) {
                                res = (res << 4) + hex_digit_to_int(*p);
                                if (res > 0xFF) {
                                    if (error) {
                                        *error = xtext::str_cat(
                                            "\\x escape sequence exceeds 0xff: \\x",
                                            std::string_view(
                                                p - 1, static_cast<size_t>(
                                                    std::min<ptrdiff_t>(
                                                        end - (p - 1), 4))));
                                    }
                                    return false;
                                }
                                ++p;
                            }
                            --p;
                            *d++ = static_cast<char>(res);
                            break;
                        }
                        case 'u': {
                            if (p + 4 > last_byte) {
                                if (error) *error = "\\u must be followed by 4 hex digits";
                                return false;
                            }
                            uint32_t rune = 0;
                            for (int i = 0; i < 4; ++i) {
                                ++p;
                                if (!xtext::ascii_isxdigit(
                                        static_cast<unsigned char>(*p))) {
                                    if (error) {
                                        *error = "\\u must be followed by 4 hex digits";
                                    }
                                    return false;
                                }
                                rune = (rune << 4) + hex_digit_to_int(*p);
                            }
                            if (IsSurrogate(rune, std::string_view(p - 3, 4), error)) {
                                return false;
                            }
                            d += strings_internal::EncodeUTF8Char(d, rune);
                            break;
                        }
                        case 'U': {
                            if (p + 8 > last_byte) {
                                if (error) *error = "\\U must be followed by 8 hex digits";
                                return false;
                            }
                            uint32_t rune = 0;
                            for (int i = 0; i < 8; ++i) {
                                ++p;
                                if (!xtext::ascii_isxdigit(
                                        static_cast<unsigned char>(*p))) {
                                    if (error) {
                                        *error = "\\U must be followed by 8 hex digits";
                                    }
                                    return false;
                                }
                                rune = (rune << 4) + hex_digit_to_int(*p);
                            }
                            if (rune > 0x10FFFF) {
                                if (error) {
                                    *error = xtext::str_cat(
                                        "\\U escape sequence exceeds Unicode limit (0x10FFFF): \\U",
                                        std::string_view(p - 7, 8));
                                }
                                return false;
                            }
                            if (IsSurrogate(rune, std::string_view(p - 7, 8), error)) {
                                return false;
                            }
                            d += strings_internal::EncodeUTF8Char(d, rune);
                            break;
                        }
                        default:
                            *d++ = '\\';
                            *d++ = *p;
                            break;
                    }
                    ++p;
                }
            }
            *dest_len = d - dest;
            return true;
        }

        constexpr unsigned char kCEscapedLen[256] = {
            4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 4, 4, 2, 4, 4,  // 0x00
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0x10
            1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1,  // 0x20
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 0x30
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 0x40
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 0x50
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 0x60
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,  // 0x70
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0x80
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0x90
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0xA0
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0xB0
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0xC0
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0xD0
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0xE0
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 0xF0
        };

        inline size_t CEscapedLength(std::string_view src) {
            size_t escaped_len = 0;
            for (size_t i = 0; i < src.size(); ++i) {
                escaped_len += kCEscapedLen[static_cast<unsigned char>(src[i])];
            }
            return escaped_len;
        }

        void CEscapeAndAppendInternal(std::string_view src, std::string *dest) {
            size_t escaped_len = CEscapedLength(src);
            if (escaped_len == src.size()) {
                dest->append(src.data(), src.size());
                return;
            }

            strings_internal::STLStringResizeUninitialized(dest, dest->size() + escaped_len);
            char *d = &(*dest)[dest->size() - escaped_len];

            for (auto c : src) {
                unsigned char uc = static_cast<unsigned char>(c);
                switch (uc) {
                    case '\n': *d++ = '\\'; *d++ = 'n'; break;
                    case '\r': *d++ = '\\'; *d++ = 'r'; break;
                    case '\t': *d++ = '\\'; *d++ = 't'; break;
                    case '\"': *d++ = '\\'; *d++ = '"'; break;
                    case '\'': *d++ = '\\'; *d++ = '\''; break;
                    case '\\': *d++ = '\\'; *d++ = '\\'; break;
                    case '\a': *d++ = '\\'; *d++ = 'a'; break;
                    case '\b': *d++ = '\\'; *d++ = 'b'; break;
                    case '\f': *d++ = '\\'; *d++ = 'f'; break;
                    case '\v': *d++ = '\\'; *d++ = 'v'; break;
                    default:
                        if (uc < 32 || uc == 127) {
                            *d++ = '\\';
                            *d++ = '0' + ((uc >> 6) & 3);
                            *d++ = '0' + ((uc >> 3) & 7);
                            *d++ = '0' + (uc & 7);
                        } else {
                            *d++ = c;
                        }
                        break;
                }
            }
        }

        void CEscapeAndAppendHexInternal(std::string_view src, std::string *dest,
                                          bool utf8_safe) {
            size_t escaped_len = 0;
            for (size_t i = 0; i < src.size(); ++i) {
                unsigned char uc = static_cast<unsigned char>(src[i]);
                if (utf8_safe && uc >= 0x80) {
                    escaped_len += 1;
                } else {
                    escaped_len += kCEscapedLen[uc];
                }
            }

            strings_internal::STLStringResizeUninitialized(dest, dest->size() + escaped_len);
            char *d = &(*dest)[dest->size() - escaped_len];

            for (size_t i = 0; i < src.size(); ++i) {
                unsigned char uc = static_cast<unsigned char>(src[i]);
                if (utf8_safe && uc >= 0x80) {
                    *d++ = src[i];
                    continue;
                }
                switch (uc) {
                    case '\n': *d++ = '\\'; *d++ = 'n'; break;
                    case '\r': *d++ = '\\'; *d++ = 'r'; break;
                    case '\t': *d++ = '\\'; *d++ = 't'; break;
                    case '\"': *d++ = '\\'; *d++ = '"'; break;
                    case '\'': *d++ = '\\'; *d++ = '\''; break;
                    case '\\': *d++ = '\\'; *d++ = '\\'; break;
                    case '\a': *d++ = '\\'; *d++ = 'a'; break;
                    case '\b': *d++ = '\\'; *d++ = 'b'; break;
                    case '\f': *d++ = '\\'; *d++ = 'f'; break;
                    case '\v': *d++ = '\\'; *d++ = 'v'; break;
                    default:
                        if (uc < 32 || uc == 127) {
                            *d++ = '\\';
                            *d++ = 'x';
                            *d++ = "0123456789abcdef"[uc / 16];
                            *d++ = "0123456789abcdef"[uc % 16];
                        } else {
                            *d++ = src[i];
                        }
                        break;
                }
            }
        }

        const char kHexTable[513] =
            "000102030405060708090a0b0c0d0e0f"
            "101112131415161718191a1b1c1d1e1f"
            "202122232425262728292a2b2c2d2e2f"
            "303132333435363738393a3b3c3d3e3f"
            "404142434445464748494a4b4c4d4e4f"
            "505152535455565758595a5b5c5d5e5f"
            "606162636465666768696a6b6c6d6e6f"
            "707172737475767778797a7b7c7d7e7f"
            "808182838485868788898a8b8c8d8e8f"
            "909192939495969798999a9b9c9d9e9f"
            "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
            "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
            "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
            "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
            "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
            "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";

        int Base64UnescapeInternal(const char *src, size_t slen, char *dest,
                                   ptrdiff_t *dest_len) {
            auto r = xtext::base64_to_binary(
                src, slen, dest, xtext::base64_default);
            if (r.is_ok()) {
                *dest_len = static_cast<ptrdiff_t>(r.count);
                return 0;
            }
            return -1;
        }

    }  // namespace

    bool c_decode(std::string_view source, std::string *dest,
                  std::string *error) {
        strings_internal::STLStringResizeUninitialized(dest, source.size());
        ptrdiff_t dest_size = 0;
        if (!CUnescapeInternal(source, kUnescapeNulls, &(*dest)[0], &dest_size,
                               error)) {
            return false;
        }
        dest->erase(dest_size);
        return true;
    }

    std::string c_encode(std::string_view src) {
        std::string dest;
        CEscapeAndAppendInternal(src, &dest);
        return dest;
    }

    std::string c_hex_encode(std::string_view src) {
        std::string dest;
        CEscapeAndAppendHexInternal(src, &dest, false);
        return dest;
    }

    std::string utf8_safe_encode(std::string_view src) {
        // Same as c_hex_encode with utf8_safe=true, but using octal
        size_t escaped_len = 0;
        for (size_t i = 0; i < src.size(); ++i) {
            unsigned char uc = static_cast<unsigned char>(src[i]);
            if (uc >= 0x80) {
                escaped_len += 1;
            } else {
                escaped_len += kCEscapedLen[uc];
            }
        }

        std::string dest;
        strings_internal::STLStringResizeUninitialized(&dest, escaped_len);
        char *d = &dest[0];

        for (size_t i = 0; i < src.size(); ++i) {
            unsigned char uc = static_cast<unsigned char>(src[i]);
            if (uc >= 0x80) {
                *d++ = src[i];
                continue;
            }
            switch (uc) {
                case '\n': *d++ = '\\'; *d++ = 'n'; break;
                case '\r': *d++ = '\\'; *d++ = 'r'; break;
                case '\t': *d++ = '\\'; *d++ = 't'; break;
                case '\"': *d++ = '\\'; *d++ = '"'; break;
                case '\'': *d++ = '\\'; *d++ = '\''; break;
                case '\\': *d++ = '\\'; *d++ = '\\'; break;
                case '\a': *d++ = '\\'; *d++ = 'a'; break;
                case '\b': *d++ = '\\'; *d++ = 'b'; break;
                case '\f': *d++ = '\\'; *d++ = 'f'; break;
                case '\v': *d++ = '\\'; *d++ = 'v'; break;
                default:
                    if (uc < 32 || uc == 127) {
                        *d++ = '\\';
                        *d++ = '0' + ((uc >> 6) & 3);
                        *d++ = '0' + ((uc >> 3) & 7);
                        *d++ = '0' + (uc & 7);
                    } else {
                        *d++ = src[i];
                    }
                    break;
            }
        }
        return dest;
    }

    std::string utf8_safe_hex_encode(std::string_view src) {
        std::string dest;
        CEscapeAndAppendHexInternal(src, &dest, true);
        return dest;
    }

    void base64_encode(std::string_view src, std::string *dest) {
        size_t out_len = (src.size() + 2) / 3 * 4;
        strings_internal::STLStringResizeUninitialized(dest, out_len);
        dest->erase(xtext::binary_to_base64(
            src.data(), src.size(), &(*dest)[0], xtext::base64_default));
    }

    std::string base64_encode(std::string_view src) {
        std::string dest;
        base64_encode(src, &dest);
        return dest;
    }

    void web_safe_base64_encode(std::string_view src, std::string *dest) {
        size_t out_len = (src.size() + 2) / 3 * 4;
        strings_internal::STLStringResizeUninitialized(dest, out_len);
        dest->erase(xtext::binary_to_base64(
            src.data(), src.size(), &(*dest)[0], xtext::base64_url));
    }

    std::string web_safe_base64_encode(std::string_view src) {
        std::string dest;
        web_safe_base64_encode(src, &dest);
        return dest;
    }

    bool base64_decode(std::string_view src, std::string *dest) {
        size_t max_len = xtext::maximal_binary_length_from_base64(
            src.data(), src.size());
        strings_internal::STLStringResizeUninitialized(dest, max_len);
        auto r = xtext::base64_to_binary(
            src.data(), src.size(), &(*dest)[0], xtext::base64_default);
        if (!r.is_ok()) {
            dest->clear();
            return false;
        }
        dest->erase(r.count);
        return true;
    }

    bool web_safe_base64_decode(std::string_view src, std::string *dest) {
        size_t max_len = xtext::maximal_binary_length_from_base64(
            src.data(), src.size());
        strings_internal::STLStringResizeUninitialized(dest, max_len);
        auto r = xtext::base64_to_binary(
            src.data(), src.size(), &(*dest)[0], xtext::base64_url);
        if (!r.is_ok()) {
            dest->clear();
            return false;
        }
        dest->erase(r.count);
        return true;
    }

    bool hex_string_to_bytes(std::string_view hex, std::string *bytes) {
        size_t num_bytes = hex.size() / 2;
        if (hex.size() != num_bytes * 2) return false;

        strings_internal::STLStringResizeUninitialized(bytes, num_bytes);
        auto hex_p = hex.data();
        for (size_t i = 0; i < num_bytes; ++i) {
            int h1 = hex_digit_to_int(hex_p[2 * i]);
            int h2 = hex_digit_to_int(hex_p[2 * i + 1]);
            if (h1 < 0 || h2 < 0) return false;
            (*bytes)[i] = static_cast<char>((h1 << 4) | h2);
        }
        return true;
    }

    std::string bytes_to_hex_string(std::string_view from) {
        std::string result;
        result.resize(from.size() * 2);
        for (size_t i = 0; i < from.size(); ++i) {
            auto *hex = &kHexTable[static_cast<unsigned char>(from[i]) * 2];
            result[2 * i] = hex[0];
            result[2 * i + 1] = hex[1];
        }
        return result;
    }

}  // namespace xtext
