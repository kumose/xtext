#include <xtext/simdutf/common_defs.h>
#include <xtext/simdutf/encoding_types.h>

namespace xtext {
namespace tests {
namespace reference {
xtext_warn_unused bool
validate_utf16_to_latin1(xtext::endianness utf16_endianness,
                         const char16_t *buf, size_t len) noexcept;

}
} // namespace tests
} // namespace xtext
