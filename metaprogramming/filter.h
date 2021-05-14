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

#ifndef JNI_BIND_METAPROGRAMMING_FILTER_H_
#define JNI_BIND_METAPROGRAMMING_FILTER_H_

#include <tuple>
#include <type_traits>

#include "combine.h"
#include "concatenate.h"
#include "conditional.h"
#include "reduce.h"
#include "same.h"

namespace jni::metaprogramming {

// Metafunction for filtering elements that do or don't satisfy a given
// metafunction.  The metafunction must expose a static constepxr boolean
// "value" member (see Conditional).
template <typename Func>
struct Filter {
  template <typename... Ts>
  struct FilterHelper {};

  template <typename... Ts>
  struct FilterHelper<std::tuple<Ts...>> {
    using type = std::tuple<>;
  };

  template <typename T1, typename... Ts>
  struct FilterHelper<std::tuple<T1, Ts...>> {
    using type =
        ConcatenateTup_t<Conditional_t<Func, T1>,
                         typename FilterHelper<std::tuple<Ts...>>::type>;
  };

  template <typename... Ts>
  using type = typename FilterHelper<std::tuple<Ts...>>::type;
};

template <typename Func, typename... Ts>
using Filter_t = typename Filter<Func>::template type<Ts...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_FILTER_H_
