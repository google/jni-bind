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

#ifndef JNI_BIND_METAPROGRAMMING_VALS_EQUAL_H_
#define JNI_BIND_METAPROGRAMMING_VALS_EQUAL_H_

#include <cstddef>
#include <utility>

#include "metaprogramming/vals.h"

namespace jni::metaprogramming {

// Performs a deep equality comparison of T, but is leniant about containers.
// Returns true iff types are equal or both types are containers and their
// non-type template parameters are equal.
template <typename T>
struct ValsEqual {
  template <typename T1, typename T2, typename Enable = void>
  struct Helper {
    static constexpr bool val = std::is_same_v<T1, T2>;
  };

  // Value comparison.
  template <template <const auto...> class Container1, const auto... V1s_,
            template <const auto...> class Container2, const auto... V2s_>
  struct Helper<Container1<V1s_...>, Container2<V2s_...>,
                std::enable_if_t<((V1s_ == V2s_) && ...)>> {
    static constexpr bool val = true;
  };

  // LHS l-reference, RHS value.
  template <template <const auto&...> class Container1, const auto&... V1s_,
            template <const auto...> class Container2, const auto... V2s_>
  struct Helper<Container1<V1s_...>, Container2<V2s_...>,
                std::enable_if_t<((V1s_ == V2s_) && ...)>> {
    static constexpr bool val = true;
  };

  // LHS value, RHS l-reference.
  template <template <const auto...> class Container1, const auto... V1s_,
            template <const auto&...> class Container2, const auto&... V2s_>
  struct Helper<Container1<V1s_...>, Container2<V2s_...>,
                std::enable_if_t<((V1s_ == V2s_) && ...)>> {
    static constexpr bool val = true;
  };

  // RHS l-reference, RHS l-reference.
  template <template <const auto&...> class Container1, const auto&... V1s_,
            template <const auto&...> class Container2, const auto&... V2s_>
  struct Helper<Container1<V1s_...>, Container2<V2s_...>,
                std::enable_if_t<((V1s_ == V2s_) && ...)>> {
    static constexpr bool val = true;
  };

  template <typename T2>
  static constexpr bool val = Helper<T, T2>::val;
};

template <typename T1, typename T2>
static constexpr bool ValsEqual_v = ValsEqual<T1>::template val<T2>;

template <auto V1, auto V2>
static constexpr bool ValsEqual_v_v =
    ValsEqual<Vals<V1>>::template val<Vals<V2>>;

template <auto& V1, auto& V2>
static constexpr bool ValsEqual_r_v =
    ValsEqual<ValsRef<V1>>::template val<ValsRef<V2>>;

template <const auto& V1, const auto& V2>
static constexpr bool ValsEqual_cr_v =
    ValsEqual<ValsConstRef<V1>>::template val<ValsConstRef<V2>>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_VALS_EQUAL_H_
