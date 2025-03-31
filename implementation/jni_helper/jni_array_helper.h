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

#include <cstddef>

#include "get_array_element_result.h"
#include "implementation/jni_helper/jni_env.h"
#include "jni_dep.h"
#include "metaprogramming/lambda_string.h"
#include "trace.h"

namespace jni {

struct JniArrayHelperBase {
  static inline std::size_t GetLength(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayLength")), array);

#ifdef DRY_RUN
    return Fake<std::size_t>();
#else
    return jni::JniEnv::GetEnv()->GetArrayLength(array);
#endif  // DRY_RUN
  }
};

// Rank 2+ arrays all behave like object arrays.
template <typename SpannedType, std::size_t kRank>
struct JniArrayHelper : public JniArrayHelperBase {
  using AsArrayType = jobjectArray;

  static inline jobjectArray NewArray(std::size_t size,
                                      jclass class_id = nullptr,
                                      jobject initial_element = nullptr) {
    Trace(metaprogramming::LambdaToStr(STR("NewObjectArray")), size, class_id,
          initial_element);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return jni::JniEnv::GetEnv()->NewObjectArray(size, class_id,
                                                 initial_element);
#endif  // DRY_RUN
  }

  // The API of fetching objects only permits accessing one object at a time.
  static inline jobject GetArrayElement(jobjectArray array, std::size_t idx) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectArrayElement")), array,
          idx);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->GetObjectArrayElement(array, idx);
#endif  // DRY_RUN
  };

  // The API of fetching objects only permits accessing one object at a time.
  static inline void SetArrayElement(jobjectArray array, std::size_t idx,
                                     SpannedType obj) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectArrayElement")), array,
          idx, obj);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectArrayElement(array, idx, obj);
#endif  // DRY_RUN
  };
};

template <>
struct JniArrayHelper<jboolean, 1> : public JniArrayHelperBase {
  using AsArrayType = jbooleanArray;

  static inline jbooleanArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewBooleanArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jbooleanArray>();
#else
    return jni::JniEnv::GetEnv()->NewBooleanArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jboolean> GetArrayElements(
      jarray array) {
    Trace(
        metaprogramming::LambdaToStr(STR("GetArrayElements, jboolean, Rank 1")),
        array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jboolean>{};
#else
    GetArrayElementsResult<jboolean> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetBooleanArrayElements(
        static_cast<jbooleanArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jboolean* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jboolean, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseBooleanArrayElements(
        static_cast<jbooleanArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jbyte, 1> : public JniArrayHelperBase {
  using AsArrayType = jbyteArray;

  static inline jbyteArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewByteArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jbyteArray>();
#else
    return jni::JniEnv::GetEnv()->NewByteArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jbyte> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jbyte, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jbyte>{};
#else
    GetArrayElementsResult<jbyte> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetByteArrayElements(
        static_cast<jbyteArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jbyte* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jbyte, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseByteArrayElements(
        static_cast<jbyteArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jchar, 1> : public JniArrayHelperBase {
  using AsArrayType = jcharArray;

  static inline jcharArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewCharArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jcharArray>();
#else
    return jni::JniEnv::GetEnv()->NewCharArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jchar> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jchar, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jchar>{};
#else
    GetArrayElementsResult<jchar> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetCharArrayElements(
        static_cast<jcharArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jchar* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jchar, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseCharArrayElements(
        static_cast<jcharArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jshort, 1> : public JniArrayHelperBase {
  using AsArrayType = jshortArray;

  static inline jshortArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewShortArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jshortArray>();
#else
    return jni::JniEnv::GetEnv()->NewShortArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jshort> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jshort, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jshort>{};
#else
    GetArrayElementsResult<jshort> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetShortArrayElements(
        static_cast<jshortArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jshort* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jshort, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseShortArrayElements(
        static_cast<jshortArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jint, 1> : public JniArrayHelperBase {
  using AsArrayType = jintArray;

  static inline jintArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewIntArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jintArray>();
#else
    return jni::JniEnv::GetEnv()->NewIntArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jint> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jint, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jint>{};
#else
    GetArrayElementsResult<jint> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetIntArrayElements(
        static_cast<jintArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jint* native_ptr,
                                          bool copy_on_completion) {
    Trace(
        metaprogramming::LambdaToStr(STR("ReleaseArrayElements, jint, Rank 1")),
        array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseIntArrayElements(
        static_cast<jintArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jlong, 1> : public JniArrayHelperBase {
  using AsArrayType = jlongArray;

  static inline jlongArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewLongArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jlongArray>();
#else
    return jni::JniEnv::GetEnv()->NewLongArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jlong> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jlong, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jlong>{};
#else
    GetArrayElementsResult<jlong> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetLongArrayElements(
        static_cast<jlongArray>(array), &return_value.is_copy);
    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jlong* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jlong, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseLongArrayElements(
        static_cast<jlongArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jfloat, 1> : public JniArrayHelperBase {
  using AsArrayType = jfloatArray;

  static inline jfloatArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewFloatArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jfloatArray>();
#else
    return jni::JniEnv::GetEnv()->NewFloatArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jfloat> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jfloat, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jfloat>{};
#else
    GetArrayElementsResult<jfloat> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetFloatArrayElements(
        static_cast<jfloatArray>(array), &return_value.is_copy);
    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jfloat* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jfloat, Rank 1")),
          array, native_ptr, copy_on_completion);

    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseFloatArrayElements(
        static_cast<jfloatArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jdouble, 1> : public JniArrayHelperBase {
  using AsArrayType = jdoubleArray;

  static inline jdoubleArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewDoubleArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jdoubleArray>();
#else
    return jni::JniEnv::GetEnv()->NewDoubleArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jdouble> GetArrayElements(jarray array) {
    Trace(
        metaprogramming::LambdaToStr(STR("GetArrayElements, jdouble, Rank 1")),
        array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jdouble>();
#else
    GetArrayElementsResult<jdouble> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetDoubleArrayElements(
        static_cast<jdoubleArray>(array), &return_value.is_copy);
    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jdouble* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jdouble, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseDoubleArrayElements(
        static_cast<jdoubleArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

// Note, this requires both a jclass and a sample jobject to build from which
// is unlike any other new array construction.
template <std::size_t kRank>
struct JniArrayHelper<jobject, kRank> : public JniArrayHelperBase {
  using AsArrayType = jobjectArray;

  static inline jobjectArray NewArray(std::size_t size, jclass class_id,
                                      jobject initial_element) {
    Trace(metaprogramming::LambdaToStr(STR("NewArray, Rank >1")), kRank);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return jni::JniEnv::GetEnv()->NewObjectArray(size, class_id,
                                                 initial_element);
#endif  // DRY_RUN
  }

  // The API of fetching objects only permits accessing one object at a time.
  static inline jobject GetArrayElement(jobjectArray array, std::size_t idx) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElement, Rank >1")), kRank);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->GetObjectArrayElement(array, idx);
#endif  // DRY_RUN
  };

  // The API of fetching objects only permits accessing one object at a time.
  static inline void SetArrayElement(jobjectArray array, std::size_t idx,
                                     jobject obj) {
    Trace(metaprogramming::LambdaToStr(STR("SetArrayElement, Rank >1")), kRank);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectArrayElement(array, idx, obj);
#endif  // DRY_RUN
  };
};

}  // namespace jni

#endif  // JNI_BIND_JNI_HELPER_JNI_ARRAY_HELPER_H_
