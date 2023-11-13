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
#ifndef JNI_BIND_LOCAL_OBJECT_H
#define JNI_BIND_LOCAL_OBJECT_H

#include <type_traits>

#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/jvm_ref.h"
#include "implementation/object_ref.h"
#include "implementation/promotion_mechanics.h"
#include "implementation/promotion_mechanics_tags.h"
#include "jni_dep.h"

namespace jni {

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
using LocalObjectImpl =
    Scoped<LifecycleType::LOCAL,
           JniT<jobject, class_v_, class_loader_v_, jvm_v_>, jobject>;

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalObject : public LocalObjectImpl<class_v_, class_loader_v_, jvm_v_> {
 public:
  using Base = LocalObjectImpl<class_v_, class_loader_v_, jvm_v_>;
  using Base::Base;

  template <typename T>
  LocalObject(ArrayViewHelper<T> array_view_helper)
      : LocalObject(AdoptLocal{}, array_view_helper.val_) {}

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  LocalObject(LocalObject<class_v, class_loader_v, jvm_v>&& obj)
      : Base(AdoptLocal{}, obj.Release()) {}
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalObject(LocalObject<class_v_, class_loader_v_, jvm_v_>&&)
    -> LocalObject<class_v_, class_loader_v_, jvm_v_>;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
bool operator==(const jobject& lhs,
                const LocalObject<class_v_, class_loader_v_, jvm_v_>& rhs) {
  return lhs == static_cast<jobject>(rhs);
}

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
bool operator!=(const jobject& lhs,
                const LocalObject<class_v_, class_loader_v_, jvm_v_>& rhs) {
  return !(lhs == rhs);
}

}  // namespace jni

#endif  // JNI_BIND_LOCAL_OBJECT_H
