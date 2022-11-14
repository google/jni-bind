/*
 * Copyright 2022 Google LLC
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
#ifndef JNI_BIND_METAPROGRAMMING_REDUCE_VALS_H_
#define JNI_BIND_METAPROGRAMMING_REDUCE_VALS_H_

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// See Reduce.
template <class Operation>
struct ReduceVals {
  template <auto... vs>
  struct SequenceGenerator;

  template <const auto v1, const auto v2>
  static constexpr auto Operation_v = Operation::template val<v1, v2>;

  template <auto... vs>
  static constexpr auto SequenceGenerator_t = SequenceGenerator<vs...>::value;

  // Used for single element sequence.
  template <auto v1, auto... vs>
  struct SequenceGenerator<v1, vs...> {
    static constexpr auto value = v1;
  };

  // Terminal element in sequence.
  template <auto v1, auto v2>
  struct SequenceGenerator<v1, v2> {
    static constexpr auto value = Operation_v<v1, v2>;
  };

  template <auto v1, auto v2, auto... vs>
  struct SequenceGenerator<v1, v2, vs...> {
    static constexpr auto value =
        SequenceGenerator_t<Operation_v<v1, v2>, vs...>;
  };

  template <auto... Args>
  static constexpr auto value = SequenceGenerator<Args...>::value;
};

template <typename Operation, const auto... vs>
static constexpr auto ReduceVals_v =
    ReduceVals<Operation>::template value<vs...>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_REDUCE_VALS_H_
