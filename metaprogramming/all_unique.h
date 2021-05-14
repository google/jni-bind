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

#ifndef JNI_BIND_METAPROGRAMMING_ALL_UNIQUE_H_
#define JNI_BIND_METAPROGRAMMING_ALL_UNIQUE_H_

#include <type_traits>

#include "contains.h"

namespace jni::metaprogramming {

template <typename...>
struct AllUnique {
  static constexpr bool value = true;
  using type = std::bool_constant<value>;
};

template <typename T, typename... Ts>
struct AllUnique<T, Ts...> {
  static constexpr bool value =
      !Contains_v<T, Ts...> && AllUnique<Ts...>::value;
  using type = std::bool_constant<value>;
};

template <typename... Ts>
static constexpr bool AllUnique_v = AllUnique<Ts...>::value;

// Constexpr value implementation.
constexpr bool AllUniqueValues(...) { return true; }

template <typename T1, typename... Ts>
constexpr bool AllUniqueValues(const T1&& t1, const Ts&&... ts) {
  return (!ContainsValue(t1, ts...)) && AllUniqueValues(ts...);
}

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_ALL_UNIQUE_H_
