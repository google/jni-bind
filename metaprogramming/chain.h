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

#ifndef JNI_BIND_METAPROGRAMMING_CHAIN_H_
#define JNI_BIND_METAPROGRAMMING_CHAIN_H_

#include "invoke.h"
#include "tuple_manipulation.h"

namespace jni::metaprogramming {

// Metafunction to invoke a metafunction and forward its output to another
// metafunction (or sequence of metafunctions).  Note, unlike typical functional
// composition, this is left to right, not right to left.
template <typename... Funcs>
struct Chain {
  static_assert(sizeof...(Funcs) > 0, "Chains must have at least 1 element.");
};

template <typename FinalFunc>
struct Chain<FinalFunc> {
  template <typename... FinalChainedArgs>
  using type = Invoke_t<FinalFunc, FinalChainedArgs...>;
};

template <typename FirstFunc, typename... Funcs>
struct Chain<FirstFunc, Funcs...> {
  template <typename... ArgsForFirstFunc>
  using type =
      Invoke_t<Chain<Funcs...>, Invoke_t<FirstFunc, ArgsForFirstFunc...>>;
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_CHAIN_H_
