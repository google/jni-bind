/*
 * Copyright 2024 Google LLC
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

// This class enables compile time lookup that perfectly forward arguments
// to a method named `Call`.  This is a C++ 20 version of InvocableMap.
//
// This is an interface that can be inherited from to expose a method named
// `Call`.  It provides compile time string index lookup with no macros.
//
// To use this API, inherit from this class using template types as follows:
//
// |CrtpBase|: The name of the class inheriting from the map.  This class
//   will inherit an operator().  It must implement this exact signature:
//
//    template <std::size_t I,  StringLiteral key_literal, typename... Args>
//    auto InvocableMap20Call(Args&&... args);
//
//   If i is the index where |tup_container_v.*nameable_member|.name_ == key,
//     then InvocableMap20Call will forward the args from operator() with the
//     same args.  Static memory can be used in this function call and it will
//     be unique because of the I non-type template parameter.
//
// |tup_container_v| is a static instance of an object whose |nameable_member|
//   contains a public field called name_.  It might seem strange not to
//   directly pass a const auto&, however, this prevents accessing subobjects.
//
// The motivation for using inheritance as opposed to a simple member is that
// the the const char cannot be propagated without losing its constexpr-ness,
// and so the clang extension can no longer restrict function candidates.
template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::* nameable_member>
class InvocableMap20 {
#if __cplusplus >= 202002L
 public:
  template <StringLiteral key_literal, std::size_t Idx, typename... Args>
  constexpr auto Do(Args&&... args) {
    return (*static_cast<CrtpBase*>(this))
        .template InvocableMap20Call<Idx, key_literal, Args...>(
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
#endif  // __cplusplus >= 202002L
};

template <typename CrtpBase, const auto& tup_container_v,
          const auto std::decay_t<decltype(tup_container_v)>::* nameable_member>
using InvocableMap20_t =
    InvocableMap20<CrtpBase, tup_container_v,
                   std::decay_t<decltype(tup_container_v)>, nameable_member>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_INVOCABLE_MAP_20_H
