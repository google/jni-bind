/*
 * Copyright 2023 Google LLC
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
#ifndef JNI_BIND_METAPROGRAMMING_STRING_CONTAINS_H_
#define JNI_BIND_METAPROGRAMMING_STRING_CONTAINS_H_

#include "lambda_string.h"

namespace jni::metaprogramming {

template <char sought>
struct StringContains {
  template <const std::string_view& str, typename IndexSequence>
  struct Helper;

  template <const std::string_view& str, std::size_t... Is>
  struct Helper<str, std::index_sequence<Is...>> {
    static constexpr bool val = ((str[Is] == sought) || ...);
  };

  template <const std::string_view& str>
  static constexpr bool val =
      Helper<str, std::make_index_sequence<str.length()>>::val;
};

template <const std::string_view& str, char sought>
static constexpr bool StringContains_v =
    StringContains<sought>::template val<str>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_STRING_CONTAINS_H_
