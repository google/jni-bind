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

#ifndef JNI_BIND_METAPROGRAMMING_CONDITIONAL_H_
#define JNI_BIND_METAPROGRAMMING_CONDITIONAL_H_

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// For a given Func which takes values, returns std::tuple<Ts...> if
// Func<Ts...>::value is true, otherwise returns std::tuple. Func must have a
// boolean value member.
template <typename Func>
struct Conditional {
  // The output is as a std::tuple<T> or std::tuple<> instead of just void in
  // order to differentiate when you might want to actually represent the type
  // void.
  template <typename... Ts>
  using type = std::conditional_t<Func::template type<Ts...>::value,
                                  std::tuple<Ts...>, std::tuple<>>;
};

template <typename Func, typename... Ts>
using Conditional_t = typename Conditional<Func>::template type<Ts...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_CONDITIONAL_H_
