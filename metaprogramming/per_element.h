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

#ifndef JNI_BIND_METAPROGRAMMING_PER_ELEMENT_H_
#define JNI_BIND_METAPROGRAMMING_PER_ELEMENT_H_

#include <tuple>

#include "tuple_manipulation.h"
#include "type_of_nth_element.h"

namespace jni::metaprogramming {

// Metafunction that takes variadic arguments and applies a bound metafunction
// to each element.  Type exposed will always be a tuple.  This is intended for
// use with a Monoid that you might want to apply across a set of every
// argument.  Otherwise, you would use the argument directly.
template <typename BoundType>
struct PerElement {
  template <typename Args, typename Is>
  struct Sequence {};

  template <typename... Args, std::size_t... Is>
  struct Sequence<std::tuple<Args...>, std::index_sequence<Is...>> {
    using type = std::tuple<
        typename BoundType::template type<TypeOfNthElement_t<Is, Args...>>...>;
  };

  template <typename... Ts>
  using type = typename Sequence<std::tuple<Ts...>,
                                 std::make_index_sequence<sizeof...(Ts)>>::type;
};

template <typename Func, typename... Ts>
using PerElement_t = typename PerElement<Func>::template type<Ts...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_PER_ELEMENT_H_
