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
#ifndef JNI_BIND_METAPROGRAMMING_REPLACE_STRING_H_
#define JNI_BIND_METAPROGRAMMING_REPLACE_STRING_H_

#include <string_view>
#include <utility>

#include "lambda_string.h"

namespace jni::metaprogramming {

template <char sought_char, char new_char>
struct Replace {
  template <const std::string_view& str, typename IndexSequence>
  struct Helper;

  template <const std::string_view& str, std::size_t... Is>
  struct Helper<str, std::index_sequence<Is...>> {
    static constexpr std::string_view val = StringAsType<(
        str[Is] == sought_char ? new_char : str[Is])...>::chars_as_sv;
  };

  template <const std::string_view& str>
  static constexpr std::string_view val =
      Helper<str, std::make_index_sequence<str.length()>>::val;
};

template <const std::string_view& str, char sought_char, char new_char>
static constexpr auto Replace_v =
    Replace<sought_char, new_char>::template val<str>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_REPLACE_STRING_H_
