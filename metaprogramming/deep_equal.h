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
#ifndef JNI_BIND_METAPROGRAMMING_DEEP_EQUAL_H_
#define JNI_BIND_METAPROGRAMMING_DEEP_EQUAL_H_

#include <type_traits>

#include "metaprogramming/pack_discriminator.h"
#include "metaprogramming/vals_equal.h"

namespace jni::metaprogramming {

template <typename T1>
struct DeepEqual;

template <typename T1, typename T2>
static constexpr bool DeepEqual_v = DeepEqual<T1>::template val<T2>;

template <typename Container1>
struct TypesEqual;

template <typename Container1_>
struct TypesEqual {
  template <typename Container1, typename Container2>
  struct Helper;

  template <template <typename...> class Container1,
            template <typename...> class Container2, typename... T1s,
            typename... T2s>
  struct Helper<Container1<T1s...>, Container2<T2s...>> {
    static constexpr bool Val() {
      if constexpr (sizeof...(T1s) == sizeof...(T2s)) {
        // Containers are interchangeable (i.e. the same).
        if constexpr (std::is_same_v<Container1<T1s...>, Container2<T1s...>>) {
          return (DeepEqual_v<T1s, T2s> && ...);
        } else {
          return false;
        }
      } else {
        return false;
      }
    }

    static constexpr bool val = Val();
  };

  template <typename Container2>
  static constexpr bool val = Helper<Container1_, Container2>::val;
};

template <typename T1>
struct DeepEqual {
  template <PackType pack_type_1, PackType pack_type_2>
  static constexpr bool Compatible() {
    if constexpr (pack_type_1 == PackType::NOT_CONTAINER ||
                  pack_type_2 == PackType::NOT_CONTAINER) {
      // Non-containers only with non-containers.
      return pack_type_1 == PackType::NOT_CONTAINER &&
             pack_type_2 == PackType::NOT_CONTAINER;
    } else if constexpr (pack_type_1 == PackType::TYPES ||
                         pack_type_2 == PackType::TYPES) {
      // Type packs only with other type packs.
      return pack_type_1 == PackType::TYPES && pack_type_2 == PackType::TYPES;
    } else {
      // All the auto pack types.
      return true;
    }
  }

  template <typename T2>
  static constexpr bool Val() {
    const PackType pack_type_1 = PackDiscriminator_e<T1>;
    const PackType pack_type_2 = PackDiscriminator_e<T2>;

    if constexpr (!Compatible<pack_type_1, pack_type_2>()) {
      return false;
    } else if constexpr (PackDiscriminator_e<T1> == PackType::NOT_CONTAINER) {
      return std::is_same_v<T1, T2>;
    } else if constexpr (PackDiscriminator_e<T1> == PackType::TYPES) {
      return TypesEqual<T1>::template val<T2>;
    } else {
      return ValsEqual_v<T1, T2>;
    }

    return true;
  }

  template <typename T2>
  static constexpr bool val = Val<T2>();
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_DEEP_EQUAL_H_
