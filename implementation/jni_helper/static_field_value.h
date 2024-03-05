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
#ifndef JNI_BIND_IMPLEMENTATION_JNI_HELPER_STATIC_FIELD_VALUE_H_
#define JNI_BIND_IMPLEMENTATION_JNI_HELPER_STATIC_FIELD_VALUE_H_

#include <utility>

#include "jni_env.h"
#include "implementation/jni_helper/field_value.h"
#include "jni_dep.h"
#include "metaprogramming/lambda_string.h"
#include "trace.h"

namespace jni {

////////////////////////////////////////////////////////////////////////////////
// Rank 0: Static primitive types (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct FieldHelper<jboolean, 0, true, void> {
  static inline jboolean GetValue(const jclass clazz,
                                  const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticBooleanField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticBooleanField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jboolean&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticBooleanField")), clazz,
          field_ref_, value);

    jni::JniEnv::GetEnv()->SetStaticBooleanField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jbyte, 0, true, void> {
  static inline jbyte GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticByteField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticByteField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jbyte&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticByteField")), clazz,
          field_ref_, value);

    return jni::JniEnv::GetEnv()->SetStaticByteField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jchar, 0, true, void> {
  static inline jchar GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticCharField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticCharField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jchar&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticCharField")), clazz,
          field_ref_, value);

    jni::JniEnv::GetEnv()->SetStaticCharField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jshort, 0, true, void> {
  static inline jshort GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticShortField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticShortField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jshort&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticShortField")), clazz,
          field_ref_, value);

    jni::JniEnv::GetEnv()->SetStaticShortField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jint, 0, true, void> {
  static inline jint GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticIntField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticIntField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jint&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticIntField")), clazz,
          field_ref_, value);

    jni::JniEnv::GetEnv()->SetStaticIntField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jlong, 0, true, void> {
  static inline jlong GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticLongField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticLongField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jlong&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticLongField")), clazz,
          field_ref_, value);

    jni::JniEnv::GetEnv()->SetStaticLongField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jfloat, 0, true, void> {
  static inline jfloat GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticFloatField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticFloatField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jfloat&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticFloatField")), clazz,
          field_ref_, value);

    jni::JniEnv::GetEnv()->SetStaticFloatField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jdouble, 0, true, void> {
  static inline jdouble GetValue(const jclass clazz,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticDoubleField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticDoubleField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jdouble&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticDoubleField")), clazz,
          field_ref_, value);

    jni::JniEnv::GetEnv()->SetStaticDoubleField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jobject, 0, true, void> {
  static inline jobject GetValue(const jclass clazz,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticObjectField")), clazz,
          field_ref_);

    return jni::JniEnv::GetEnv()->GetStaticObjectField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jobject&& new_value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticObjectField")), clazz,
          field_ref_, new_value);

    jni::JniEnv::GetEnv()->SetStaticObjectField(clazz, field_ref_, new_value);
  }
};

template <>
struct FieldHelper<jstring, 0, true, void> {
  static inline jstring GetValue(const jclass clazz,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticObjectField")), clazz,
          field_ref_);

    return reinterpret_cast<jstring>(
        jni::JniEnv::GetEnv()->GetStaticObjectField(clazz, field_ref_));
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jstring&& new_value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticObjectField")), clazz,
          field_ref_, new_value);

    jni::JniEnv::GetEnv()->SetStaticObjectField(clazz, field_ref_, new_value);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1: Static single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <typename ArrayType>
struct StaticBaseFieldArrayHelper {
  static inline ArrayType GetValue(const jobject object_ref,
                                   const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField")), object_ref,
          field_ref_);

    return static_cast<ArrayType>(
        jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_));
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, ArrayType&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField")), object_ref,
          field_ref_, value);

    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, value);
  }
};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jboolean>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jbooleanArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jbyte>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jbyteArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jchar>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jcharArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jshort>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jshortArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jint>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jintArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jlong>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jlongArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jfloat>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jfloatArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jdouble>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jdoubleArray> {};

////////////////////////////////////////////////////////////////////////////////
// Rank 1: Static jobjects.
// Rank 2+: Static multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t kRank>
struct FieldHelper<
    T, kRank, true,
    std::enable_if_t<(std::is_same_v<jobject, T> || (kRank > 1))> > {
  static inline jobjectArray GetValue(const jclass clazz,
                                      const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticObjectField, Rank 1+")),
          clazz, field_ref_);

    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->GetStaticObjectField(clazz, field_ref_));
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jobjectArray&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticObjectField, Rank 1+")),
          clazz, field_ref_, value);

    jni::JniEnv::GetEnv()->SetStaticObjectField(clazz, field_ref_, value);
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_STATIC_FIELD_VALUE_H_
