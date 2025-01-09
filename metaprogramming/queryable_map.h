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

#ifndef JNI_BIND_METAPROGRAMMING_QUERYABLE_MAP_H
#define JNI_BIND_METAPROGRAMMING_QUERYABLE_MAP_H

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace jni::metaprogramming {

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, typename MemberT, MemberT nameable_member,
          typename IndexSequenceType>
class QueryableMapBase {};

// This is an interface that can be inherited from to expose an
// operator["name"]. It provides compile time string index lookup with no macros
// although it is dependent on a clang extension.
//
// To use this API, inherit from this class using template types as follows:
//
// |CrtpBase|: The name of the class inheriting from the map.  This class
//   will inherit an operator[].  It must implement this exact signature:
//
//    template <std::size_t I>
//    auto QueryableMapCall(const char* key);
//
// |tup_container_v| is a static instance of an object whose |nameable_member|
//   contains a public field called name_.  It might seem strange not to
//   directly pass a const auto&, however, this prevents accessing subobjects.
//
// The motivation for using inheritance as opposed to a simple member is that
// the the const char cannot be propagated without losing its constexpr-ness,
// and so the clang extension can no longer restrict function candidates.
template <typename CrtpBase, const auto& tup_container_v,
          std::size_t container_size, typename TupContainerT, typename MemberT,
          MemberT nameable_member>
class QueryableMap
    : public QueryableMapBase<CrtpBase, tup_container_v, TupContainerT, MemberT,
                              nameable_member,
                              std::make_index_sequence<container_size>> {};

template <typename CrtpBase, const auto& tup_container_v, typename MemberT,
          MemberT nameable_member>
using QueryableMap_t = QueryableMap<
    CrtpBase, tup_container_v,
    std::tuple_size_v<
        std::decay_t<decltype((tup_container_v.*nameable_member))>>,
    std::decay_t<decltype(tup_container_v)>, MemberT, nameable_member>;

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, typename MemberT, MemberT nameable_member,
          std::size_t I>
class QueryableMapEntry;

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, typename MemberT, MemberT nameable_member,
          std::size_t... idxs>
class QueryableMapBase<CrtpBase, tup_container_v, TupContainerT, MemberT,
                       nameable_member, std::index_sequence<idxs...>>
    : public QueryableMapEntry<CrtpBase, tup_container_v, TupContainerT,
                               MemberT, nameable_member, idxs>... {
 public:
#if __clang__
  using QueryableMapEntry<CrtpBase, tup_container_v, TupContainerT,

                          MemberT, nameable_member, idxs>::operator[]...;

  using QueryableMapEntry<CrtpBase, tup_container_v, TupContainerT,

                          MemberT, nameable_member, idxs>::Contains...;
#endif  // __clang__

  // Will select subclass specialisations if present.
  constexpr bool Contains(const char* key) { return false; }
};

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, typename MemberT, MemberT nameable_member,
          std::size_t I>
class QueryableMapEntry {
 public:
#if __clang__
  // This function blurs the distinction between type and value space.  The
  // clang extension allows the key to be wrapped in a constexpr way.  This
  // allows for string to string comparison based on the static value the class
  // is templated by.
  //
  // The reason the TypeMap interface requires inheritance as opposed to simply
  // holding an instance of this map (like you would with a regular hash map) is
  // the constexpr-ness of the string can't be propagated.  This essentially
  // means you get one shot at defining the function.
  constexpr auto operator[](const char* key) __attribute__((
      enable_if(std::string_view(key) ==
                    std::get<I>(tup_container_v.*nameable_member).name_,
                ""))) {
    static_assert(std::is_base_of_v<QueryableMapEntry, CrtpBase>,
                  "You must derive from the invocable map.");

    return (*static_cast<CrtpBase*>(this)).template QueryableMapCall<I>(key);
  }

  constexpr bool Contains(const char* key) __attribute__((
      enable_if(std::string_view(key) ==
                    std::get<I>(tup_container_v.*nameable_member).name_,
                ""))) {
    return true;
  }
#endif  // __clang__
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_QUERYABLE_MAP_H
