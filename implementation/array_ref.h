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

#ifndef JNI_BIND_ARRAY_REF_H_
#define JNI_BIND_ARRAY_REF_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <atomic>
#include <cstddef>
#include <type_traits>

#include "implementation/array_view.h"
#include "implementation/class_ref.h"
#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/jni_array_helper.h"
#include "implementation/jni_helper/lifecycle.h"
#include "implementation/local_object.h"
#include "implementation/no_idx.h"
#include "implementation/promotion_mechanics_tags.h"
#include "jni_dep.h"

namespace jni {

// Note: All arrays are local (global arrays of local objects is too confusing).
template <typename JniT>
using ScopedArrayImpl =
    Scoped<LifecycleType::LOCAL, JniT, jarray, typename JniT::StorageType>;

// |SpanType| is primitive types like jint, jfloat, etc.
template <typename JniT, typename Enable = void>
class ArrayRef : public ScopedArrayImpl<JniT> {
 public:
  using Base = ScopedArrayImpl<JniT>;

  using Base::Base;
  using SpanType = typename JniT::SpanType;

  ArrayRef(std::size_t size)
      : Base(AdoptLocal{},
             JniArrayHelper<SpanType, JniT::kRank>::NewArray(size)) {}

  explicit ArrayRef(int size) : ArrayRef(static_cast<std::size_t>(size)) {}

  // Pin the array to get direct access to its elements
  //
  // copy_on_completion: Whether to copy changes back to the Java array when the
  //                     ArrayView is destroyed (default: true)
  // access_mode: The access mode to use (REGULAR for normal JNI access,
  //              CRITICAL for GetPrimitiveArrayCritical)
  //
  // Returns: An ArrayView that provides access to the array elements
  // 
  // When access_mode is REGULAR, this uses the standard JNI Get*ArrayElements functions
  // When access_mode is CRITICAL, this uses the JNI GetPrimitiveArrayCritical function
  // which may provide a direct pointer to the array in the Java heap without copying
  ArrayView<SpanType, JniT::kRank> Pin(bool copy_on_completion = true,
                                    ArrayAccessMode access_mode = ArrayAccessMode::REGULAR) {
    return {Base::object_ref_, copy_on_completion, Length(), access_mode};
  }
  
  // Convenience method to access array using the critical API
  //
  // This uses GetPrimitiveArrayCritical which may provide direct access to the 
  // array memory without copying. During critical access:
  // - No other JNI calls should be made
  // - The critical section should be as short as possible
  // - The JVM's garbage collector may be blocked
  //
  // copy_on_completion: Whether to copy changes back to the Java array when the
  //                     ArrayView is destroyed (default: true)
  //
  // Returns: An ArrayView that provides access to the array elements
  ArrayView<SpanType, JniT::kRank> PinCritical(bool copy_on_completion = true) {
    return Pin(copy_on_completion, ArrayAccessMode::CRITICAL);
  }

  std::size_t Length() {
    if (length_.load() == kNoIdx) {
      length_.store(
          JniArrayHelper<SpanType, JniT::kRank>::GetLength(Base::object_ref_));
    }

    return length_.load();
  }

 private:
  std::atomic<std::size_t> length_ = kNoIdx;
};

// Shared behaviour for object like arrays.
template <typename JniT>
class ArrayRefBase : public ScopedArrayImpl<JniT> {
 public:
  using Base = ScopedArrayImpl<JniT>;
  using Base::Base;
  using SpanType = typename JniT::SpanType;

  // Construct array with given size and null values.
  explicit ArrayRefBase(std::size_t size)
      : Base(AdoptLocal{},
             JniArrayHelper<jobject, JniT::kRank>::NewArray(
                 size, ClassRef_t<JniT>::GetAndMaybeLoadClassRef(nullptr),
                 static_cast<jobject>(nullptr))) {}

  // Construct from jobject lvalue (object is used as template).
  explicit ArrayRefBase(std::size_t size, jobject obj)
      : Base(AdoptLocal{}, JniArrayHelper<jobject, JniT::kRank>::NewArray(
                               size,
                               ClassRef_t<JniT>::GetAndMaybeLoadClassRef(
                                   static_cast<jobject>(obj)),
                               static_cast<jobject>(obj))) {}

  // Object arrays cannot be efficiently pinned like primitive types can.
  // The access_mode parameter is ignored for object arrays as they cannot use
  // the GetPrimitiveArrayCritical API.
  //
  // copy_on_completion: Whether to copy changes back to the Java array when the
  //                     ArrayView is destroyed (default: false)
  // access_mode: Ignored for object arrays
  //
  // Returns: An ArrayView that provides access to the array elements
  ArrayView<SpanType, JniT::kRank> Pin(bool copy_on_completion = false,
                                     ArrayAccessMode access_mode = ArrayAccessMode::REGULAR) {
    return {Base::object_ref_, copy_on_completion, Length()};
  }
  
  // For API consistency only - objects cannot use critical access
  //
  // Note: This method exists for API consistency, but object arrays
  // cannot use GetPrimitiveArrayCritical. This call is equivalent to
  // calling Pin() with REGULAR access mode.
  //
  // copy_on_completion: Whether to copy changes back to the Java array when the
  //                     ArrayView is destroyed (default: false)
  //
  // Returns: An ArrayView that provides access to the array elements
  ArrayView<SpanType, JniT::kRank> PinCritical(bool copy_on_completion = false) {
    return Pin(copy_on_completion, ArrayAccessMode::REGULAR);
  }

  std::size_t Length() {
    return JniArrayHelper<jobject, JniT::kRank>::GetLength(Base::object_ref_);
  }

  // Note: Globals are not permitted in a local array because it makes reasoning
  // about them confusing.
  //
  // TODO(b/406948932): Permit lvalues of locals and globals as technically
  // they're both viable (the scope will be extended as expected).
  void Set(
      std::size_t idx,
      LocalObject<JniT::class_v, JniT::class_loader_v, JniT::jvm_v>&& val) {
    JniArrayHelper<jobject, JniT::kRank>::SetArrayElement(Base::object_ref_,
                                                          idx, val.Release());
  }
};

// |SpanType| is object and rank is 1.
template <typename JniT>
class ArrayRef<
    JniT, std::enable_if_t<(std::is_same_v<typename JniT::SpanType, jobject> &&
                            JniT::kRank == 1)>> : public ArrayRefBase<JniT> {
 public:
  using Base = ArrayRefBase<JniT>;
  using Base::Base;
  using SpanType = typename JniT::SpanType;

  // Construct from LocalObject lvalue (object is used as template).
  //
  // e.g.
  //  LocalArray arr { 5, LocalObject<kClass> {args...} };
  //  LocalArray arr { 5, GlobalObject<kClass> {args...} };
  template <
      template <const auto&, const auto&, const auto&> class ObjectContainer,
      const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  ArrayRef(std::size_t size,
           const ObjectContainer<class_v, class_loader_v, jvm_v>& obj)
      : ArrayRef(size, static_cast<jobject>(obj)) {}

  LocalObject<JniT::class_v, JniT::class_loader_v, JniT::jvm_v> Get(
      std::size_t idx) {
    return {JniArrayHelper<jobject, JniT::kRank>::GetArrayElement(
        Base::object_ref_, idx)};
  }
};

// |SpanType| is object or rank is > 1.
template <typename JniT>
class ArrayRef<JniT, std::enable_if_t<(JniT::kRank > 1)>>
    : public ArrayRefBase<JniT> {
 public:
  using Base = ArrayRefBase<JniT>;
  using Base::Base;

  static constexpr std::decay_t<decltype(JniT::GetClass())> clazz =
      JniT::GetClass();
  static constexpr std::decay_t<decltype(JniT::GetClassLoader())> class_loader =
      JniT::GetClassLoader();
  static constexpr std::decay_t<decltype(JniT::GetJvm())> jvm = JniT::GetJvm();

  LocalArray<typename JniT::SpanType, JniT::kRank - 1, clazz, class_loader, jvm>
  Get(std::size_t idx) {
    return {AdoptLocal{},
            static_cast<jarray>(
                JniArrayHelper<typename JniT::SpanType,
                               JniT::kRank>::GetArrayElement(Base::object_ref_,
                                                             idx))};
  }

  template <typename SpanType, std::size_t kRank_, const auto& class_v_,
            const auto& class_loader_v_, const auto& jvm_v_>
  void Set(std::size_t idx, const LocalArray<SpanType, kRank_, class_v_,
                                             class_loader_v_, jvm_v_>& val) {
    using ElementT =
        typename JniArrayHelper<SpanType, JniT::kRank - 1>::AsArrayType;
    JniArrayHelper<ElementT, JniT::kRank>::SetArrayElement(
        Base::object_ref_, idx, static_cast<ElementT>(val));
  }
};

}  // namespace jni

#endif  // JNI_BIND_ARRAY_REF_H_
