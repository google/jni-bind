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
#ifndef JNI_BIND_JNI_HELPER_JNI_ARRAY_HELPER_H_
#define JNI_BIND_JNI_HELPER_JNI_ARRAY_HELPER_H_

#include <type_traits>

#include "implementation//jni_helper/jni_env.h"
#include "jni_dep.h"

namespace jni {

// Convenience struct for returning results from pinning array.
template <typename SpanType>
struct GetArrayElementsResult {
  SpanType* ptr_;
  jboolean is_copy;
};

struct JniArrayHelperBase {
  static inline std::size_t GetLength(jarray array) {
    return jni::JniEnv::GetEnv()->GetArrayLength(array);
  }
};

// Rank 2+ arrays all behave like object arrays.
template <typename SpannedType, std::size_t kRank>
struct JniArrayHelper : public JniArrayHelperBase {
  static inline jobjectArray NewArray(std::size_t size,
                                      jclass class_id = nullptr,
                                      jobject initial_element = nullptr) {
    return jni::JniEnv::GetEnv()->NewObjectArray(size, class_id,
                                                 initial_element);
  }

  // The API of fetching objects only permits accessing one object at a time.
  static inline jobject GetArrayElement(jobjectArray array, std::size_t idx) {
    return jni::JniEnv::GetEnv()->GetObjectArrayElement(array, idx);
  };

  // The API of fetching objects only permits accessing one object at a time.
  static inline void SetArrayElement(jobjectArray array, std::size_t idx,
                                     SpannedType obj) {
    jni::JniEnv::GetEnv()->SetObjectArrayElement(array, idx, obj);
  };
};

template <>
struct JniArrayHelper<jboolean, 1> : public JniArrayHelperBase {
  using AsArrayType = jbooleanArray;

  static inline jbooleanArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewBooleanArray(size);
  }

  static inline GetArrayElementsResult<jboolean> GetArrayElements(
      jarray array) {
    GetArrayElementsResult<jboolean> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetBooleanArrayElements(
        static_cast<jbooleanArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jboolean* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseBooleanArrayElements(
        static_cast<jbooleanArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jbyte, 1> : public JniArrayHelperBase {
  using AsArrayType = jbyteArray;

  static inline jbyteArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewByteArray(size);
  }

  static inline GetArrayElementsResult<jbyte> GetArrayElements(jarray array) {
    GetArrayElementsResult<jbyte> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetByteArrayElements(
        static_cast<jbyteArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jbyte* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseByteArrayElements(
        static_cast<jbyteArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jchar, 1> : public JniArrayHelperBase {
  using AsArrayType = jcharArray;

  static inline jcharArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewCharArray(size);
  }

  static inline GetArrayElementsResult<jchar> GetArrayElements(jarray array) {
    GetArrayElementsResult<jchar> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetCharArrayElements(
        static_cast<jcharArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jchar* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseCharArrayElements(
        static_cast<jcharArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jshort, 1> : public JniArrayHelperBase {
  using AsArrayType = jshortArray;

  static inline jshortArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewShortArray(size);
  }

  static inline GetArrayElementsResult<jshort> GetArrayElements(jarray array) {
    GetArrayElementsResult<jshort> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetShortArrayElements(
        static_cast<jshortArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jshort* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseShortArrayElements(
        static_cast<jshortArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jint, 1> : public JniArrayHelperBase {
  using AsArrayType = jintArray;

  static inline jintArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewIntArray(size);
  }

  static inline GetArrayElementsResult<jint> GetArrayElements(jarray array) {
    GetArrayElementsResult<jint> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetIntArrayElements(
        static_cast<jintArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, int* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseIntArrayElements(
        static_cast<jintArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jlong, 1> : public JniArrayHelperBase {
  using AsArrayType = jlongArray;

  static inline jlongArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewLongArray(size);
  }

  static inline GetArrayElementsResult<jlong> GetArrayElements(jarray array) {
    GetArrayElementsResult<jlong> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetLongArrayElements(
        static_cast<jlongArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jlong* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseLongArrayElements(
        static_cast<jlongArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jfloat, 1> : public JniArrayHelperBase {
  using AsArrayType = jfloatArray;

  static inline jfloatArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewFloatArray(size);
  }

  static inline GetArrayElementsResult<jfloat> GetArrayElements(jarray array) {
    GetArrayElementsResult<jfloat> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetFloatArrayElements(
        static_cast<jfloatArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jfloat* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseFloatArrayElements(
        static_cast<jfloatArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jdouble, 1> : public JniArrayHelperBase {
  using AsArrayType = jdoubleArray;

  static inline jdoubleArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewDoubleArray(size);
  }

  static inline GetArrayElementsResult<jdouble> GetArrayElements(jarray array) {
    GetArrayElementsResult<jdouble> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetDoubleArrayElements(
        static_cast<jdoubleArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jdouble* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseDoubleArrayElements(
        static_cast<jdoubleArray>(array), native_ptr, copy_back_mode);
  }
};

// Note, this requires both a jclass and a sample jobject to build from which
// is unlike any other new array construction.
template <std::size_t kRank>
struct JniArrayHelper<jobject, kRank> : public JniArrayHelperBase {
  static inline jobjectArray NewArray(std::size_t size, jclass class_id,
                                      jobject initial_element) {
    return jni::JniEnv::GetEnv()->NewObjectArray(size, class_id,
                                                 initial_element);
  }

  // The API of fetching objects only permits accessing one object at a time.
  static inline jobject GetArrayElement(jobjectArray array, std::size_t idx) {
    return jni::JniEnv::GetEnv()->GetObjectArrayElement(array, idx);
  };

  // The API of fetching objects only permits accessing one object at a time.
  static inline void SetArrayElement(jobjectArray array, std::size_t idx,
                                     jobject obj) {
    jni::JniEnv::GetEnv()->SetObjectArrayElement(array, idx, obj);
  };
};

}  // namespace jni

#endif  // JNI_BIND_JNI_HELPER_JNI_ARRAY_HELPER_H_
