#include <xtext/simdutf/common_defs.h>
#include <xtext/simdutf/encoding_types.h>

namespace xtext {
namespace tests {
namespace reference {
// validate UTF-16
xtext_warn_unused bool validate_utf16(endianness utf16_endianness,
                                        const char16_t *buf,
                                        size_t len) noexcept;

} // namespace reference
} // namespace tests
} // namespace xtext
