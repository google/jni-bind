/*
 * Copyright 2022 Google LLC
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
#ifndef JNI_BIND_METAPROGRAMMING_INDEX_H_
#define JNI_BIND_METAPROGRAMMING_INDEX_H_

#include <cstddef>
#include <tuple>
#include <utility>

#include "tuple_manipulation.h"

namespace jni::metaprogramming {

template <std::size_t Idx, typename T>
struct IndexEntry {
  static constexpr std::size_t kIdx = Idx;
  using type = T;
};

struct Index {
  template <typename T, typename... Ts>
  struct Helper;

  template <std::size_t... Idxs, typename... Ts>
  struct Helper<std::index_sequence<Idxs...>, Ts...> {
    using type = std::tuple<IndexEntry<Idxs, Ts>...>;
  };

  template <typename... Ts>
  using type =
      typename Helper<std::make_index_sequence<sizeof...(Ts)>, Ts...>::type;
};

template <typename... Ts>
using Index_t = typename Index::template type<Ts...>;

template <typename Tup>
using Index_Tup = TupleUnroller_t<Index, Tup>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_INDEX_H_
