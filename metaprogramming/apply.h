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

#ifndef JNI_BIND_METAPROGRAMMING_APPLY_H_
#define JNI_BIND_METAPROGRAMMING_APPLY_H_

#include <tuple>

#include "tuple_manipulation.h"

namespace jni::metaprogramming {

// Forwards all arguments to a metafunction.  Note, Apply<Func> is itself a type
// whose |type| (aka its metafunction's invocation).
template <template <typename...> class Function>
struct Apply {
  template <typename... Ts>
  using type = Function<Ts...>;
};

template <template <typename...> class Function, typename... Ts>
using Apply_t = typename Apply<Function>::template type<Ts...>;

// Forwards all arguments from a tuple to a metafunction.
// i.e. Apply_t<Func, Ts...> == ApplyTup_t<Metafunc, std::tuple<Ts...>>.
template <template <typename...> class Function, typename TupleType>
using ApplyTup_t = TupleUnroller_t<Apply<Function>, TupleType>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_APPLY_H_
