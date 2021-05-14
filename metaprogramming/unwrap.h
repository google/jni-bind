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

#ifndef JNI_BIND_METAPROGRAMMING_UNWRAP_H_
#define JNI_BIND_METAPROGRAMMING_UNWRAP_H_

#include "tuple_manipulation.h"

namespace jni::metaprogramming {

// Metafunction to forward all args in a tuple on to a metafunction.
// It's useful if you want to "derefence" arguments from another meta function.
template <typename Func>
struct Unwrap {
  template <typename TupType>
  using type = TupleUnroller_t<Func, TupType>;
};

template <typename Func, typename... Ts>
using Unwrap_t = typename Unwrap<Func>::template type<Ts...>;

template <typename Func, typename... Ts>
static constexpr auto Unwrap_v = Unwrap_t<Func, Ts...>::value;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_UNWRAP_H_
