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

#ifndef JNI_BIND_METAPROGRAMMING_INVOCABLE_MAP_20_H
#define JNI_BIND_METAPROGRAMMING_INVOCABLE_MAP_20_H

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "modified_max.h"
#include "string_literal.h"

namespace jni::metaprogramming {

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::* nameable_member>
class InvocableMap20 {
 public:
  template <StringLiteral string_literal, std::size_t Idx, typename... Args>
  constexpr auto Do(Args&&... args) {
    return (*static_cast<CrtpBase*>(this))
        .template InvocableMapCall<Idx, Args...>(string_literal.value,
                                                 std::forward<Args>(args)...);
  }

  template <std::size_t N, std::size_t... Is>
  static constexpr std::size_t SelectCandidate(StringLiteral<N> string_literal,
                                               std::index_sequence<Is...>) {
    return ModifiedMax(
        {((std::get<Is>(tup_container_v.*nameable_member).name_ ==
           std::string_view{string_literal.value})
              ? std::size_t{Is}
              : kNegativeOne)...,
         kNegativeOne});
  }

  template <StringLiteral string_literal, typename... Args>
  constexpr auto Call(Args&&... args) {
    return Do<string_literal,
              SelectCandidate(
                  string_literal,
                  std::make_index_sequence<std::tuple_size_v<std::decay_t<
                      decltype(tup_container_v.*nameable_member)>>>())>(
        std::forward<Args>(args)...);
  }
};

template <typename CrtpBase, const auto& tup_container_v,
          const auto std::decay_t<decltype(tup_container_v)>::* nameable_member>
using InvocableMap20_t =
    InvocableMap20<CrtpBase, tup_container_v,
                   std::decay_t<decltype(tup_container_v)>, nameable_member>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_INVOCABLE_MAP_20_H
