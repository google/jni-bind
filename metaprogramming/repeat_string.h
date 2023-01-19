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
#ifndef JNI_BIND_METAPROGRAMMING_REPEAT_H_
#define JNI_BIND_METAPROGRAMMING_REPEAT_H_

#include "string_concatenate.h"

namespace jni::metaprogramming {

template <std::size_t repeat_cnt>
struct RepeatString {
  template <const std::string_view& val_to_repeat>
  static constexpr std::string_view val = metaprogramming::StringConcatenate_v<
      val_to_repeat, RepeatString<repeat_cnt - 1>::template val<val_to_repeat>>;
};

template <>
struct RepeatString<0> {
  template <const std::string_view& val_to_repeat>
  static constexpr std::string_view val{""};
};

template <std::size_t repeat_cnt, const std::string_view& val_to_repeat>
static constexpr auto RepeatString_v{
    RepeatString<repeat_cnt>::template val<val_to_repeat>};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_REPEAT_H_
