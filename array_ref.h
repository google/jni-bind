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

#include <span>

#include "array.h"
#include "class.h"
#include "default_class_loader.h"
#include "jni_dep.h"
#include "jni_helper/jni_array_helper.h"
#include "ref_base.h"

namespace jni {

template <typename SpanType>
class ArrayView {
 public:
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

 private:
  const jarray array_;
  const GetArrayElementsResult<SpanType> get_array_elements_result_;
  const bool copy_on_completion_;
};

template <typename SpanType>
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

}  // namespace jni

#endif  // JNI_BIND_ARRAY_REF_H_
