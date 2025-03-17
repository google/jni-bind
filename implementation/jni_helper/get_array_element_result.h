/*
 * Copyright 2024 Google LLC
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

#ifndef JNI_BIND_IMPLEMENTATION_JNI_HELPER_GET_ARRAY_ELEMENT_RESULT_H_
#define JNI_BIND_IMPLEMENTATION_JNI_HELPER_GET_ARRAY_ELEMENT_RESULT_H_

#include "jni_dep.h"

namespace jni {

// Convenience struct for returning results from pinning array.
template <typename SpanType>
struct GetArrayElementsResult {
  SpanType* ptr_;
  jboolean is_copy;
};

// Enum to specify array access mode (regular vs. critical)
enum class ArrayAccessMode {
  REGULAR, // Uses Get*ArrayElements API
  CRITICAL // Uses GetPrimitiveArrayCritical API
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_GET_ARRAY_ELEMENT_RESULT_H_
