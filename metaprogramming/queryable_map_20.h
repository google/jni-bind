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

#ifndef JNI_BIND_METAPROGRAMMING_QUERYABLE_MAP_20_H
#define JNI_BIND_METAPROGRAMMING_QUERYABLE_MAP_20_H

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "modified_max.h"
#include "string_literal.h"

namespace jni::metaprogramming {

// This is an interface that can be inherited from to expose an
// operator.Access<"name">(). It provides compile time string index lookup with
// no macros although it is dependent on a clang extension.
//
// To use this API, inherit from this class using template types as follows:
//
// |CrtpBase|: The name of the class inheriting from the map.  This class
//   will inherit an operator[].  It must implement this exact signature:
//
//    template <std::size_t I, StringLiteral key_literal>
//    auto QueryableMap20Call();
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
class QueryableMap20 {
#if __cplusplus >= 202002L
 private:
  template <StringLiteral string_literal, std::size_t Idx>
  constexpr auto Do() {
    return (*static_cast<CrtpBase*>(this))
        .template QueryableMap20Call<Idx, string_literal>();
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

 public:
  template <StringLiteral string_literal>
  constexpr auto Access() {
    return Do<string_literal,
              SelectCandidate(
                  string_literal,
                  std::make_index_sequence<std::tuple_size_v<std::decay_t<
                      decltype(tup_container_v.*nameable_member)>>>())>();
  }
#endif  // __cplusplus >= 202002L
};

template <typename CrtpBase, const auto& tup_container_v,
          const auto std::decay_t<decltype(tup_container_v)>::* nameable_member>
using QueryableMap20_t =
    QueryableMap20<CrtpBase, tup_container_v,
                   std::decay_t<decltype(tup_container_v)>, nameable_member>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_QUERYABLE_MAP_20_H
