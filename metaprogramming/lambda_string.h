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

#ifndef JNI_BIND_METAPROGRAMMING_LAMBDA_STRING_H_
#define JNI_BIND_METAPROGRAMMING_LAMBDA_STRING_H_

#include <string_view>
#include <type_traits>
#include <utility>

#define STR(x) []() { return x; }

namespace jni::metaprogramming {

template <typename Identifier>
using identifier_type = decltype(std::declval<Identifier>()());

constexpr std::size_t ConstexprStrlen(const char* str) {
  return str[0] == 0 ? 0 : ConstexprStrlen(str + 1) + 1;
}

struct StringAsTypeBase {};

// Represents a string by embedding a sequence of characters in a type.
template <char... chars>
struct StringAsType : StringAsTypeBase {
  static constexpr char static_chars[] = {chars..., 0};
  static constexpr std::string_view chars_as_sv = {static_chars,
                                                   sizeof...(chars)};
};

template <typename Identifier, std::size_t... I>
constexpr auto LambdaToStr(Identifier id, std::index_sequence<I...>) {
  return StringAsType<id()[I]...>{};
}

template <
    typename Identifier,
    std::enable_if_t<std::is_same_v<identifier_type<Identifier>, const char*>,
                     int> = 0>
constexpr auto LambdaToStr(Identifier id) {
  return LambdaToStr(id, std::make_index_sequence<ConstexprStrlen(id())>{});
}

template <typename NameLambda>
using LambdaStringToType = decltype(LambdaToStr(std::declval<NameLambda>()));

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_LAMBDA_STRING_H_
