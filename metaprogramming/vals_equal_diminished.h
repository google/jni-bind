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

#ifndef JNI_BIND_METAPROGRAMMING_VALS_EQUAL_DIMINISHED_H_
#define JNI_BIND_METAPROGRAMMING_VALS_EQUAL_DIMINISHED_H_

#include "metaprogramming/vals.h"

namespace jni::metaprogramming {

// This class is a diminished form of `ValsEqual` that treats everything as an
//  auto const&. Unfortunately, MSVC appears to have a bug that makes compiling
// `ValsEqual` impossible, however, JNI Bind only ever needs auto const&.
template <typename T>
struct ValsEqualDiminished {
  template <typename T1, typename T2>
  struct Helper {
    static constexpr bool val = std::is_same_v<T1, T2>;
  };

  template <template <const auto&...> class Container1, const auto&... V1s_,
            template <const auto&...> class Container2, const auto&... V2s_>
  struct Helper<Container1<V1s_...>, Container2<V2s_...>> {
    static constexpr bool val = ((V1s_ == V2s_) && ...);
  };

  template <typename T2>
  static constexpr bool val = Helper<T, T2>::val;
};

template <typename T1, typename T2>
static constexpr bool ValsEqualDiminished_v =
    ValsEqualDiminished<T1>::template val<T2>;

template <auto V1, auto V2>
static constexpr bool ValsEqualDiminished_v_v =
    ValsEqualDiminished<Vals<V1>>::template val<Vals<V2>>;

template <auto& V1, auto& V2>
static constexpr bool ValsEqualDiminished_r_v =
    ValsEqualDiminished<ValsRef<V1>>::template val<ValsRef<V2>>;

template <const auto& V1, const auto& V2>
static constexpr bool ValsEqualDiminished_cr_v =
    ValsEqualDiminished<ValsConstRef<V1>>::template val<ValsConstRef<V2>>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_VALS_EQUAL_DIMINISHED_H_
