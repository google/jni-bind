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

#ifndef JNI_BIND_METAPROGRAMMING_N_BIT_SEQUENCE_H
#define JNI_BIND_METAPROGRAMMING_N_BIT_SEQUENCE_H

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "n_bit.h"
#include "type_index_mask.h"
#include "type_of_nth_element.h"

namespace jni::metaprogramming {

template <typename...>
struct NBitSequence {};

template <>
struct NBitSequence<> {
  using Increment = NBitSequence<>;
};

// Represents a sequence of "NBits".  This can be useful for generating
// selections of pools of objects of various sizes.
//
// For a sequence with N values, a tuple of size N that contains tuples whose
// sizes match those of max_values can be selected from using TypeMask.
//
// In other words, a sequence of N sets of sizes K0, K1, K2...KN can represent a
// single selection with an NBitSequence where sizeof...(values) == N and
// sizeof...(max_values) == N where Ki == max_values[i].
template <size_t... values, size_t... max_values, bool... overflows>
struct NBitSequence<NBit<values, max_values, overflows>...> {
  static constexpr size_t sequence_size_ = sizeof...(values);

  using Sequence = std::tuple<NBit<values, max_values, overflows>...>;
  using TypeMask = TypeIndexMask<values...>;

  template <size_t I>
  using GetBit = TypeOfNthTupleElement_t<I, Sequence>;

  // Helper method to compare values of all NBits.
  template <size_t... Bits>
  static constexpr bool Same(std::index_sequence<Bits...> = {}) {
    static_assert(sizeof...(Bits) == sizeof...(values));
    return std::is_same_v<std::index_sequence<values...>,
                          std::index_sequence<Bits...>>;
  }

  static constexpr size_t max_representable_size_ = ((max_values + 1) * ...);

  template <size_t... Bits>
  static constexpr bool same_ = Same(std::index_sequence<Bits...>{});

  // If the next bit is overflowing (i.e. it just rolled over), this current
  // value increments. Bits only overflow for a single cycle, otherwise higher
  // order bits flip when no rollover is happening.
  template <size_t I>
  struct IncrementedBit {
    using type = std::conditional_t<IncrementedBit<I + 1>::type::overflow_bit_,
                                    typename GetBit<I>::Increment,
                                    typename GetBit<I>::ResetOverflow>;
  };

  template <>
  struct IncrementedBit<sequence_size_ - 1> {
    using type = typename GetBit<sequence_size_ - 1>::Increment;
  };

  template <typename index_sequence>
  struct IncrementSequenceHelper {};

  template <size_t... Is>
  struct IncrementSequenceHelper<std::index_sequence<Is...>> {
    using type = NBitSequence<typename IncrementedBit<Is>::type...>;
  };

  using Increment = typename IncrementSequenceHelper<
      std::make_index_sequence<sequence_size_>>::type;
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_N_BIT_SEQUENCE_H
