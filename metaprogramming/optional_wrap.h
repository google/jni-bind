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

#ifndef JNI_BIND_METAPROGRAMMING_OPTIONAL_WRAP_H_
#define JNI_BIND_METAPROGRAMMING_OPTIONAL_WRAP_H_

#include <optional>
#include <tuple>

namespace jni::metaprogramming {

template <typename>
struct OptionalTup {};

// Takes a _tuple_ of types and returns a tuple with the same types except
// wrapped in std::optional.
template <typename... Ts>
struct OptionalTup<std::tuple<Ts...>> {
  using type = std::tuple<std::optional<Ts>...>;
};

template <typename... Ts>
using OptionalTup_t = typename OptionalTup<Ts...>::type;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_OPTIONAL_WRAP_H_
