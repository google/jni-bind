/*
 * Copyright 2023 Google LLC
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

#ifndef JNI_BIND_METAPROGRAMMING_LAMBDA_COMPATIBLE_H_
#define JNI_BIND_METAPROGRAMMING_LAMBDA_COMPATIBLE_H_

#include <tuple>
#include <utility>

#include "deep_equal.h"
#include "tuple_manipulation.h"
#include "type_of_nth_element.h"

namespace jni::metaprogramming {

// Allows lambda argument comparison with deep equality.
// Iff arg lists are equal, and each element is pointwise equal, `val` is true.
template <const auto& lambda>
struct LambdaCompatible {
  template <const auto& query, typename IndexSequence>
  struct Helper;

  using LambdaT = std::decay_t<decltype(std::declval<decltype(lambda())>())>;
  static constexpr std::size_t kNumArgs = std::tuple_size_v<LambdaT>;

  template <const auto& query, size_t... Is>
  struct Helper<query, std::index_sequence<Is...>> {
    using QueryT = std::decay_t<decltype(query())>;
    static constexpr std::size_t kNumQueryArgs = std::tuple_size_v<QueryT>;

    // Sizes aren't equal.
    template <const auto&, bool>
    struct InnerHelper {
      static constexpr bool val = false;
    };

    // Sizes are equal, deep comparison (templated to defer compilatino).
    template <const auto& query_inner>
    struct InnerHelper<query_inner, true> {
      static constexpr bool val =
          (DeepEqual_v<TypeOfNthTupleElement_t<Is, LambdaT>,
                       TypeOfNthTupleElement_t<Is, QueryT>> &&
           ...);
    };

    static constexpr bool val =
        InnerHelper<query, kNumArgs == kNumQueryArgs>::val;
  };

  template <const auto& query>
  static constexpr bool val =
      Helper<query, std::make_index_sequence<kNumArgs>>::val;
};

template <const auto& lambda, const auto& query>
static constexpr bool LambdaCompatible_v =
    LambdaCompatible<lambda>::template val<query>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_LAMBDA_COMPATIBLE_H_
