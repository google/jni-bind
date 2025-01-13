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

#ifndef JNI_BIND_METAPROGRAMMING_CARTESIAN_PRODUCT_H_
#define JNI_BIND_METAPROGRAMMING_CARTESIAN_PRODUCT_H_

#include <cstddef>
#include <tuple>
#include <utility>

#include "increment.h"
#include "n_bit.h"
#include "n_bit_sequence.h"
#include "type_of_nth_element.h"

namespace jni::metaprogramming {

// Metafunction that takes a list of tuples, and creates a counter where each
// value is the size of the corresponding tuple.  This can be used to create
// every possible permutation of the input tuples.
struct GenerateBitSequenceFromTupSets {
  template <typename... Tups>
  struct Helper {
    using ListOfTups = std::tuple<Tups...>;

    template <typename>
    struct SequenceGenerator {};

    template <size_t... Is>
    struct SequenceGenerator<std::index_sequence<Is...>> {
      // Generates count to max size inclusive, so -1 on the size.
      // e.g. std::tuple<int> has only 1 viable selection but would be
      // represented by a unary NBit (i.e NBit<0, 0, false>);
      using type =
          NBitSequence<NBit<0,
                            std::tuple_size_v<TypeOfNthTupleElement_t<
                                Is, typename Helper<Tups...>::ListOfTups>> -
                                1,
                            false>...>;
    };
  };

  // Due to a gcc bug, this requires using a name that is *not* type.
  template <typename... Tups>
  using generated_type = typename Helper<Tups...>::template SequenceGenerator<
      std::make_index_sequence<sizeof...(Tups)>>::type;
};

struct GenerateBitSequenceFromTupSetsFunc {
  template <typename... Tups>
  using generated_type =
      typename GenerateBitSequenceFromTupSets::template generated_type<Tups...>;
};

template <typename... Tups>
using GenerateBitSequenceFromTupSets_t =
    typename GenerateBitSequenceFromTupSets::template generated_type<Tups...>;

template <typename... Tups>
struct CartesianProductHelper {
  using NBitSequenceForTups = GenerateBitSequenceFromTupSets_t<Tups...>;
  using AllTupsAsList = std::tuple<Tups...>;

  template <typename>
  struct SequenceGenerator {};

  template <size_t... Is>
  struct SequenceGenerator<std::index_sequence<Is...>> {
    using type = std::tuple<typename Increment_t<
        NBitSequenceForTups, Is>::TypeMask::template type<AllTupsAsList>...>;
  };

  using type = typename SequenceGenerator<std::make_index_sequence<
      NBitSequenceForTups::max_representable_size_>>::type;
};

template <>
struct CartesianProductHelper<> {
  using type = std::tuple<>;
};

struct CartesianProduct {
  template <typename... Tups>
  using type = typename CartesianProductHelper<Tups...>::type;
};

template <typename... Tups>
using CartesianProduct_t = typename CartesianProduct::template type<Tups...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_CARTESIAN_PRODUCT_H_
