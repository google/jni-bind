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

#ifndef JNI_BIND_METHOD_INVOKE_H_
#define JNI_BIND_METHOD_INVOKE_H_

#include <utility>

#include "jni_env.h"
#include "jni_dep.h"

namespace jni {

struct NoModifiersTag {};
struct ArrayTag {};

template <typename ReturnType, typename Modifiers = NoModifiersTag>
class JniMethodInvoke {};

template <>
struct JniMethodInvoke<void> {
  template <typename... Ts>
  static void Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    jni::JniEnv::GetEnv()->CallVoidMethod(object, method_id,
                                          std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jboolean> {
  template <typename... Ts>
  static jboolean Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallBooleanMethod(object, method_id,
                                                    std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jint> {
  template <typename... Ts>
  static jint Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallIntMethod(object, method_id,
                                                std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jlong> {
  template <typename... Ts>
  static jlong Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallLongMethod(object, method_id,
                                                 std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jfloat> {
  template <typename... Ts>
  static jfloat Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallFloatMethod(object, method_id,
                                                  std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jdouble> {
  template <typename... Ts>
  static jdouble Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallDoubleMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jobject> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jstring> {
  template <typename... Ts>
  static jobject Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_INVOKE_H_
