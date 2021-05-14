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

#ifndef JNI_BIND_METAPROGRAMMING_UNIQUE_SET_H_
#define JNI_BIND_METAPROGRAMMING_UNIQUE_SET_H_

#include "contains.h"
#include "reverse.h"

namespace jni::metaprogramming {

// Builds a stable (i.e. first unique type is preserved) set of unique types
// from a set.
struct UniqueSet {
  template <typename... Ts>
  struct Helper {
    using type = std::tuple<>;
  };

  template <typename... Ts>
  using Helper_t = typename Helper<Ts...>::type;

  // Strips the first element from the first set if it's not unique.
  template <typename T, typename... Ts>
  struct Helper<T, Ts...> {
    using type =
        std::conditional_t<!Contains_v<T, Ts...>,
                           ConcatenateTup_t<std::tuple<T>, Helper_t<Ts...>>,
                           Helper_t<Ts...>>;
  };

  template <typename... Ts>
  using InputSetReversed = Reverse_t<Ts...>;

  template <typename... Ts>
  using ReverseSetStrippedOfDuplicates =
      typename TupleToType_t<InputSetReversed<Ts...>, Helper>::type;

  template <typename... Ts>
  using type = Reverse_Tup<ReverseSetStrippedOfDuplicates<Ts...>>;
};

template <typename... Ts>
using UniqueSet_t = typename UniqueSet::template type<Ts...>;

template <typename Tup>
using UniqueSet_Tup = TupleUnroller_t<UniqueSet, Tup>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_UNIQUE_SET_H_
