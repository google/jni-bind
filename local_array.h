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

#include <span>
#include <type_traits>

#include "array.h"
#include "array_ref.h"
#include "class.h"
#include "object.h"
#include "array.h"
#include "default_class_loader.h"
#include "jni_dep.h"
#include "jni_helper/jni_array_helper.h"
#include "jni_helper/jni_helper.h"
#include "jvm.h"

namespace jni {

template <typename SpanType>
struct LocalArrayTag {};

// TODO(b/174273621):  Add support for object arrays.
template <typename SpanType, const auto& kClass = kNoClassSpecified>
class LocalArray : public ArrayRef<SpanType>,
                   LocalArrayTag<ArrayStrip_t<SpanType>> {
 public:
  using Base = ArrayRef<SpanType>;

  // Note: jintArray, jfloatArray, etc. are implicitly convertible to jarray.
  LocalArray(jarray array) : Base(array) {}

  LocalArray(std::size_t size)
      : Base(JniArrayHelper<SpanType>::NewArray(size)) {}
};

template <const auto& array_val>
constexpr auto LocalArrayBuildFromArray() {
  using SpanType = std::decay_t<decltype(array_val.raw_type_)>;

  return LocalArray<SpanType, array_val>{jarray{nullptr}};
}

}  // namespace jni

#endif  // JNI_BIND_LOCAL_ARRAY_H_
