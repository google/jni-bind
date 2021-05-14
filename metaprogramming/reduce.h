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

#ifndef JNI_BIND_METAPROGRAMMING_REDUCE_H_
#define JNI_BIND_METAPROGRAMMING_REDUCE_H_

#include <tuple>
#include <type_traits>

#include "combine.h"
#include "tuple_manipulation.h"

namespace jni::metaprogramming {

// Metafunction for a left fold reduction of a sequence of types with an
// operation. Single element sequences return the input type with no use of
// Operation.
template <class Operation>
struct Reduce {
  template <typename... Ts>
  struct SequenceGenerator {};

  template <typename T1, typename T2>
  using Operation_t = typename Operation::template type<T1, T2>;

  template <typename... Ts>
  using SequenceGenerator_t = typename SequenceGenerator<Ts...>::type;

  // Used for single element sequence.
  template <typename T1, typename... Ts>
  struct SequenceGenerator<T1, Ts...> {
    using type = T1;
  };

  // Terminal element in sequence.
  template <typename T1, typename T2>
  struct SequenceGenerator<T1, T2> {
    using type = Operation_t<T1, T2>;
  };

  template <typename T1, typename T2, typename... Ts>
  struct SequenceGenerator<T1, T2, Ts...> {
    using type = SequenceGenerator_t<Operation_t<T1, T2>, Ts...>;
  };

  template <typename... Args>
  using type = typename SequenceGenerator<Args...>::type;
};

template <typename Operation, typename... Ts>
using Reduce_t = typename Reduce<Operation>::template type<Ts...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_REDUCE_H_
