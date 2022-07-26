/*
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JNI_BIND_NAME_CONSTANTS_H_
#define JNI_BIND_NAME_CONSTANTS_H_

#include <string_view>
#include <type_traits>

namespace jni {

// Metafunction that returns either "" if a member called |name_| isn't
// present, or a constexpr std::string_view of the name if it is.
template <const auto&, typename Enable = void>
struct NameOrNothing {
  static constexpr std::string_view val{""};
};

template <const auto& val_>
struct NameOrNothing<val_, std::void_t<decltype(val_.name_)>> {
  static constexpr std::string_view val{val_.name_};
};

template <const auto& val>
static constexpr auto NameOrNothing_v = NameOrNothing<val>::val;

////////////////////////////////////////////////////////////////////////////////
// Constants for signature generation.
////////////////////////////////////////////////////////////////////////////////

static constexpr std::string_view kLeftBracket{"["};
static constexpr std::string_view kRightBracket{"]"};
static constexpr std::string_view kLeftParenthesis{"("};
static constexpr std::string_view kRightParenthesis{")"};
static constexpr std::string_view kInit{"<init>"};
static constexpr std::string_view kComma{","};
static constexpr std::string_view kSemiColon{";"};
static constexpr std::string_view kLetterL{"L"};

}  // namespace jni

#endif  // JNI_BIND_NAME_CONSTANTS_H_
