/*
 * Copyright 2025 Google LLC
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

#ifndef JNI_BIND_GLOBAL_EXCEPTION_H_
#define JNI_BIND_GLOBAL_EXCEPTION_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include "class_defs/java_lang_exception.h"
#include "implementation/class_ref.h"
#include "implementation/default_class_loader.h"
#include "implementation/global_object.h"
#include "implementation/jni_helper/jni_env.h"
#include "implementation/jni_type.h"
#include "implementation/promotion_mechanics.h"
#include "implementation/jni_helper/lifecycle.h"
#include "implementation/jvm.h"
#include "implementation/local_exception.h"
#include "implementation/local_string.h"
#include "jni_dep.h"

namespace jni {

template <const auto& class_v_ = kJavaLangException,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class GlobalException
    : public GlobalObjectImpl<class_v_, class_loader_v_, jvm_v_> {
 public:
  using Base = GlobalObjectImpl<class_v_, class_loader_v_, jvm_v_>;
  using JniT = JniT<jobject, class_v_, class_loader_v_, jvm_v_>;
  using Base::Base;
  using SpanType = jobject;
  using LifecycleT = LifecycleHelper<jobject, LifecycleType::GLOBAL>;

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalException(GlobalObject<class_v, class_loader_v, jvm_v>&& obj)
      : Base(obj.Release()) {}

  template <typename... Ts>
  GlobalException(Ts&&... vals) : Base(std::forward<Ts&&>(vals)...) {
    RefBase<jobject>::object_ref_ =
        LifecycleT::Promote(RefBase<jobject>::object_ref_);
  }

  GlobalException() {
    RefBase<jobject>::object_ref_ =
        LifecycleT::Promote(RefBase<jobject>::object_ref_);
  }

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalException& operator=(
      LocalObject<class_v, class_loader_v, jvm_v>&& rhs) {
    Base::MaybeReleaseUnderlyingObject();
    Base::object_ref_ = rhs.Release();
    return *this;
  }

  void Throw() {
    LocalString message = (*this)("getMessage");
    ::jni::JniEnv::GetEnv()->ThrowNew(
        ::jni::ClassRef<JniT>::GetAndMaybeLoadClassRef(nullptr),
        message.PinAsStr().ToString().c_str());
  }
};

template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
GlobalException(LocalObject<class_v, class_loader_v, jvm_v>&&)
    -> GlobalException<class_v, class_loader_v, jvm_v>;

template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
GlobalException(LocalException<class_v, class_loader_v, jvm_v>&&)
    -> GlobalException<class_v, class_loader_v, jvm_v>;

template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
GlobalException(GlobalObject<class_v, class_loader_v, jvm_v>&&)
    -> GlobalException<class_v, class_loader_v, jvm_v>;

template <const auto& class_v_ = kJavaLangException,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
GlobalException(jobject) -> GlobalException<class_v_, class_loader_v_, jvm_v_>;

}  // namespace jni

#endif  // JNI_BIND_GLOBAL_EXCEPTION_H_
