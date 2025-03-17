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

#ifndef JNI_BIND_LOCAL_ARRAY_H_
#define JNI_BIND_LOCAL_ARRAY_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <type_traits>

#include "implementation/array_ref.h"
#include "implementation/class.h"
#include "implementation/class_ref.h"
#include "implementation/default_class_loader.h"
#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/jni_array_helper.h"
#include "implementation/jni_helper/lifecycle.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/no_class_specified.h"
#include "implementation/promotion_mechanics_tags.h"
#include "jni_dep.h"

namespace jni {

// Represents an array object (e.g. int[], float[][], Object[], etc).
// Currently GlobalArrays do not exist, as reasoning about the lifecycles of the
// underlying objects is non-trivial, e.g. a GlobalArray taking a local object
// would result in a possibly unexpected extension of lifetime.
//
// LocalArray supports both regular JNI array access (via Get*ArrayElements) and 
// critical access (via GetPrimitiveArrayCritical) for primitive array types.
//
// Direct arrays:
// When using critical access (PinCritical() method), the implementation may return
// a direct pointer to the array in the JVM's memory, avoiding copying. This is
// determined by the JVM and indicated by the is_copy flag in GetArrayElementsResult.
//
// Indirect arrays:
// When using regular access (Pin() method), or when the JVM cannot provide direct 
// access with critical methods, a copy of the array is returned.
//
// Usage notes:
// - Critical sections should be kept as short as possible
// - No JNI calls should be made during a critical section
// - Critical access may block garbage collection in the JVM
template <typename SpanType_, std::size_t kRank_ = 1,
          const auto& class_v_ = kNoClassSpecified,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalArray
    : public ArrayRef<
          JniT<SpanType_, class_v_, class_loader_v_, jvm_v_, kRank_>> {
 public:
  static constexpr Class kClass{class_v_.name_};
  static constexpr std::size_t kRank = kRank_;
  using SpanType = SpanType_;

  using RawValT = std::conditional_t<std::is_same_v<jobject, SpanType>,
                                     std::decay_t<decltype(kClass)>, SpanType>;

  using _JniT = JniT<SpanType, class_v_, class_loader_v_, jvm_v_, kRank_>;

  using Base =
      ArrayRef<JniT<SpanType, class_v_, class_loader_v_, jvm_v_, kRank_>>;
  using Base::Base;

  using RefTag = std::conditional_t<(kRank_ > 1), jobject, SpanType>;

  // RefTag ctor (supports multi-dimensions, `jobject` if rank > 1).
  LocalArray(std::size_t size, RefTag arr)
      : Base(AdoptLocal{}, JniArrayHelper<jobject, kRank_>::NewArray(
                               size,
                               ClassRef_t<_JniT>::GetAndMaybeLoadClassRef(
                                   static_cast<jobject>(arr)),
                               arr)) {}

  template <typename T>
  LocalArray(ArrayViewHelper<T> array_view_helper)
      : LocalArray(AdoptLocal{}, array_view_helper.val_) {}

  // Rvalue ctor.
  LocalArray(LocalArray<SpanType, kRank_>&& rhs)
      : Base(AdoptLocal{}, rhs.Release()) {}

  // Rvalue ctor.
  template <typename SpanTypeRhs_, std::size_t kRank, const auto& class_v,
            const auto& class_loader_v, const auto& jvm_v>
  LocalArray(
      LocalArray<SpanTypeRhs_, kRank, class_v, class_loader_v, jvm_v>&& rhs)
      : Base(AdoptLocal{}, rhs.Release()) {
    static_assert(std::is_same_v<SpanType, SpanTypeRhs_> && kRank == kRank_ &&
                  class_v == class_v_ && class_loader_v == class_loader_v_);
  }

  // Construct from decorated object lvalue (object is used as template).
  //
  // e.g.
  //  LocalArray arr { 5, LocalObject<kClass> {args...} };
  //  LocalArray arr { 5, GlobalObject<kClass> {args...} };
  template <
      template <const auto&, const auto&, const auto&> class ObjectContainer,
      const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  LocalArray(
      std::size_t size,
      const ObjectContainer<class_v, class_loader_v, jvm_v>& local_object)
      : Base(JniArrayHelper<jobject, kRank_>::NewArray(
            size,
            ClassRef_t<_JniT>::GetAndMaybeLoadClassRef(
                static_cast<jobject>(local_object)),
            static_cast<jobject>(local_object))) {}

  operator jobject() { return static_cast<jobject>(Base::object_ref_); }
};

template <
    template <const auto&, const auto&, const auto&> class ObjectContainer,
    const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalArray(std::size_t,
           const ObjectContainer<class_v_, class_loader_v_, jvm_v_>&)
    -> LocalArray<jobject, class_v_, class_loader_v_, jvm_v_>;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalArray(
    std::size_t,
    const Scoped<LifecycleType::LOCAL,
                 JniT<jobject, class_v_, class_loader_v_, jvm_v_>, jobject>&)
    -> LocalArray<jobject, class_v_, class_loader_v_, jvm_v_>;

template <typename SpanType>
LocalArray(std::size_t, SpanType)
    -> LocalArray<SpanType, 1, kNoClassSpecified, kDefaultClassLoader,
                  kDefaultJvm>;

template <typename SpanType, std::size_t kRank_minus_1>
LocalArray(std::size_t, LocalArray<SpanType, kRank_minus_1>)
    -> LocalArray<SpanType, kRank_minus_1 + 1>;

template <typename SpanType, std::size_t kRank_minus_1, const auto& class_v,
          const auto& class_loader_v, const auto& jvm_v>
LocalArray(std::size_t,
           LocalArray<SpanType, kRank_minus_1, class_v, class_loader_v, jvm_v>)
    -> LocalArray<SpanType, kRank_minus_1 + 1>;

template <typename SpanType, std::size_t kRank_minus_1>
LocalArray(std::size_t, LocalArray<SpanType, kRank_minus_1>&&)
    -> LocalArray<SpanType, kRank_minus_1 + 1>;

template <typename TUndecayed>
struct ProxyHelper;

}  // namespace jni

#endif  // JNI_BIND_LOCAL_ARRAY_H_
