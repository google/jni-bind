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

#ifndef JNI_BIND_METAPROGRAMMING_CONCATENATE_H_
#define JNI_BIND_METAPROGRAMMING_CONCATENATE_H_

#include <tuple>

#include "combine.h"
#include "reduce.h"

namespace jni::metaprogramming {

// Metafunction to generate a concatenation of variadic args.
struct Concatenate {
  template <typename... Ts>
  using type = std::tuple<Ts...>;
};

template <typename... Ts>
using Concatenate_t = typename Concatenate::template type<Ts...>;

// Metafunction to concatenate tuples into a single tuple.
template <typename... Ts>
struct ConcatenateTup {
  static_assert(sizeof...(Ts) != 0, "ConcatenateTup must only take tuples.");
};

template <typename... T1s>
struct ConcatenateTup<std::tuple<T1s...>> {
  using type = std::tuple<T1s...>;
};

template <typename... Tups>
using ConcatenateTup_t = Reduce_t<Combine, Tups...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_CONCATENATE_H_
