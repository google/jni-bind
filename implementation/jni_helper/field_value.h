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

#ifndef JNI_BIND_FIELD_VALUE_GETTER_H_
#define JNI_BIND_FIELD_VALUE_GETTER_H_

#include <cstddef>
#include <type_traits>

#include "jni_env.h"
#include "jni_dep.h"
#include "metaprogramming/lambda_string.h"
#include "trace.h"

namespace jni {

template <typename Raw, std::size_t kRank = 0, bool kStatic = false,
          typename Enable = void>
struct FieldHelper {
  static Raw GetValue(jobject object_ref, jfieldID field_ref_);

  static void SetValue(jobject object_ref, jfieldID field_ref_, Raw&& value);
};

////////////////////////////////////////////////////////////////////////////////
// Rank 0: Primitive types (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct FieldHelper<jboolean, 0, false, void> {
  static inline jboolean GetValue(const jobject object_ref,
                                  const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetBooleanValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    return jni::JniEnv::GetEnv()->GetBooleanField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jboolean&& value) {
    Trace(metaprogramming::LambdaToStr(STR("GetBooleanValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetBooleanField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jbyte, 0, false, void> {
  static inline jbyte GetValue(const jobject object_ref,
                               const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetByteValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jbyte>();
#else
    return jni::JniEnv::GetEnv()->GetByteField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jbyte&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetByteValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetByteField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jchar, 0, false, void> {
  static inline jchar GetValue(const jobject object_ref,
                               const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetCharValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jchar>();
#else
    return jni::JniEnv::GetEnv()->GetCharField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jchar&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetCharValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetCharField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jshort, 0, false, void> {
  static inline jshort GetValue(const jobject object_ref,
                                const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetShortValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jshort>();
#else
    return jni::JniEnv::GetEnv()->GetShortField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jshort&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetShortValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetShortField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jint, 0, false, void> {
  static inline jint GetValue(const jobject object_ref,
                              const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetIntValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jint>();
#else
    return jni::JniEnv::GetEnv()->GetIntField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jint&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetIntValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetIntField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jlong, 0, false, void> {
  static inline jlong GetValue(const jobject object_ref,
                               const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetLongField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jlong>();
#else
    return jni::JniEnv::GetEnv()->GetLongField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jlong&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetLongField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetLongField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jfloat, 0, false, void> {
  static inline jfloat GetValue(const jobject object_ref,
                                const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetFloatField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return 123.f;
#else
    return jni::JniEnv::GetEnv()->GetFloatField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jfloat&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetFloatField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetFloatField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jdouble, 0, false, void> {
  static inline jdouble GetValue(const jobject object_ref,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetDoubleField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return 123.;
#else
    return jni::JniEnv::GetEnv()->GetDoubleField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jdouble&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetDoubleField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetDoubleField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jobject, 0, false, void> {
  static inline jobject GetValue(const jobject object_ref,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jobject&& new_value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, new_value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jstring, 0, false, void> {
  static inline jstring GetValue(const jobject object_ref,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jstring>();
#else
    return reinterpret_cast<jstring>(
        jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jstring&& new_value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, new_value);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1: Single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <typename ArrayType>
struct BaseFieldArrayHelper {
  static inline ArrayType GetValue(const jobject object_ref,
                                   const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField, Rank 1")),
          object_ref, field_ref_);

#ifdef DRY_RUN
    return Fake<ArrayType>();
#else
    return static_cast<ArrayType>(
        jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, ArrayType&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jboolean>, kRank, false, void>
    : BaseFieldArrayHelper<jbooleanArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jbyte>, kRank, false, void>
    : BaseFieldArrayHelper<jbyteArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jchar>, kRank, false, void>
    : BaseFieldArrayHelper<jcharArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jshort>, kRank, false, void>
    : BaseFieldArrayHelper<jshortArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jint>, kRank, false, void>
    : BaseFieldArrayHelper<jintArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jlong>, kRank, false, void>
    : BaseFieldArrayHelper<jlongArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jfloat>, kRank, false, void>
    : BaseFieldArrayHelper<jfloatArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jdouble>, kRank, false, void>
    : BaseFieldArrayHelper<jdoubleArray> {};

////////////////////////////////////////////////////////////////////////////////
// Rank 1: jobjects & jstrings.
// Rank 2+: Multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t kRank>
struct FieldHelper<
    T, kRank, false,
    std::enable_if_t<(std::is_same_v<jobject, T> ||
                      std::is_same_v<jstring, T> || (kRank > 1))> > {
  static inline jobjectArray GetValue(const jobject object_ref,
                                      const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField, Rank >1")),
          object_ref, field_ref_);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jobjectArray&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField, Rank >1")),
          object_ref, field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

}  // namespace jni

#endif  // JNI_BIND_FIELD_VALUE_GETTER_H_
