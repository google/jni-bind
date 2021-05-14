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

#ifndef JNI_BIND_METAPROGRAMMING_UNFURL_H_
#define JNI_BIND_METAPROGRAMMING_UNFURL_H_

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// "Unfurls" an index sequence onto a container, and provides convenience
// helpers
//
// |type| is a variadic metafunction that generates a tuple for each type
//  where each type is Container<Idx, Ts...> for [0, Is...).
//    i.e. std::tuple<Container<0, Ts...>, Container<1, Ts...>, ...>
//
// |val| is a variadic data member that is the fold of the val member of each
//   element of |type|. If |use_conjunction_fold_on_val| is true, the fold
//   operator will be &&, otherwise it will be ||.
template <template <std::size_t, typename...> class Container, std::size_t Is,
          bool use_conjunction_fold_on_val = true>
struct Unfurl {
  template <typename Ts, typename Indexes>
  struct Helper;

  template <typename... Ts, std::size_t... Indices>
  struct Helper<std::tuple<Ts...>, std::index_sequence<Indices...>> {
    using type = std::tuple<Container<Indices, Ts...>...>;

    static constexpr bool val = use_conjunction_fold_on_val
                                    ? (Container<Indices, Ts...>::val && ...)
                                    : (Container<Indices, Ts...>::val || ...);
  };

  template <typename... Ts>
  using type =
      typename Helper<std::tuple<Ts...>, std::make_index_sequence<Is>>::type;

  template <typename... Ts>
  static constexpr bool val =
      Helper<std::tuple<Ts...>, std::make_index_sequence<Is>>::val;
};

template <std::size_t Is, template <std::size_t, typename...> class Container,
          typename... Ts>
using Unfurl_t = typename Unfurl<Container, Is>::template type<Ts...>;

template <std::size_t Is, template <std::size_t, typename...> class Container,
          typename... Ts>
static constexpr bool UnfurlConjunction_v =
    Unfurl<Container, Is, true>::template val<Ts...>;

template <std::size_t Is, template <std::size_t, typename...> class Container,
          typename... Ts>
static constexpr bool UnfurlDisjunction_v =
    Unfurl<Container, Is, false>::template val<Ts...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_UNFURL_H_
