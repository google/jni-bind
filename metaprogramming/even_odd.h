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

#ifndef JNI_BIND_METAPROGRAMMING_EVEN_ODD_H_
#define JNI_BIND_METAPROGRAMMING_EVEN_ODD_H_

#include <tuple>

#include "concatenate.h"

namespace jni::metaprogramming {

// Metafunction to return only even elements.
struct Even {
  template <typename... Ts>
  struct EvenHelper {};

  template <>
  struct EvenHelper<std::tuple<>> {
    using type = std::tuple<>;
  };

  template <typename... Ts>
  struct OddHelper {};

  template <>
  struct OddHelper<std::tuple<>> {
    using type = std::tuple<>;
  };

  template <typename T, typename... Ts>
  struct OddHelper<std::tuple<T, Ts...>> {
    using type = typename EvenHelper<std::tuple<Ts...>>::type;
  };

  template <typename T, typename... Ts>
  struct EvenHelper<std::tuple<T, Ts...>> {
    using type = ConcatenateTup_t<std::tuple<T>,
                                  typename OddHelper<std::tuple<Ts...>>::type>;
  };

  template <typename... Ts>
  using type = typename EvenHelper<std::tuple<Ts...>>::type;
};

template <typename... Ts>
using Even_t = typename Even::template type<Ts...>;

// Metafunction to return only odd elements.
struct Odd {
  template <typename... Ts>
  struct OddHelper {
    using type = std::tuple<>;
  };

  template <typename T, typename... Ts>
  struct OddHelper<std::tuple<T, Ts...>> {
    using type = Even_t<Ts...>;
  };

  template <typename... Ts>
  using type = typename OddHelper<std::tuple<Ts...>>::type;
};

template <typename... Ts>
using Odd_t = typename Odd::template type<Ts...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_EVEN_ODD_H_
