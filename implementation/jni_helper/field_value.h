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

#include "jni_env.h"
#include "jni_dep.h"

namespace jni {

template <typename ValueRaw>
struct FieldHelper {
  static ValueRaw GetValue(const jobject object_ref, const jfieldID field_ref_);

  static void SetValue(const jobject object_ref, const jfieldID field_ref_,
                       ValueRaw&& value);
};

template <>
inline jboolean FieldHelper<jboolean>::GetValue(const jobject object_ref,
                                                const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetBooleanField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jboolean>::SetValue(const jobject object_ref,
                                            const jfieldID field_ref_,
                                            jboolean&& value) {
  jni::JniEnv::GetEnv()->SetBooleanField(object_ref, field_ref_, value);
}

template <>
inline jbyte FieldHelper<jbyte>::GetValue(const jobject object_ref,
                                          const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetByteField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jbyte>::SetValue(const jobject object_ref,
                                         const jfieldID field_ref_,
                                         jbyte&& value) {
  jni::JniEnv::GetEnv()->SetByteField(object_ref, field_ref_, value);
}

template <>
inline jchar FieldHelper<jchar>::GetValue(const jobject object_ref,
                                          const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetCharField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jchar>::SetValue(const jobject object_ref,
                                         const jfieldID field_ref_,
                                         jchar&& value) {
  jni::JniEnv::GetEnv()->SetCharField(object_ref, field_ref_, value);
}

template <>
inline jshort FieldHelper<jshort>::GetValue(const jobject object_ref,
                                            const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetShortField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jshort>::SetValue(const jobject object_ref,
                                          const jfieldID field_ref_,
                                          jshort&& value) {
  jni::JniEnv::GetEnv()->SetShortField(object_ref, field_ref_, value);
}

template <>
inline jint FieldHelper<jint>::GetValue(const jobject object_ref,
                                        const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetIntField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jint>::SetValue(const jobject object_ref,
                                        const jfieldID field_ref_,
                                        jint&& value) {
  jni::JniEnv::GetEnv()->SetIntField(object_ref, field_ref_, value);
}

template <>
inline jlong FieldHelper<jlong>::GetValue(const jobject object_ref,
                                          const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetLongField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jlong>::SetValue(const jobject object_ref,
                                         const jfieldID field_ref_,
                                         jlong&& value) {
  jni::JniEnv::GetEnv()->SetLongField(object_ref, field_ref_, value);
}

template <>
inline jfloat FieldHelper<jfloat>::GetValue(const jobject object_ref,
                                            const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetFloatField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jfloat>::SetValue(const jobject object_ref,
                                          const jfieldID field_ref_,
                                          jfloat&& value) {
  jni::JniEnv::GetEnv()->SetFloatField(object_ref, field_ref_, value);
}

template <>
inline jdouble FieldHelper<jdouble>::GetValue(const jobject object_ref,
                                              const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetDoubleField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jdouble>::SetValue(const jobject object_ref,
                                           const jfieldID field_ref_,
                                           jdouble&& value) {
  jni::JniEnv::GetEnv()->SetDoubleField(object_ref, field_ref_, value);
}

template <>
inline jobject FieldHelper<jobject>::GetValue(const jobject object_ref,
                                              const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jobject>::SetValue(const jobject object_ref,
                                           const jfieldID field_ref_,
                                           jobject&& new_value) {
  jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, new_value);
}

}  // namespace jni

#endif  // JNI_BIND_FIELD_VALUE_GETTER_H_
