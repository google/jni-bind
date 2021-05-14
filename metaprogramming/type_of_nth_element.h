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

#ifndef JNI_BIND_METAPROGRAMMING_TYPE_OF_NTH_ELEMENT_H_
#define JNI_BIND_METAPROGRAMMING_TYPE_OF_NTH_ELEMENT_H_

#include <tuple>
#include <type_traits>

#include "unwrap.h"

namespace jni::metaprogramming {

template <std::size_t N>
struct TypeOfNthElement {
  template <std::size_t idx, typename... T>
  struct PackUnroller;

  template <typename T0, typename... T>
  struct PackUnroller<0, T0, T...> {
    using type = T0;
  };

  template <std::size_t idx, typename T0, typename... T>
  struct PackUnroller<idx, T0, T...> {
    using type = typename PackUnroller<idx - 1, T...>::type;
  };

  template <typename... Ts>
  using type = typename PackUnroller<N, Ts...>::type;
};

template <std::size_t N, typename... Ts>
using TypeOfNthElement_t = typename TypeOfNthElement<N>::template type<Ts...>;

template <std::size_t N, typename TupType>
using TypeOfNthTupleElement_t = Unwrap_t<TypeOfNthElement<N>, TupType>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_TYPE_OF_NTH_ELEMENT_H_
