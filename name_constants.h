#ifndef JNI_BIND_NAME_CONSTANTS_H_
#define JNI_BIND_NAME_CONSTANTS_H_

#include <string_view>

namespace jni {

////////////////////////////////////////////////////////////////////////////////
// Constants for signature generation.
////////////////////////////////////////////////////////////////////////////////

static constexpr std::string_view kLeftParenthesis{"("};
static constexpr std::string_view kRightParenthesis{")"};
static constexpr std::string_view kInit{"<init>"};
static constexpr std::string_view kComma{","};
static constexpr std::string_view kSemiColon{";"};
static constexpr std::string_view kLetterL{"L"};

}  // namespace jni

#endif  // JNI_BIND_NAME_CONSTANTS_H_
