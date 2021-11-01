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

#ifndef JNI_BIND_ARRAY_H_
#define JNI_BIND_ARRAY_H_

#include <type_traits>

#include "jni_dep.h"
#include "return.h"

namespace jni {

struct ArrayTag {};

template <typename RawType>
struct Array : public ArrayTag {
  RawType raw_type_;

  constexpr Array(RawType raw_type) : raw_type_(raw_type) {}

  template <typename RawTypeRhs>
  constexpr bool operator==(const Array<RawTypeRhs>& rhs) const {
    if constexpr (std::is_same_v<RawType, RawTypeRhs>) {
      return (raw_type_ == rhs.raw_type_);
    }
    return false;
  }

  template <typename RawTypeRhs>
  constexpr bool operator!=(const Array<RawTypeRhs>& rhs) const {
    return !(*this == rhs);
  }
};

template <typename RawType>
Array(RawType) -> Array<RawType>;

template <typename RawType>
Array(Array<RawType>) -> Array<Array<RawType>>;

////////////////////////////////////////////////////////////////////////////////
// Rank + Type Strip Utilities.
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Rankifier {
  static constexpr bool kComputableRank = true;

  template <typename ArrayT>
  static inline constexpr std::size_t Rank(const ArrayT& maybe_array) {
    if constexpr (std::is_base_of_v<ArrayTag,
                                    std::decay_t<decltype(maybe_array)>>) {
      return Rank(maybe_array.raw_type_) + 1;
    } else {
      return 0;
    }
  }
};

template <>
struct Rankifier<void> {
  static constexpr bool kComputableRank = false;

  template <typename ArrayT>
  constexpr std::size_t Rank(const ArrayT& maybe_array) {
    return 0;
  }
};

// Takes an native array like type, and emits the innermost type.
// e.g. {Array<int>, Array{Array<int>}, Array{Array{Array<int>}} } => int.
template <typename T>
struct ArrayStrip {
  using type = T;
};

template <typename T>
using ArrayStrip_t = typename ArrayStrip<T>::type;

template <typename T>
struct ArrayStrip<Array<T>> {
  using type = ArrayStrip_t<T>;
};

}  // namespace jni

#endif  // JNI_BIND_ARRAY_H_
