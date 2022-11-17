/*
 * Copyright 2022 Google LLC
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

#ifndef JNI_BIND_METAPROGRAMMING_PER_ELEMENT_VAL_H_
#define JNI_BIND_METAPROGRAMMING_PER_ELEMENT_VAL_H_

#include <array>

namespace jni::metaprogramming {

template <typename Lambda>
struct PerElementVal {
  template <const auto... vs>
  struct Helper {
    static constexpr std::array val{Lambda::template val<vs>...};
  };

  template <const auto... vs>
  static constexpr std::array<bool, sizeof...(vs)> val{Helper<vs...>::val};
};

template <typename Lambda, const auto... vs>
static constexpr auto PerElementVal_v =
    PerElementVal<Lambda>::template val<vs...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_PER_ELEMENT_VAL_H_
