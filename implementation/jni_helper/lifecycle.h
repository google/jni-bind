/*
 * Copyright 2023 Google LLC
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
#ifndef JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_H_
#define JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_H_

#include "jni_env.h"
#include "bind_lang/lifecycle.h"
#include "bind_lang/metaprogramming/lambda_string.h"
#include "jni_dep.h"
#include "trace.h"

namespace jni {

using ::bind_lang::LifecycleHelper;
using ::bind_lang::LifecycleType;

// Shared implementation for local jobjects (jobject, jstring).
template <typename Span>
struct LifecycleLocalBase {
  static inline void Delete(Span object) {
    Trace(bind_lang::metaprogramming::LambdaToStr(STR("DeleteLocalRef")),
          object);

#ifdef DRY_RUN
#else
    JniEnv::GetEnv()->DeleteLocalRef(object);
#endif  // DRY_RUN
  }

  static inline Span NewReference(Span object) {
    Trace(bind_lang::metaprogramming::LambdaToStr(STR("NewLocalRef")), object);

#ifdef DRY_RUN
    return Fake<Span>();
#else
    return static_cast<Span>(JniEnv::GetEnv()->NewLocalRef(object));
#endif  // DRY_RUN
  }
};

// Shared implementation for global jobjects (jobject, jstring).
template <typename Span>
struct LifecycleGlobalBase {
  static inline Span Promote(Span object) {
    Trace(bind_lang::metaprogramming::LambdaToStr(STR("NewGlobalRef")), object);

#ifdef DRY_RUN
    jobject ret = Fake<jobject>();
#else
    jobject ret = JniEnv::GetEnv()->NewGlobalRef(object);
#endif  // DRY_RUN

    Trace(bind_lang::metaprogramming::LambdaToStr(STR("DeleteLocalRef")),
          object);

#ifdef DRY_RUN
#else
    JniEnv::GetEnv()->DeleteLocalRef(object);
#endif  // DRY_RUN

    return static_cast<Span>(ret);
  }

  static inline void Delete(Span object) {
    Trace(bind_lang::metaprogramming::LambdaToStr(STR("DeleteGlobalRef")),
          object);

#ifdef DRY_RUN
#else
    JniEnv::GetEnv()->DeleteGlobalRef(object);
#endif  // DRY_RUN
  }

  static inline Span NewReference(Span object) {
    Trace(bind_lang::metaprogramming::LambdaToStr(STR("NewGlobalRef")), object);

#ifdef DRY_RUN
    return Fake<Span>();
#else
    return static_cast<Span>(JniEnv::GetEnv()->NewGlobalRef(object));
#endif  // DRY_RUN
  }
};

}  // namespace jni

// Explicit specializations for JNI handle/array types (`jobject`/`jclass` are
// specialized in `lifecycle_object.h`, and `jstring` in `lifecycle_string.h`).
// Specializations of `bind_lang::LifecycleHelper` must live in `bind_lang`.
namespace bind_lang {

#define JNI_LIFECYCLE_HELPER_SPECIALIZATION(Type)     \
  template <>                                         \
  struct LifecycleHelper<Type, LifecycleType::LOCAL>  \
      : public ::jni::LifecycleLocalBase<Type> {      \
    using Base = ::jni::LifecycleLocalBase<Type>;     \
    using Base::Base;                                 \
  };                                                  \
  template <>                                         \
  struct LifecycleHelper<Type, LifecycleType::GLOBAL> \
      : public ::jni::LifecycleGlobalBase<Type> {     \
    using Base = ::jni::LifecycleGlobalBase<Type>;    \
    using Base::Base;                                 \
  };

JNI_LIFECYCLE_HELPER_SPECIALIZATION(jthrowable)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jarray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jobjectArray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jbooleanArray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jbyteArray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jcharArray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jshortArray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jintArray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jlongArray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jfloatArray)
JNI_LIFECYCLE_HELPER_SPECIALIZATION(jdoubleArray)

#undef JNI_LIFECYCLE_HELPER_SPECIALIZATION

}  // namespace bind_lang

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_H_
