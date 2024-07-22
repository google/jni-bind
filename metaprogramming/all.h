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

#ifndef JNI_BIND_METAPROGRAMMING_ALL_H_
#define JNI_BIND_METAPROGRAMMING_ALL_H_

#include <type_traits>

namespace jni::metaprogramming {

template <typename Func>
struct All {
  template <typename... Ts>
  using type =
      typename std::conjunction<typename Func::template type<Ts>...>::type;
};

template <typename Func, typename... Ts>
using All_t = typename All<Func>::template type<Ts...>;

template <typename Func, typename... Ts>
static constexpr bool All_v = All_t<Func, Ts...>::value;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_ALL_H_
