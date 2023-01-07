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

#include "jni_env.h"
#include "jni_dep.h"

namespace jni {

template <typename Raw, std::size_t kRank>
struct FieldHelper {
  static Raw GetValue(jobject object_ref, jfieldID field_ref_);

  static void SetValue(jobject object_ref, jfieldID field_ref_, Raw&& value);
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1+.
////////////////////////////////////////////////////////////////////////////////
template <>
inline jboolean FieldHelper<jboolean, 1>::GetValue(const jobject object_ref,
                                                   const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetBooleanField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jboolean, 1>::SetValue(const jobject object_ref,
                                               const jfieldID field_ref_,
                                               jboolean&& value) {
  jni::JniEnv::GetEnv()->SetBooleanField(object_ref, field_ref_, value);
}

template <>
inline jbyte FieldHelper<jbyte, 1>::GetValue(const jobject object_ref,
                                             const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetByteField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jbyte, 1>::SetValue(const jobject object_ref,
                                            const jfieldID field_ref_,
                                            jbyte&& value) {
  jni::JniEnv::GetEnv()->SetByteField(object_ref, field_ref_, value);
}

template <>
inline jchar FieldHelper<jchar, 1>::GetValue(const jobject object_ref,
                                             const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetCharField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jchar, 1>::SetValue(const jobject object_ref,
                                            const jfieldID field_ref_,
                                            jchar&& value) {
  jni::JniEnv::GetEnv()->SetCharField(object_ref, field_ref_, value);
}

template <>
inline jshort FieldHelper<jshort, 1>::GetValue(const jobject object_ref,
                                               const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetShortField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jshort, 1>::SetValue(const jobject object_ref,
                                             const jfieldID field_ref_,
                                             jshort&& value) {
  jni::JniEnv::GetEnv()->SetShortField(object_ref, field_ref_, value);
}

template <>
inline jint FieldHelper<jint, 1>::GetValue(const jobject object_ref,
                                           const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetIntField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jint, 1>::SetValue(const jobject object_ref,
                                           const jfieldID field_ref_,
                                           jint&& value) {
  jni::JniEnv::GetEnv()->SetIntField(object_ref, field_ref_, value);
}

template <>
inline jlong FieldHelper<jlong, 1>::GetValue(const jobject object_ref,
                                             const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetLongField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jlong, 1>::SetValue(const jobject object_ref,
                                            const jfieldID field_ref_,
                                            jlong&& value) {
  jni::JniEnv::GetEnv()->SetLongField(object_ref, field_ref_, value);
}

template <>
inline jfloat FieldHelper<jfloat, 1>::GetValue(const jobject object_ref,
                                               const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetFloatField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jfloat, 1>::SetValue(const jobject object_ref,
                                             const jfieldID field_ref_,
                                             jfloat&& value) {
  jni::JniEnv::GetEnv()->SetFloatField(object_ref, field_ref_, value);
}

template <>
inline jdouble FieldHelper<jdouble, 1>::GetValue(const jobject object_ref,
                                                 const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetDoubleField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jdouble, 1>::SetValue(const jobject object_ref,
                                              const jfieldID field_ref_,
                                              jdouble&& value) {
  jni::JniEnv::GetEnv()->SetDoubleField(object_ref, field_ref_, value);
}

template <>
inline jobject FieldHelper<jobject, 1>::GetValue(const jobject object_ref,
                                                 const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_);
}

// template <>

template <std::size_t kRank,
          typename T = std::enable_if_t<(kRank == 1), jobject>>
inline void FieldHelper<T, kRank>::SetValue(const jobject object_ref,
                                            const jfieldID field_ref_,
                                            jobject&& new_value) {
  jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, new_value);
}

////////////////////////////////////////////////////////////////////////////////
// Rank 2+.
////////////////////////////////////////////////////////////////////////////////
/*
template <std::size_t kRank>
inline void FieldHelper<
  std::enable_if_t<(kRank > 1), jobject>, kRank>
>::SetValue(const jobject object_ref, const jfieldID field_ref_,
                                           jobject&& new_value) {
  jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, new_value);
}
*/

}  // namespace jni

#endif  // JNI_BIND_FIELD_VALUE_GETTER_H_
