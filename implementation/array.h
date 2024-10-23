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

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <type_traits>

#include "implementation/array_type_conversion.h"
#include "implementation/object.h"
#include "jni_dep.h"

namespace jni {

template <typename RawType, std::size_t kRank = 1>
struct Array;

template <std::size_t kRank>
struct Rank {};

struct ArrayBase {};

////////////////////////////////////////////////////////////////////////////////
// Array Non-Object Implementation.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType, std::size_t kRank>
struct ArrayNonObjectTypeImpl : ArrayBase {
  RawType raw_;

  constexpr ArrayNonObjectTypeImpl(RawType raw) : raw_(raw) {}

  constexpr ArrayNonObjectTypeImpl(RawType raw, Rank<kRank>) : raw_(raw) {}

  template <std::size_t kRank_, typename RawType_>
  constexpr ArrayNonObjectTypeImpl(Array<RawType, kRank>&& invalid_arg)
      : raw_(nullptr) {
    static_assert(std::is_same_v<RawType, void>,
                  "JNI Error: Invalid array declaration, use Array { type{}, "
                  "Rank<kRank>{} }.");
  }
};

template <typename T>
struct ArrayComparisonHelper {};

template <typename RawType, std::size_t kRank>
struct ArrayComparisonHelper<Array<ArrayNonObjectTypeImpl<RawType, kRank>>> {
  using type = RawType;
};

template <typename RawType, std::size_t kRank>
struct ArrayComparisonHelper<Array<RawType, kRank>> {
  using type = RawType;
};

template <typename T>
using ArrayComparisonHelper_t = typename ArrayComparisonHelper<T>::type;

template <typename T1, typename T2>
static constexpr bool IsArrayComparable() {
  return std::is_base_of_v<ArrayBase, T1> && std::is_base_of_v<ArrayBase, T2>;
};

////////////////////////////////////////////////////////////////////////////////
// Equality operators.
////////////////////////////////////////////////////////////////////////////////
template <typename T1, typename T2>
constexpr std::enable_if_t<IsArrayComparable<T1, T2>(), bool> operator==(
    const T1& lhs, const T2& rhs) {
  if constexpr (std::is_same_v<ArrayComparisonHelper_t<T1>,
                               ArrayComparisonHelper_t<T2>>) {
    return (lhs.raw_ == rhs.raw_);
  }
  return false;
}

template <typename T1, typename T2>
constexpr std::enable_if_t<IsArrayComparable<T1, T2>(), bool> operator!=(
    const T1& lhs, const T2& rhs) {
  return !(lhs == rhs);
}

// Primitive array implementaiton.
template <typename T, std::size_t kRank, bool HoldsObject>
struct ArrayImpl : public ArrayNonObjectTypeImpl<T, kRank>,
                   ArrayTag<RegularToArrayTypeMap_t<T>> {
 public:
  using ArrayNonObjectTypeImpl<T, kRank>::ArrayNonObjectTypeImpl;
};

////////////////////////////////////////////////////////////////////////////////
// Array Object Implementation.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType, std::size_t kRank_>
struct ArrayImpl<RawType, kRank_, true> : public ArrayTag<jobjectArray>,
                                          ArrayBase {
  RawType raw_;

  constexpr ArrayImpl(RawType raw) : raw_(raw) {}

  template <std::size_t kRank>
  constexpr ArrayImpl(RawType raw, Rank<kRank>) : raw_(raw) {}
};

template <typename RawTypeLhs, std::size_t kRankLhs, typename RawTypeRhs,
          std::size_t kRankRhs>
constexpr bool operator==(const ArrayImpl<RawTypeLhs, kRankLhs, true>& lhs,
                          const ArrayImpl<RawTypeRhs, kRankRhs, true>& rhs) {
  if constexpr (std::is_same_v<RawTypeLhs, RawTypeRhs>) {
    return (lhs.raw_ == rhs.raw_);
  }
  return false;
}

template <typename RawTypeLhs, std::size_t kRankLhs, typename RawTypeRhs,
          std::size_t kRankRhs>
constexpr bool operator!=(const ArrayImpl<RawTypeLhs, kRankLhs, true>& lhs,
                          const Array<RawTypeRhs, kRankRhs>& rhs) {
  return !(lhs == rhs);
}

// This type correlates to those used in declarations,
//   e.g. Field { Array { Array { jint {} } } }.
template <typename RawType, std::size_t kRank_>
struct Array
    : public ArrayImpl<RawType, kRank_, std::is_base_of_v<Object, RawType>> {
  constexpr Array()
      : ArrayImpl<RawType, kRank_, std::is_base_of_v<Object, RawType>>(
            RawType{}) {}

  constexpr Array(RawType raw)
      : ArrayImpl<RawType, kRank_, std::is_base_of_v<Object, RawType>>(raw) {}

  template <std::size_t kRank>
  constexpr Array(RawType raw, Rank<kRank>)
      : ArrayImpl<RawType, kRank, std::is_base_of_v<Object, RawType>>(raw) {}
};

template <typename RawType>
Array(RawType) -> Array<RawType, 1>;

template <typename RawType, std::size_t kRank>
Array(RawType, Rank<kRank>) -> Array<RawType, kRank>;

////////////////////////////////////////////////////////////////////////////////
// Rank Utilities.
////////////////////////////////////////////////////////////////////////////////
struct Rankifier {
  template <typename SpanType_>
  struct Helper;

  template <typename SpanType_, std::size_t kRank_>
  struct Helper<Array<SpanType_, kRank_>> {
    static constexpr std::size_t kRank = kRank_;
  };

  template <typename ArrayT>
  static inline constexpr std::size_t Rank(const ArrayT& maybe_array) {
    if constexpr (kIsArrayType<std::decay_t<decltype(maybe_array)>>) {
      return Helper<ArrayT>::kRank;
    } else {
      return 0;
    }
  }
};

////////////////////////////////////////////////////////////////////////////////
// Strip Utilities.
// Takes an native array like type, and emits the innermost type.
// e.g. {Array<int>, Array{Rank<2>{}, int{}, Array{Rank<3>{}, int{}} } => int.
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct ArrayStrip {
  using type = T;
};

template <typename T>
using ArrayStrip_t = typename ArrayStrip<T>::type;

template <typename T, std::size_t kRank>
struct ArrayStrip<Array<T, kRank>> {
  using type = T;
};

template <typename T>
constexpr auto FullArrayStripV(const T& val) {
  if constexpr (kIsArrayType<std::decay_t<decltype(val)>>) {
    return FullArrayStripV(val.raw_);
  } else {
    return val;
  }
}

}  // namespace jni

#endif  // JNI_BIND_ARRAY_H_
