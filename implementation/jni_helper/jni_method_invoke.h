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

template <typename ReturnType, std::size_t kRank = 1>
class JniMethodInvoke {};

////////////////////////////////////////////////////////////////////////////////
// Rank 0 type (aka void).
////////////////////////////////////////////////////////////////////////////////
template <>
struct JniMethodInvoke<void, 0> {
  template <typename... Ts>
  static void Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    jni::JniEnv::GetEnv()->CallVoidMethod(object, method_id,
                                          std::forward<Ts>(ts)...);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1 types, i.e. the primitive type itself (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct JniMethodInvoke<jboolean, 1> {
  template <typename... Ts>
  static jboolean Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallBooleanMethod(object, method_id,
                                                    std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jint, 1> {
  template <typename... Ts>
  static jint Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallIntMethod(object, method_id,
                                                std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jlong, 1> {
  template <typename... Ts>
  static jlong Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallLongMethod(object, method_id,
                                                 std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jfloat, 1> {
  template <typename... Ts>
  static jfloat Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallFloatMethod(object, method_id,
                                                  std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jdouble, 1> {
  template <typename... Ts>
  static jdouble Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallDoubleMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jobject, 1> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jstring, 1> {
  template <typename... Ts>
  static jobject Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 2+ types, i.e. single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <>
struct JniMethodInvoke<jbooleanArray, 2> {
  template <typename... Ts>
  static jbooleanArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jbooleanArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jbyteArray, 2> {
  template <typename... Ts>
  static jbyteArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jbyteArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jcharArray, 2> {
  template <typename... Ts>
  static jcharArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jcharArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jshortArray, 2> {
  template <typename... Ts>
  static jshortArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jshortArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jintArray, 2> {
  template <typename... Ts>
  static jintArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jintArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jfloatArray, 2> {
  template <typename... Ts>
  static jfloatArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jfloatArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jdoubleArray, 2> {
  template <typename... Ts>
  static jdoubleArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jdoubleArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jlongArray, 2> {
  template <typename... Ts>
  static jlongArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jlongArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jarray, 2> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jobjectArray, 2> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_INVOKE_H_
