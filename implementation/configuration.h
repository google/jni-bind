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

#ifndef JNI_BIND_IMPLEMENTATION_CONFIGURATION_H_
#define JNI_BIND_IMPLEMENTATION_CONFIGURATION_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

namespace jni {

struct Configuration {
  // Release jclassID on JVM teardown (needed in test to  balance global IDs).
  bool release_class_ids_on_teardown_ = false;

  // Release jmethodID on JVM teardown (needed in test to  balance global IDs).
  bool release_method_ids_on_teardown_ = false;

  // Release jfieldID on JVM teardown (needed in test to  balance global IDs).
  bool release_field_ids_on_teardown_ = false;
};

static inline Configuration kConfiguration = {};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_CONFIGURATION_H_
