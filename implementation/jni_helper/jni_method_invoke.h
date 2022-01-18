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

template <typename ReturnType, bool is_array = false>
class JniMethodInvoke {};

template <>
struct JniMethodInvoke<void, false> {
  template <typename... Ts>
  static void Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    jni::JniEnv::GetEnv()->CallVoidMethod(object, method_id,
                                          std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jboolean, false> {
  template <typename... Ts>
  static jboolean Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallBooleanMethod(object, method_id,
                                                    std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jint, false> {
  template <typename... Ts>
  static jint Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallIntMethod(object, method_id,
                                                std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jlong, false> {
  template <typename... Ts>
  static jlong Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallLongMethod(object, method_id,
                                                 std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jfloat, false> {
  template <typename... Ts>
  static jfloat Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallFloatMethod(object, method_id,
                                                  std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jdouble, false> {
  template <typename... Ts>
  static jdouble Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallDoubleMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jobject, false> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jstring, false> {
  template <typename... Ts>
  static jobject Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Array Types.
////////////////////////////////////////////////////////////////////////////////
template <>
struct JniMethodInvoke<jboolean, true> {
  template <typename... Ts>
  static jbooleanArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jbooleanArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jbyte, true> {
  template <typename... Ts>
  static jbyteArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jbyteArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jchar, true> {
  template <typename... Ts>
  static jcharArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jcharArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jshortArray, true> {
  template <typename... Ts>
  static jshortArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jshortArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jint, true> {
  template <typename... Ts>
  static jintArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jintArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jfloat, true> {
  template <typename... Ts>
  static jfloatArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jfloatArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jlong, true> {
  template <typename... Ts>
  static jlongArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jlongArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jobject, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jobjectArray, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_INVOKE_H_
