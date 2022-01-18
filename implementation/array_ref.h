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
#include "implementation/class.h"
#include "implementation/default_class_loader.h"
#include "implementation/jni_helper/jni_array_helper.h"
#include "implementation/ref_base.h"
#include "jni_dep.h"

namespace jni {

template <typename SpanType>
class ArrayView {
 public:
  ArrayView(ArrayView&&) = delete;
  ArrayView(const ArrayView&) = delete;

  ArrayView(jarray array, bool copy_on_completion)
      : array_(array),
        get_array_elements_result_(
            JniArrayHelper<SpanType>::GetArrayElements(array)),
        copy_on_completion_(copy_on_completion) {}

  ~ArrayView() {
    JniArrayHelper<SpanType>::ReleaseArrayElements(
        array_, get_array_elements_result_.ptr_, copy_on_completion_);
  }

  SpanType* ptr() { return get_array_elements_result_.ptr_; }

 protected:
  const jarray array_;
  const GetArrayElementsResult<SpanType> get_array_elements_result_;
  const bool copy_on_completion_;
};

// This CTAD guide is required for materialising new ArrayViews from |Pin()|
// calls as move and copy constructors are deleted.
template <typename SpanType>
ArrayView(ArrayView<SpanType>&&) -> ArrayView<SpanType>;

template <typename SpanType, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
class ArrayRef : public RefBaseTag<jarray> {
 public:
  using Base = RefBaseTag<jarray>;
  using Base::Base;

  ArrayView<SpanType> Pin(bool copy_on_completion = true) {
    return {*Base::object_ref_, copy_on_completion};
  }

  std::size_t Length() {
    return JniArrayHelper<SpanType>::GetLength(*Base::object_ref_);
  }
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class ArrayRef<jobject, class_v_, class_loader_v_, jvm_v_>
    : public RefBaseTag<jobjectArray> {
 public:
  using Base = RefBaseTag<jobjectArray>;
  using Base::Base;

  std::size_t Length() {
    return JniArrayHelper<jobject>::GetLength(*Base::object_ref_);
  }

  LocalObject<class_v_, class_loader_v_, jvm_v_> Get(std::size_t idx) {
    return {JniArrayHelper<jobject>::GetArrayElement(*Base::object_ref_, idx)};
  }

  // Note: Globals are not permitted in a local array because it makes reasoning
  // about them confusing.
  //
  // TODO(b/406948932): Permit lvalues of locals and globals as technically
  // they're both viable (the scope will be extended as expected).
  void Set(std::size_t idx,
           LocalObject<class_v_, class_loader_v_, jvm_v_>&& val) {
    return JniArrayHelper<jobject>::SetArrayElement(*Base::object_ref_, idx,
                                                    val.Release());
  }
};

}  // namespace jni

#endif  // JNI_BIND_ARRAY_REF_H_
