/*
 * Copyright 2024 Google LLC
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

#ifndef JNI_BIND_METAPROGRAMMING_STRING_LITERAL_H_
#define JNI_BIND_METAPROGRAMMING_STRING_LITERAL_H_

#ifdef __cplusplus
#if __cplusplus >= 202002L

#include <algorithm>
#include <cstddef>
#include <string_view>

namespace jni::metaprogramming {

// Inspired by Kevin Hartman's StringLiteral implementation.
// https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
//
// This class is not currently being used, but is being used to prototype
// changes that will be needed for consumers of `InvocableMap`. This class is
// not included by default for build because it requires C++20.
template <size_t N>
struct StringLiteral {
  constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }

  constexpr StringLiteral<N> Self() { return *this; }

  template <std::size_t U>
  constexpr bool operator==(const StringLiteral<U>& rhs) const {
    if constexpr (N != U) {
      return false;
    } else {
      return std::string_view{value} == std::string_view{rhs.value};
    }
  }

  template <std::size_t U>
  constexpr bool operator!=(const StringLiteral<U>& rhs) const {
    return !(*this == rhs);
  }

  char value[N];
};

template <size_t N>
StringLiteral(const char (&str)[N]) -> StringLiteral<N>;

#endif  // __cplusplus >= 202002L
#endif  // __cplusplus

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_STRING_LITERAL_H_
