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

#ifndef JNI_BIND_METAPROGRAMMING_FIND_IDX_OF_VAL_H_
#define JNI_BIND_METAPROGRAMMING_FIND_IDX_OF_VAL_H_

#include <tuple>
#include <type_traits>

#include "all_unique.h"
#include "same.h"
#include "tuple_manipulation.h"

namespace jni::metaprogramming {

// Metafunction that binds a metafunction (e.g. Same), a query (e.g. "A"),  a
// variadic pack as arguments, and returns the idx of the query.  All members
// of the variadic pack must be unique.
//
// |Comparator| must express a static bool data member "value".
template <typename Comparator>
struct FindIdxOfVal {
  template <typename... Ts>
  struct FindIdxOfValHelper {
    static_assert(sizeof...(Ts) == -1, "The sought element is not in the set");
  };

  template <typename T, typename... Ts>
  struct FindIdxOfValHelper<T, Ts...> {
    template <std::size_t idx>
    static constexpr std::size_t GetIdxHelper() {
      if constexpr (Comparator::template value<T>) {
        return idx;
      } else {
        return FindIdxOfValHelper<Ts...>::template GetIdxHelper<idx - 1>();
      }
    }

    static constexpr std::size_t GetIdx() {
      return sizeof...(Ts) - GetIdxHelper<sizeof...(Ts)>();
    }
  };

  // Done once in a seperate struct for improved compilation time.
  template <typename... Ts>
  struct StaticAssertWrapper {
    static_assert(AllUnique_v<Ts...>,
                  "FindIdxOfVal only operates on unique sets.");
    static constexpr std::size_t value = FindIdxOfValHelper<Ts...>::GetIdx();
  };

  template <typename... Ts>
  static constexpr std::size_t value = StaticAssertWrapper<Ts...>::value;
};

template <typename Comparator, typename... Ts>
static constexpr std::size_t FindIdxOfValWithComparator_idx =
    FindIdxOfVal<Comparator>::template value<Ts...>;

// Convenience alias with default of std::is_same Comparator (typical use case).
template <typename Query, typename... Ts>
static constexpr std::size_t FindIdxOfVal_idx =
    FindIdxOfValWithComparator_idx<Same<Query>, Ts...>;

template <typename Comparator, typename TupType>
static constexpr std::size_t FindIdxOfValInTupWithComparator_idx =
    TupleUnroller_v<FindIdxOfVal<Comparator>, TupType>;

// Convenience alias with default of std::is_same Comparator (typical use case).
template <typename Query, typename TupType>
static constexpr std::size_t FindIdxOfValInTup_idx =
    FindIdxOfValInTupWithComparator_idx<Same<Query>, TupType>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_FIND_IDX_OF_VAL_H_
