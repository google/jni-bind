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

#ifndef JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_OBJECT_H_
#define JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_OBJECT_H_

#include "implementation/jni_helper/jni_env.h"
#include "implementation/jni_helper/lifecycle.h"
#include "jni_dep.h"
#include "metaprogramming/lambda_string.h"
#include "trace.h"

namespace jni {

// jobject.
template <>
struct LifecycleHelper<jobject, LifecycleType::LOCAL>
    : public LifecycleLocalBase<jobject> {
  template <typename... CtorArgs>
  static inline jobject Construct(jclass clazz, jmethodID ctor_method,
                                  CtorArgs&&... ctor_args) {
    Trace(metaprogramming::LambdaToStr(STR("NewObject")), clazz, ctor_method,
          ctor_args...);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return JniEnv::GetEnv()->NewObject(clazz, ctor_method, ctor_args...);
#endif  // DRY_RUN
  }
};

template <>
struct LifecycleHelper<jobject, LifecycleType::GLOBAL>
    : public LifecycleGlobalBase<jobject> {
  template <typename... CtorArgs>
  static inline jobject Construct(jclass clazz, jmethodID ctor_method,
                                  CtorArgs&&... ctor_args) {
    using Local = LifecycleHelper<jobject, LifecycleType::LOCAL>;

    jobject local_object = Local::Construct(
        clazz, ctor_method, std::forward<CtorArgs&&>(ctor_args)...);
    jobject global_object = Promote(local_object);
    Local::Delete(local_object);

    return global_object;
  }
};

// jclass.
template <>
struct LifecycleHelper<jclass, LifecycleType::LOCAL>
    : public LifecycleLocalBase<jclass> {};

template <>
struct LifecycleHelper<jclass, LifecycleType::GLOBAL>
    : public LifecycleGlobalBase<jclass> {};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_OBJECT_H_
