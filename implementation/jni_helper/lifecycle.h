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

#include <utility>

#include "jni_env.h"
#include "jni_dep.h"

namespace jni {

enum class LifecycleType {
  LOCAL,
  GLOBAL,
  // WEAK, // not implemented yet.
};

template <typename Span, LifecycleType lifecycle_type>
struct LifecycleHelper;

// Shared implementation for local jobjects (jobject, jstring).
template <typename Span>
struct LifecycleLocalBase {
  static inline void Delete(Span object) {
    JniEnv::GetEnv()->DeleteLocalRef(object);
  }

  static inline Span NewReference(Span object) {
    return static_cast<Span>(JniEnv::GetEnv()->NewLocalRef(object));
  }
};

template <typename Span>
struct LifecycleHelper<Span, LifecycleType::LOCAL>
    : public LifecycleLocalBase<Span> {
  using Base = LifecycleLocalBase<Span>;
  using Base::Base;
};

// Shared implementation for global jobjects (jobject, jstring).
template <typename Span>
struct LifecycleGlobalBase {
  static inline Span Promote(Span object) {
    jobject ret = JniEnv::GetEnv()->NewGlobalRef(object);
    JniEnv::GetEnv()->DeleteLocalRef(object);

    return static_cast<Span>(ret);
  }

  static inline void Delete(Span object) {
    JniEnv::GetEnv()->DeleteGlobalRef(object);
  }

  static inline Span NewReference(Span object) {
    return static_cast<Span>(JniEnv::GetEnv()->NewGlobalRef(object));
  }
};

template <typename Span>
struct LifecycleHelper<Span, LifecycleType::GLOBAL>
    : public LifecycleLocalBase<Span> {
  using Base = LifecycleGlobalBase<Span>;
  using Base::Base;
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_HELPER_LIFECYCLE_H_
