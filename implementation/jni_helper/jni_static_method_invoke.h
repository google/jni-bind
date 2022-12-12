/*
 * Copyright 2022 Google LLC
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
#ifndef JNI_BIND_IMPLEMENTATION_JNI_HELPER_JNI_STATIC_METHOD_INVOKE_H_
#define JNI_BIND_IMPLEMENTATION_JNI_HELPER_JNI_STATIC_METHOD_INVOKE_H_

#include <cstddef>
#include <utility>

#include "jni_env.h"
#include "jni_dep.h"

namespace jni {

template <typename ReturnType, std::size_t kRank = 1>
class JniStaticMethodInvoke {};

////////////////////////////////////////////////////////////////////////////////
// Rank 0 type (aka void).
////////////////////////////////////////////////////////////////////////////////
template <>
struct JniStaticMethodInvoke<void, 0> {
  template <typename... Ts>
  static void Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    jni::JniEnv::GetEnv()->CallStaticVoidMethod(clazz, method_id,
                                                std::forward<Ts>(ts)...);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1 types, i.e. the primitive type itself (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct JniStaticMethodInvoke<jboolean, 1> {
  template <typename... Ts>
  static jboolean Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticBooleanMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jbyte, 1> {
  template <typename... Ts>
  static jboolean Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticByteMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jchar, 1> {
  template <typename... Ts>
  static jboolean Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticCharMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jshort, 1> {
  template <typename... Ts>
  static jboolean Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticShortMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jint, 1> {
  template <typename... Ts>
  static jint Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticIntMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jlong, 1> {
  template <typename... Ts>
  static jlong Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticLongMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jfloat, 1> {
  template <typename... Ts>
  static jfloat Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticFloatMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jdouble, 1> {
  template <typename... Ts>
  static jdouble Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticDoubleMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jobject, 1> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct JniStaticMethodInvoke<jstring, 1> {
  template <typename... Ts>
  static jobject Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 2 types, i.e. single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jboolean>, kRank> {
  template <typename... Ts>
  static jbooleanArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jbooleanArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jbyte>, kRank> {
  template <typename... Ts>
  static jbyteArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jbyteArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jchar>, kRank> {
  template <typename... Ts>
  static jcharArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jcharArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jshort>, kRank> {
  template <typename... Ts>
  static jshortArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jshortArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jint>, kRank> {
  template <typename... Ts>
  static jintArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jintArray>(jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jfloat>, kRank> {
  template <typename... Ts>
  static jfloatArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jfloatArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jdouble>, kRank> {
  template <typename... Ts>
  static jdoubleArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jdoubleArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jlong>, kRank> {
  template <typename... Ts>
  static jlongArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jlongArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jarray>, kRank> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank == 2), jobject>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 3+ types, i.e. multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jboolean>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jbyte>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jchar>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jshort>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jint>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jfloat>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jdouble>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jlong>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jarray>, kRank> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct JniStaticMethodInvoke<std::enable_if_t<(kRank > 2), jobject>, kRank> {
  template <typename... Ts>
  static jobjectArray Invoke(jclass clazz, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_JNI_STATIC_METHOD_INVOKE_H_
