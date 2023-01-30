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

namespace jni {

////////////////////////////////////////////////////////////////////////////////
// Static Fields.
////////////////////////////////////////////////////////////////////////////////
template <>
struct FieldHelper<jboolean, 0, true> {
  static inline jboolean GetValue(const jclass clazz,
                                  const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticBooleanField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jboolean&& value) {
    jni::JniEnv::GetEnv()->SetStaticBooleanField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jbyte, 0, true> {
  static inline jbyte GetValue(const jclass clazz, const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticByteField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jbyte&& value) {
    return jni::JniEnv::GetEnv()->SetStaticByteField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jchar, 0, true> {
  static inline jchar GetValue(const jclass clazz, const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticCharField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jchar&& value) {
    jni::JniEnv::GetEnv()->SetStaticCharField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jshort, 0, true> {
  static inline jshort GetValue(const jclass clazz, const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticShortField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jshort&& value) {
    jni::JniEnv::GetEnv()->SetStaticShortField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jint, 0, true> {
  static inline jint GetValue(const jclass clazz, const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticIntField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jint&& value) {
    jni::JniEnv::GetEnv()->SetStaticIntField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jlong, 0, true> {
  static inline jlong GetValue(const jclass clazz, const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticLongField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jlong&& value) {
    jni::JniEnv::GetEnv()->SetStaticLongField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jfloat, 0, true> {
  static inline jfloat GetValue(const jclass clazz, const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticFloatField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jfloat&& value) {
    jni::JniEnv::GetEnv()->SetStaticFloatField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jdouble, 0, true> {
  static inline jdouble GetValue(const jclass clazz,
                                 const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticDoubleField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jdouble&& value) {
    jni::JniEnv::GetEnv()->SetStaticDoubleField(clazz, field_ref_, value);
  }
};

template <>
struct FieldHelper<jobject, 0, true> {
  static inline jobject GetValue(const jclass clazz,
                                 const jfieldID field_ref_) {
    return jni::JniEnv::GetEnv()->GetStaticObjectField(clazz, field_ref_);
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jobject&& new_value) {
    jni::JniEnv::GetEnv()->SetStaticObjectField(clazz, field_ref_, new_value);
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_STATIC_FIELD_VALUE_H_
