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

#ifndef JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_STRING_H_
#define JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_STRING_H_

#include "jni_env.h"
#include "implementation/jni_helper/lifecycle.h"
#include "jni_dep.h"
#include "metaprogramming/lambda_string.h"
#include "trace.h"

namespace jni {

template <>
struct LifecycleHelper<jstring, LifecycleType::LOCAL>
    : public LifecycleLocalBase<jstring> {
  static inline jstring Construct(const char* chars) {
    Trace(metaprogramming::LambdaToStr(STR("NewStringUTF")), chars);

#ifdef DRY_RUN
    return Fake<jstring>();
#else
    return jni::JniEnv::GetEnv()->NewStringUTF(chars);
#endif  // DRY_RUN
  }
};

template <>
struct LifecycleHelper<jstring, LifecycleType::GLOBAL>
    : public LifecycleGlobalBase<jstring> {
  template <typename... CtorArgs>
  static inline jstring Construct(const char* chars) {
    using Local = LifecycleHelper<jstring, LifecycleType::LOCAL>;

    jstring local_string = Local::Construct(chars);
    jstring global_string = Promote(local_string);
    Local::Delete(local_string);

    return global_string;
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_STRING_H_
