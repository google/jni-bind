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

#ifndef JNI_BIND_METAPROGRAMMING_SAME_H_
#define JNI_BIND_METAPROGRAMMING_SAME_H_

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Metafunction to take a sequence of values and emit std::true or std::false
// per element based on their type equality.
template <typename... Ts>
struct Same {
  template <typename... Us>
  using type =
      typename std::is_same<std::tuple<Ts...>, std::tuple<Us...>>::type;

  template <typename... Us>
  constexpr static bool value = type<Us...>::value;
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_SAME_H_
