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

#include "implementation/array_type_conversion.h"
#include "implementation/object.h"
#include "implementation/return.h"
#include "jni_dep.h"
#include "metaprogramming/contains.h"
#include "metaprogramming/type_to_type_map.h"

namespace jni {

template <typename RawType>
struct Array;

////////////////////////////////////////////////////////////////////////////////
// Array Non-Object Implementation.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType>
struct ArrayNonObjectTypeImpl {
  RawType raw_type_;

  constexpr ArrayNonObjectTypeImpl(RawType raw_type) : raw_type_(raw_type) {}

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

// Primitive array implementaiton.
template <typename T, bool HoldsObject>
struct ArrayImpl : public ArrayNonObjectTypeImpl<T>,
                   ArrayTag<RegularToArrayTypeMap_t<T>> {
 public:
  using ArrayNonObjectTypeImpl<T>::ArrayNonObjectTypeImpl;
};

// Arrays of arrays are nominally special in that they index as jarrays but they
// will CDecl as their most base type.
template <typename T>
struct ArrayImpl<Array<T>, false> : public ArrayNonObjectTypeImpl<Array<T>>,
                                    ArrayTag<jarray> {
 public:
  using ArrayNonObjectTypeImpl<Array<T>>::ArrayNonObjectTypeImpl;
};

////////////////////////////////////////////////////////////////////////////////
// Array Object Implementation.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType>
struct ArrayImpl<RawType, true> : public ArrayTag<jobjectArray> {
  RawType raw_type_;

  constexpr ArrayImpl(RawType raw_type) : raw_type_(raw_type) {}

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

// This type correlates to those used in declarations,
//   e.g. Field { Array { Array { jint {} } } }.
template <typename RawType>
struct Array : public ArrayImpl<RawType, std::is_base_of_v<Object, RawType>> {
  constexpr Array(RawType raw_type)
      : ArrayImpl<RawType, std::is_base_of_v<Object, RawType>>(raw_type) {}
};

template <typename RawType>
Array(RawType) -> Array<RawType>;

template <typename RawType>
Array(Array<RawType>) -> Array<Array<RawType>>;

////////////////////////////////////////////////////////////////////////////////
// Rank Utilities.
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Rankifier {
  static constexpr bool kComputableRank = true;

  template <typename ArrayT>
  static inline constexpr std::size_t Rank(const ArrayT& maybe_array) {
    if constexpr (kIsArrayType<std::decay_t<decltype(maybe_array)>>) {
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

////////////////////////////////////////////////////////////////////////////////
// Strip Utilities.
// Takes an native array like type, and emits the innermost type.
// e.g. {Array<int>, Array{Array<int>}, Array{Array{Array<int>}} } => int.
////////////////////////////////////////////////////////////////////////////////
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

template <typename T>
constexpr auto FullArrayStripV(const T& val) {
  if constexpr (kIsArrayType<std::decay_t<decltype(val)>>) {
    return FullArrayStripV(val.raw_type_);
  } else {
    return val;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Array Build from Span + Rank.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType, std::size_t kRank>
struct ArrayFromRank {
  // Deferred
  template <std::size_t kRecursedRank>
  struct Helper {
    using type = Array<typename ArrayFromRank<RawType, kRank - 1>::type>;
  };

  template <>
  struct Helper<0> {
    using type = RawType;
  };

  using type = typename Helper<kRank>::type;
};

template <typename RawType, std::size_t kRank>
using ArrayFromRank_t = typename ArrayFromRank<RawType, kRank>::type;

}  // namespace jni

#endif  // JNI_BIND_ARRAY_H_
