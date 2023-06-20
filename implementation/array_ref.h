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

#include "implementation/array.h"
#include "implementation/array_view.h"
#include "implementation/class.h"
#include "implementation/default_class_loader.h"
#include "implementation/jni_helper/jni_array_helper.h"
#include "implementation/jni_helper/lifecycle.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/local_object.h"
#include "implementation/object_ref.h"
#include "implementation/ref_base.h"
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

  ArrayRef(std::nullptr_t) {}
  ArrayRef(std::size_t size)
      : Base(JniArrayHelper<SpanType, JniT::kRank>::NewArray(size)) {}

  explicit ArrayRef(int size) : ArrayRef(static_cast<std::size_t>(size)) {}

  ArrayView<SpanType, JniT::kRank> Pin(bool copy_on_completion = true) {
    return {Base::object_ref_, copy_on_completion, Length()};
  }

  std::size_t Length() {
    return JniArrayHelper<SpanType, JniT::kRank>::GetLength(Base::object_ref_);
  }
};

template <typename JniT>
class ArrayRef<
    JniT, std::enable_if_t<std::is_same_v<typename JniT::SpanType, jobject>>>
    : public ScopedArrayImpl<JniT> {
 public:
  using Base = ScopedArrayImpl<JniT>;
  using Base::Base;
  using SpanType = jobject;

  // Construct from jobject lvalue (object is used as template).
  explicit ArrayRef(std::size_t size, jobject obj)
      : Base(JniArrayHelper<jobject, JniT::kRank>::NewArray(
            size,
            ClassRef_t<JniT>::GetAndMaybeLoadClassRef(
                static_cast<jobject>(obj)),
            static_cast<jobject>(obj))) {}

  // Construct from LocalObject lvalue (object is used as template).
  //
  // e.g.
  //  LocalArray arr { 5, LocalObject<kClass> {args...} };
  //  LocalArray arr { 5, GlobalObject<kClass> {args...} };
  template <template <const auto&, const auto&, const auto&>
            class ObjectContainer,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  ArrayRef(std::size_t size,
           const ObjectContainer<class_v, class_loader_v, jvm_v>& obj)
      : ArrayRef(size, static_cast<jobject>(obj)) {}

  std::size_t Length() {
    return JniArrayHelper<jobject, JniT::kRank>::GetLength(Base::object_ref_);
  }

  // Object arrays cannot be efficiently pinned like primitive types can.
  ArrayView<jobject, JniT::kRank> Pin() {
    return {Base::object_ref_, false, Length()};
  }

  LocalObject<JniT::class_v, JniT::class_loader_v, JniT::jvm_v> Get(
      std::size_t idx) {
    return {JniArrayHelper<jobject, JniT::kRank>::GetArrayElement(
        Base::object_ref_, idx)};
  }

  // Note: Globals are not permitted in a local array because it makes reasoning
  // about them confusing.
  //
  // TODO(b/406948932): Permit lvalues of locals and globals as technically
  // they're both viable (the scope will be extended as expected).
  void Set(
      std::size_t idx,
      LocalObject<JniT::class_v, JniT::class_loader_v, JniT::jvm_v>&& val) {
    return JniArrayHelper<jobject, JniT::kRank>::SetArrayElement(
        Base::object_ref_, idx, val.Release());
  }
};

}  // namespace jni

#endif  // JNI_BIND_ARRAY_REF_H_
