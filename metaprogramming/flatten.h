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

#ifndef JNI_BIND_METAPROGRAMMING_FLATTEN_H_
#define JNI_BIND_METAPROGRAMMING_FLATTEN_H_

#include <tuple>

#include "concatenate.h"

namespace jni::metaprogramming {

// Metafunction that takes sets of tuples and reduces them to a single tuple.
// No element in the result set will be a tuple, any and all nested tuples will
// be flattened.
struct Flatten {
  template <typename... Ts>
  struct FlattenHelper {
    using type = std::tuple<>;
  };

  template <typename... TupleContents, typename... Ts>
  struct FlattenHelper<std::tuple<TupleContents...>, Ts...> {
    // If the type itself is a tuple, it may itself contain nested tuples.
    using type =
        ConcatenateTup_t<typename FlattenHelper<TupleContents...>::type,
                         typename FlattenHelper<Ts...>::type>;
  };

  template <typename NonTupleType, typename... Ts>
  struct FlattenHelper<NonTupleType, Ts...> {
    // The first type is for certain not a tuple or the alternate specialisation
    // would have been chosen. The value is wrapped back into a tuple, in order
    // to exploit ConcatenateTup_t and potentially flatten any remaining args.
    using type = ConcatenateTup_t<std::tuple<NonTupleType>,
                                  typename FlattenHelper<Ts...>::type>;
  };

  template <typename... Ts>
  using type = typename FlattenHelper<Ts...>::type;
};

template <typename... Ts>
using Flatten_t = typename Flatten::template type<Ts...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_FLATTEN_H_
