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

#include <atomic>
#include <cstddef>
#include <type_traits>

#include "implementation/array.h"
#include "implementation/array_ref.h"
#include "implementation/class.h"
#include "implementation/class_ref.h"
#include "implementation/default_class_loader.h"
#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/jni_array_helper.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/no_idx.h"
#include "implementation/object.h"
#include "jni_dep.h"

namespace jni {

// Represents a an array object (e.g. int[], float[][], Object[], etc).
// Currently GlobalArrays do not exist, as reasoning about the lifecycles of the
// underlying objects is non-trivial, e.g. a GlobalArray taking a local object
// would result in a possibly unexpected extension of lifetime.
template <typename SpanType, std::size_t kRank_ = 1,
          const auto& class_v_ = kNoClassSpecified,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalArray
    : public ArrayRef<
          JniT<SpanType, class_v_, class_loader_v_, jvm_v_, kRank_>> {
 public:
  static constexpr Class kClass{class_v_.name_};
  static constexpr std::size_t kRank = kRank_;

  using RawValT = std::conditional_t<std::is_same_v<jobject, SpanType>,
                                     std::decay_t<decltype(kClass)>, SpanType>;

  using JniT_ = JniT<SpanType, class_v_, class_loader_v_, jvm_v_, kRank_>;

  using Base =
      ArrayRef<JniT<SpanType, class_v_, class_loader_v_, jvm_v_, kRank_>>;
  using Base::Base;

  using RefTag = std::conditional_t<(kRank_ > 1), jobject, SpanType>;

  // RefTag ctor (supports multi-dimensions, `jobject` if rank > 1).
  LocalArray(std::size_t size, RefTag arr)
      : Base(JniArrayHelper<jobject, kRank_>::NewArray(
            size,
            ClassRef_t<JniT_>::GetAndMaybeLoadClassRef(
                static_cast<jobject>(arr)),
            arr)) {}

  // Rvalue ctor.
  LocalArray(LocalArray<SpanType, kRank_>&& rhs) : Base(rhs.Release()) {}

  // Rvalue ctor.
  template <typename SpanType_, std::size_t kRank, const auto& class_v,
            const auto& class_loader_v, const auto& jvm_v>
  LocalArray(LocalArray<SpanType_, kRank, class_v, class_loader_v, jvm_v>&& rhs)
      : Base(rhs.Release()) {
    static_assert(std::is_same_v<SpanType, SpanType_> && kRank == kRank_ &&
                  class_v == class_v_ && class_loader_v == class_loader_v_);
  }

  // Construct from decorated object lvalue (object is used as template).
  //
  // e.g.
  //  LocalArray arr { 5, LocalObject<kClass> {args...} };
  //  LocalArray arr { 5, GlobalObject<kClass> {args...} };
  template <template <const auto&, const auto&, const auto&>
            class ObjectContainer,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  LocalArray(
      std::size_t size,
      const ObjectContainer<class_v, class_loader_v, jvm_v>& local_object)
      : Base(JniArrayHelper<jobject, kRank_>::NewArray(
            size,
            ClassRef_t<JniT_>::GetAndMaybeLoadClassRef(
                static_cast<jobject>(local_object)),
            static_cast<jobject>(local_object))) {}

  operator jobject() { return static_cast<jobject>(Base::object_ref_); }
};

template <template <const auto&, const auto&, const auto&>
          class ObjectContainer,
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
