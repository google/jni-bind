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

#ifndef JNI_BIND_METAPROGRAMMING_TYPE_INDEX_MASK_H_
#define JNI_BIND_METAPROGRAMMING_TYPE_INDEX_MASK_H_

#include <tuple>
#include <utility>

#include "type_of_nth_element.h"

namespace jni::metaprogramming {

// Metafunction to take a sequence of indices, and produce the corresponding
// elements in sequences of tuples.
template <std::size_t... sizes>
struct TypeIndexMask {
  template <typename, typename... Ts>
  struct GetHelper {};

  template <std::size_t... Is, typename... Ts>
  struct GetHelper<std::index_sequence<Is...>, Ts...> {
    using type = std::tuple<TypeOfNthTupleElement_t<Is, Ts>...>;
  };

  template <typename... Ts>
  using Get = typename GetHelper<std::index_sequence<sizes...>, Ts...>::type;

  template <typename>
  struct SequenceGenerator {};

  template <typename... Ts>
  struct SequenceGenerator<std::tuple<Ts...>> {
    using type = Get<Ts...>;
  };

  template <typename Tup>
  using type = typename SequenceGenerator<Tup>::type;
};

template <typename TupOfTypes, std::size_t... sizes>
using TypeTupFromIndexMask_t =
    typename TypeIndexMask<sizes...>::template type<TupOfTypes>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_TYPE_INDEX_MASK_H_
