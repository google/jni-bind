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

#ifndef JNI_BIND_GLOBAL_OBJECT_H_
#define JNI_BIND_GLOBAL_OBJECT_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include "implementation/class.h"
#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/local_object.h"
#include "implementation/object_ref.h"
#include "implementation/promotion_mechanics.h"
#include "jni_dep.h"
#include "metaprogramming/deep_equal_diminished.h"

namespace jni {

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
using GlobalObjectImpl =
    Scoped<LifecycleType::GLOBAL,
           JniT<jobject, class_v_, class_loader_v_, jvm_v_>, jobject>;

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class GlobalObject
    : public GlobalObjectImpl<class_v_, class_loader_v_, jvm_v_> {
 public:
  using Base = GlobalObjectImpl<class_v_, class_loader_v_, jvm_v_>;
  using Base::Base;
  using LifecycleT = LifecycleHelper<jobject, LifecycleType::GLOBAL>;

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject(GlobalObject<class_v, class_loader_v, jvm_v>&& obj)
      : Base(obj.Release()) {}

  template <typename... Ts>
  GlobalObject(Ts&&... vals) : Base(std::forward<Ts&&>(vals)...) {
    RefBaseTag<jobject>::object_ref_ =
        LifecycleT::Promote(RefBaseTag<jobject>::object_ref_);
  }

  GlobalObject() {
    RefBaseTag<jobject>::object_ref_ =
        LifecycleT::Promote(RefBaseTag<jobject>::object_ref_);
  }

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject& operator=(LocalObject<class_v, class_loader_v, jvm_v>&& rhs) {
    static_assert(::jni::metaprogramming::DeepEqualDiminished_v<
                  LocalObject<class_v_, class_loader_v_, jvm_v_>,
                  LocalObject<class_v, class_loader_v, jvm_v>>);
    Base::MaybeReleaseUnderlyingObject();
    Base::object_ref_ = rhs.Release();

    return *this;
  }
};

template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
GlobalObject(LocalObject<class_v, class_loader_v, jvm_v>&&)
    -> GlobalObject<class_v, class_loader_v, jvm_v>;

}  // namespace jni

#endif  // JNI_BIND_GLOBAL_OBJECT_H_
