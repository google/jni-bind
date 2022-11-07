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

#ifndef JNI_BIND_METAPROGRAMMING_MONOID_H_
#define JNI_BIND_METAPROGRAMMING_MONOID_H_

#include <tuple>
#include <type_traits>

#include "per_element.h"
#include "tuple_manipulation.h"
#include "type_of_nth_element.h"

namespace jni::metaprogramming {

// "Invokes" a metafunction (i.e. a type alias for Func::type).
// Handles both variadic or non-variadic args for a consistent syntax.
template <typename Func>
struct Invoke {
  template <typename, typename Enable = void>
  struct InvokeHelper {};

  // Variadic forwarding.
  template <typename... Ts>
  struct InvokeHelper<std::tuple<Ts...>> {
    using type = typename Func::template type<Ts...>;
  };

  // Non-variadic forwarding.
  template <typename Ts>
  struct InvokeHelper<std::tuple<Ts>> {
    using type = typename Func::template type<Ts>;
  };

  template <typename... Args>
  using type = typename InvokeHelper<std::tuple<Args...>>::type;
};

// Helper alias to invoke a metafunction with args as a variadic pack.
template <typename Func, typename... Args>
using Invoke_t = typename Invoke<Func>::template type<Args...>;

// Helper alias to take a pack from a tuple and use it as args to invocation.
template <typename Func, typename TupleOfArgs>
using InvokeAsPack_t = TupleUnroller_t<Invoke<Func>, TupleOfArgs>;

// Helper alias to invoke a metafunction with args contained in a tuple. Each
// tuple element will itself be an arg.
template <typename Func, typename TupleOfArgs>
using InvokePerTupArg_t = TupleUnroller_t<PerElement<Func>, TupleOfArgs>;

// Helper alias to invoke a metafunction with args contained in each element of
// a tuple. Each tuple element will itself be a tuple of arguments.
template <typename Func, typename TupleOfTupleOfArgs>
using InvokePerTupArgAsPack_t =
    InvokePerTupArg_t<Unwrap<Func>, TupleOfTupleOfArgs>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_MONOID_H_
