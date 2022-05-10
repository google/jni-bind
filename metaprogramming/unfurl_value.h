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

#ifndef JNI_BIND_METAPROGRAMMING_CONDITIONAL_H_
#define JNI_BIND_METAPROGRAMMING_CONDITIONAL_H_

#include <type_traits>
#include <utility>
#include <array>

namespace jni::metaprogramming {

// Exposes a value that applies the given lambda to each value in the parameter
// pack.
template <auto lambda>
struct UnfurlValue {
  template <typename T, auto... vals>
  struct Helper;

  template <std::size_t... Is, auto... vals >
  struct Helper<std::index_sequence<Is...>, vals...> {
    static_assert(sizeof...(Is) == 3);
    static constexpr std::array val {
      (lambda(Is, vals...))...
    };
  };

  template <auto... vals >
  static constexpr auto val {
    Helper<std::make_index_sequence<sizeof...(vals)>, vals...>::val };
};

template <auto lambda, auto... vals>
constexpr auto kUnfurlValue { UnfurlValue<lambda>::template val<vals...> };

}  // namespace jni::metaprogramming


#endif  // JNI_BIND_METAPROGRAMMING_CONDITIONAL_H_
