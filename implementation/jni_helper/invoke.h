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
#include "metaprogramming/lambda_string.h"
#include "trace.h"

namespace jni {

template <typename ReturnType, std::size_t kRank, bool kStatic>
class InvokeHelper {};

////////////////////////////////////////////////////////////////////////////////
// Rank 0 type: void
//    void is special, but for symmetry it uses rank 0 with primitives.
////////////////////////////////////////////////////////////////////////////////
template <>
struct InvokeHelper<void, 0, false> {
  template <typename... Ts>
  static void Invoke(jobject object, jclass clazz, jmethodID method_id,
                     Ts&&... ts) {
#ifdef DRY_RUN
#else
    Trace(metaprogramming::LambdaToStr(STR("CallVoidMethod")), object, clazz,
          method_id, ts...);

    jni::JniEnv::GetEnv()->CallVoidMethod(object, method_id,
                                          std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 0 types, i.e. the primitive type itself (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct InvokeHelper<jboolean, 0, false> {
  template <typename... Ts>
  static jboolean Invoke(jobject object, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    Trace(metaprogramming::LambdaToStr(STR("CallBooleanMethod")), object, clazz,
          method_id, ts...);

    return jni::JniEnv::GetEnv()->CallBooleanMethod(object, method_id,
                                                    std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jint, 0, false> {
  template <typename... Ts>
  static jint Invoke(jobject object, jclass clazz, jmethodID method_id,
                     Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallIntMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jint>();
#else
    return jni::JniEnv::GetEnv()->CallIntMethod(object, method_id,
                                                std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jlong, 0, false> {
  template <typename... Ts>
  static jlong Invoke(jobject object, jclass clazz, jmethodID method_id,
                      Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallLongMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jlong>();
#else
    return jni::JniEnv::GetEnv()->CallLongMethod(object, method_id,
                                                 std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jfloat, 0, false> {
  template <typename... Ts>
  static jfloat Invoke(jobject object, jclass clazz, jmethodID method_id,
                       Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallFloatMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    //    return Fake<jfloat>();
    return 123.f;
#else
    return jni::JniEnv::GetEnv()->CallFloatMethod(object, method_id,
                                                  std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jdouble, 0, false> {
  template <typename... Ts>
  static jdouble Invoke(jobject object, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallDoubleMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    // return Fake<jdouble>();
    return 123.f;
#else
    return jni::JniEnv::GetEnv()->CallDoubleMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jobject, 0, false> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jobject object, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallObjectMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jstring, 0, false> {
  template <typename... Ts>
  static jobject Invoke(jobject object, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallObjectMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jstring>();
#else
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1 types, i.e. single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jboolean>, kRank, false> {
  template <typename... Ts>
  static jbooleanArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                              Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jbooleanArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jbooleanArray>();
#else
    return static_cast<jbooleanArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jbyte>, kRank, false> {
  template <typename... Ts>
  static jbyteArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jbyteArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jbyteArray>();
#else
    return static_cast<jbyteArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jchar>, kRank, false> {
  template <typename... Ts>
  static jcharArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jcharArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jcharArray>();
#else
    return static_cast<jcharArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jshort>, kRank, false> {
  template <typename... Ts>
  static jshortArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                            Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jshortArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jshortArray>();
#else
    return static_cast<jshortArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jint>, kRank, false> {
  template <typename... Ts>
  static jintArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                          Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jintArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jintArray>();
#else
    return static_cast<jintArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jlong>, kRank, false> {
  template <typename... Ts>
  static jlongArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jlongArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jlongArray>();
#else
    return static_cast<jlongArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jfloat>, kRank, false> {
  template <typename... Ts>
  static jfloatArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                            Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jfloatArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jfloatArray>();
#else
    return static_cast<jfloatArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jdouble>, kRank, false> {
  template <typename... Ts>
  static jdoubleArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jdoubleArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jdoubleArray>();
#else
    return static_cast<jdoubleArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jarray>, kRank, false> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jobject>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 2+ types, i.e. multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jboolean>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jbyte>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jchar>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jshort>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jint>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jfloat>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);
#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jdouble>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jlong>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jarray>, kRank, false> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jobject>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_INVOKE_H_
