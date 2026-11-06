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
#include "bind_lang/lifecycle.h"
#include "bind_lang/metaprogramming/lambda_string.h"
#include "implementation/jni_helper/lifecycle.h"
#include "jni_dep.h"
#include "trace.h"

// Specializations of `bind_lang::LifecycleHelper` must live in `bind_lang`.
namespace bind_lang {

template <>
struct LifecycleHelper<jstring, LifecycleType::LOCAL>
    : public ::jni::LifecycleLocalBase<jstring> {
  static jstring Construct(const char* chars) {
    ::jni::Trace(bind_lang::metaprogramming::LambdaToStr(STR("NewStringUTF")),
                 chars);

#ifdef DRY_RUN
    return ::jni::Fake<jstring>();
#else
    return ::jni::JniEnv::GetEnv()->NewStringUTF(chars);
#endif  // DRY_RUN
  }
};

template <>
struct LifecycleHelper<jstring, LifecycleType::GLOBAL>
    : public ::jni::LifecycleGlobalBase<jstring> {
  template <typename... CtorArgs>
  static jstring Construct(const char* chars) {
    using Local = LifecycleHelper<jstring, LifecycleType::LOCAL>;

    jstring local_string = Local::Construct(chars);
    jstring global_string = Promote(local_string);
    Local::Delete(local_string);

    return global_string;
  }
};

}  // namespace bind_lang

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_STRING_H_
