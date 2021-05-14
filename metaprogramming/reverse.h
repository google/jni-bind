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

#ifndef JNI_BIND_METAPROGRAMMING_REVERSE_H_
#define JNI_BIND_METAPROGRAMMING_REVERSE_H_

#include <tuple>

#include "concatenate.h"
#include "tuple_manipulation.h"

namespace jni::metaprogramming {

struct Reverse {
  template <typename... Ts>
  struct Helper {
    using type = std::tuple<>;
  };

  template <typename... Ts>
  using Helper_t = typename Helper<std::tuple<Ts...>>::type;

  template <typename T1, typename... Ts>
  struct Helper<std::tuple<T1, Ts...>> {
    using type = ConcatenateTup_t<Helper_t<Ts...>, std::tuple<T1>>;
  };

  template <typename... Ts>
  using type = Helper_t<Ts...>;
};

template <typename... Ts>
using Reverse_t = typename Reverse::template type<Ts...>;

template <typename Tup>
using Reverse_Tup = TupleUnroller_t<Reverse, Tup>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_REVERSE_H_
