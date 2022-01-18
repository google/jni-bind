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

#include <cstddef>
#include <span>
#include <type_traits>

#include "array.h"
#include "array_ref.h"
#include "class.h"
#include "class_ref.h"
#include "object.h"
#include "implementation/array.h"
#include "implementation/class_ref.h"
#include "implementation/default_class_loader.h"
#include "implementation/jni_helper/jni_array_helper.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jvm.h"
#include "jni_dep.h"

namespace jni {

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class GlobalObject;

template <typename SpanType>
struct LocalArrayTag {};

// Represents a an array object (e.g. int[], Object[], etc).
// Currently GlobalArrays do not exist, as reasoning about the lifecycles of the
// underlying objects is non-trivial, e.g. a GlobalArray taking a local object
// would result in a possibly unexpected extension of lifetime.
template <typename SpanType, const auto& class_v_ = kNoClassSpecified,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalArray : public ArrayRef<SpanType, kNoClassSpecified,
                                   kDefaultClassLoader, kDefaultJvm>,
                   LocalArrayTag<ArrayStrip_t<SpanType>> {
 public:
  using Base =
      ArrayRef<SpanType, kNoClassSpecified, kDefaultClassLoader, kDefaultJvm>;

  // Note: jintArray, jfloatArray, etc. are implicitly convertible to jarray.
  LocalArray(jarray array) : Base(array) {}

  ~LocalArray() {
    if (Base::object_ref_) {
      JniHelper::DeleteLocalObject(*Base::object_ref_);
    }
  }

  // TODO(b/406948932): Add span views for construction.
  LocalArray(std::size_t size)
      : Base(JniArrayHelper<SpanType>::NewArray(size)) {}
};

// For classes (only default class loaded objects supported).
template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalArray<jobject, class_v_, class_loader_v_, jvm_v_>
    : public ArrayRef<jobject, class_v_, class_loader_v_, jvm_v_>,
      LocalArrayTag<jobject> {
 public:
  using Base = ArrayRef<jobject, class_v_, class_loader_v_, jvm_v_>;
  using ObjectClassRefT = ClassRef_t<jvm_v_, class_loader_v_, class_v_>;

  // Note: jintArray, jfloatArray, etc. are implicitly convertible to jarray.
  LocalArray(jobjectArray array) : Base(array) {}

  // Same as above.
  template <template <const auto&, const auto&, const auto&>
            class ObjectContainer,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  // TODO(b/406948932): Add span views for construction.
  LocalArray(
      std::size_t size,
      const ObjectContainer<class_v, class_loader_v, jvm_v>& local_object)
      : Base(JniArrayHelper<jobject>::NewArray(
            size,
            ObjectClassRefT::GetAndMaybeLoadClassRef(
                static_cast<jobject>(local_object)),
            static_cast<jobject>(local_object))) {}

  // Construct from LocalObject lvalue (object is used as template).
  //
  // e.g.
  //  LocalArray arr { 5, LocalObject<kClass> {args...} };
  //  LocalArray arr { 5, GlobalObject<kClass> {args...} };
  template <template <const auto&, const auto&, const auto&>
            class ObjectContainer>
  LocalArray(std::size_t size,
             ObjectContainer<class_v_, class_loader_v_, jvm_v_>&& local_object)
      : Base(JniArrayHelper<jobject>::NewArray(
            size,
            ObjectClassRefT::GetAndMaybeLoadClassRef(
                static_cast<jobject>(local_object)),
            local_object.Release())) {}
};

template <template <const auto&, const auto&, const auto&>
          class ObjectContainer,
          const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalArray(std::size_t,
           const ObjectContainer<class_v_, class_loader_v_, jvm_v_>&)
    -> LocalArray<jobject, class_v_, class_loader_v_, jvm_v_>;

template <typename SpanType>
LocalArray(std::size_t, SpanType)
    -> LocalArray<SpanType, kNoClassSpecified, kDefaultClassLoader,
                  kDefaultJvm>;

template <const auto& array_val>
constexpr auto LocalArrayBuildFromArray() {
  using SpanType = std::decay_t<decltype(array_val.raw_type_)>;

  return LocalArray<SpanType, array_val>{jarray{nullptr}};
}

}  // namespace jni

#endif  // JNI_BIND_LOCAL_ARRAY_H_
