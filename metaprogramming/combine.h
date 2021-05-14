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

#ifndef JNI_BIND_METAPROGRAMMING_COMBINE_H_
#define JNI_BIND_METAPROGRAMMING_COMBINE_H_

#include <tuple>

namespace jni::metaprogramming {

// Takes two tuples and collapses them into a single tuple.
struct Combine {
  template <typename T1, typename T2>
  struct CombineHelper {
    static_assert(std::is_same_v<T1, void> && std::is_same_v<T2, void>,
                  "Combine requires tuples.");
  };

  template <typename... T1s, typename... T2s>
  struct CombineHelper<std::tuple<T1s...>, std::tuple<T2s...>> {
    using type = std::tuple<T1s..., T2s...>;
  };

  template <typename Tup1, typename Tup2>
  using type = typename CombineHelper<Tup1, Tup2>::type;
};

template <typename Tup1, typename Tup2>
using Combine_t = typename Combine::template type<Tup1, Tup2>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_COMBINE_H_
