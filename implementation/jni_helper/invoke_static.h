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

#include "invoke.h"
#include "jni_env.h"
#include "jni_dep.h"
#include "trace.h"

namespace jni {

////////////////////////////////////////////////////////////////////////////////
// Rank 0 type (aka void).
////////////////////////////////////////////////////////////////////////////////
template <>
struct InvokeHelper<void, 0, true> {
  template <typename... Ts>
  static void Invoke(jobject, jclass clazz, jmethodID method_id, Ts&&... ts) {
    Trace("CallStaticVoidMethod", clazz, method_id, ts...);

    jni::JniEnv::GetEnv()->CallStaticVoidMethod(clazz, method_id,
                                                std::forward<Ts>(ts)...);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 0 types, i.e. the primitive type itself (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct InvokeHelper<jboolean, 0, true> {
  template <typename... Ts>
  static jboolean Invoke(jobject, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
    Trace("CallStaticBooleanMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticBooleanMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jbyte, 0, true> {
  template <typename... Ts>
  static jboolean Invoke(jobject, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
    Trace("CallStaticByteMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticByteMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jchar, 0, true> {
  template <typename... Ts>
  static jboolean Invoke(jobject, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
    Trace("CallStaticCharMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticCharMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jshort, 0, true> {
  template <typename... Ts>
  static jboolean Invoke(jobject, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
    Trace("CallStaticShortMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticShortMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jint, 0, true> {
  template <typename... Ts>
  static jint Invoke(jobject, jclass clazz, jmethodID method_id, Ts&&... ts) {
    Trace("CallStaticIntMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticIntMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jlong, 0, true> {
  template <typename... Ts>
  static jlong Invoke(jobject, jclass clazz, jmethodID method_id, Ts&&... ts) {
    Trace("CallStaticLongMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticLongMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jfloat, 0, true> {
  template <typename... Ts>
  static jfloat Invoke(jobject, jclass clazz, jmethodID method_id, Ts&&... ts) {
    Trace("CallStaticFloatMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticFloatMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jdouble, 0, true> {
  template <typename... Ts>
  static jdouble Invoke(jobject, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace("CallStaticDoubleMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticDoubleMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jobject, 0, true> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jobject, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace("CallStaticObjectMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

template <>
struct InvokeHelper<jstring, 0, true> {
  template <typename... Ts>
  static jobject Invoke(jobject, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace("CallStaticObjectMethod", clazz, method_id, ts...);

    return jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1 types, i.e. single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jboolean>, kRank, true> {
  template <typename... Ts>
  static jbooleanArray Invoke(jobject, jclass clazz, jmethodID method_id,
                              Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jbooleanArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jbyte>, kRank, true> {
  template <typename... Ts>
  static jbyteArray Invoke(jobject, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jbyteArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jchar>, kRank, true> {
  template <typename... Ts>
  static jcharArray Invoke(jobject, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jcharArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jshort>, kRank, true> {
  template <typename... Ts>
  static jshortArray Invoke(jobject, jclass clazz, jmethodID method_id,
                            Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jshortArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jint>, kRank, true> {
  template <typename... Ts>
  static jintArray Invoke(jobject, jclass clazz, jmethodID method_id,
                          Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jintArray>(jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jfloat>, kRank, true> {
  template <typename... Ts>
  static jfloatArray Invoke(jobject, jclass clazz, jmethodID method_id,
                            Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jfloatArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jdouble>, kRank, true> {
  template <typename... Ts>
  static jdoubleArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jdoubleArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jlong>, kRank, true> {
  template <typename... Ts>
  static jlongArray Invoke(jobject, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jlongArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jarray>, kRank, true> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jobject>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod, Rank 1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 2+ types, i.e. multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jboolean>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jboolean), Rank >1", clazz, method_id,
          ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jbyte>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jbyte), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jchar>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jchar), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jshort>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jshort), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jint>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jint), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jfloat>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jfloat), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jdouble>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jdouble), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jlong>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jlong), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jarray>, kRank, true> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jarray), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jobject>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace("CallStaticObjectMethod (jobject), Rank >1", clazz, method_id, ts...);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_JNI_STATIC_METHOD_INVOKE_H_
