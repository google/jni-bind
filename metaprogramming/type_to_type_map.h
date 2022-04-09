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

#ifndef JNI_BIND_METAPROGRAMMING_TYPE_TO_TYPE_MAP_H_
#define JNI_BIND_METAPROGRAMMING_TYPE_TO_TYPE_MAP_H_

#include <tuple>

#include "concatenate.h"
#include "contains.h"
#include "even_odd.h"
#include "find_idx_of_val.h"
#include "same.h"
#include "tuple_manipulation.h"
#include "type_of_nth_element.h"

namespace jni::metaprogramming {

// Maps types to other types.
// Keys do not need to be unique, although queries only return the first entry.
template <typename Tup1, typename Tup2>
struct TypeToTypeMap {};

template <typename... Keys_, typename... Values_>
struct TypeToTypeMap<std::tuple<Keys_...>, std::tuple<Values_...>> {
  static_assert(sizeof...(Keys_) == sizeof...(Values_),
                "Keys must be an equal size to the value.");

  using Keys = std::tuple<Keys_...>;
  using Values = std::tuple<Values_...>;
  using Invert = TypeToTypeMap<std::tuple<Values_...>, std::tuple<Keys_...>>;

  template <typename Comparator>
  using type =
      TypeOfNthElement_t<FindIdxOfValWithComparator_idx<Comparator, Keys_...>,
                         Values_...>;
};

template <typename TypeToTypeMap>
using TypeToTypeMap_Keys_t = typename TypeToTypeMap::Keys;

template <typename TypeToTypeMap>
using TypeToTypeMap_Values_t = typename TypeToTypeMap::Values;

// Metafunction to invert the map from keys->vals to vals->keys.
// Note, keys do not need to be unique.
template <typename TypeToTypeMap>
using TypeToTypeMap_Invert = typename TypeToTypeMap::Invert;

template <typename TypeToTypeMap, typename Query>
using TypeToTypeMapQuery_t = typename TypeToTypeMap::template type<Same<Query>>;

// Queries a type map using a custom comparator,
// If multiple keys satisfy, the first is returned.
template <typename TypeToTypeMap, typename Comparator>
using TypeToTypeMapQueryWithComparator_t =
    typename TypeToTypeMap::template type<Comparator>;

// Builds a TypeToTypeMap from interleaved Keys and Values.
template <typename... Ts>
using TypeToTypeMapFromKeyValues_t = TypeToTypeMap<Even_t<Ts...>, Odd_t<Ts...>>;

template <typename TupleOfKeyValuePairs>
using TypeToTypeMapFromKeyValuesTup_t =
    TupleToType_t<TupleOfKeyValuePairs, TypeToTypeMapFromKeyValues_t>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_TYPE_TO_TYPE_MAP_H_
