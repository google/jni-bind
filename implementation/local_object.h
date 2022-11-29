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
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jvm.h"
#include "implementation/jvm_ref.h"
#include "implementation/object_ref.h"
#include "jni_dep.h"

namespace jni {

// Represents a "Local" JNI reference of a class.  This object is *not* thread
// safe, but can be promoted to a GlobalObject which *is* thread safe.
template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalObject
    : public ObjectRefBuilder_t<class_v_, class_loader_v_, jvm_v_> {
 public:
  using ObjectRefT = ObjectRefBuilder_t<class_v_, class_loader_v_, jvm_v_>;
  using ObjectRefT::ObjectRefT;

  LocalObject(jobject object) : ObjectRefT(object) {}

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  LocalObject(LocalObject<class_v, class_loader_v, jvm_v>&& rhs)
      : ObjectRefT(rhs.Release()) {
    static_assert(
        std::string_view(class_v.name_) == std::string_view(class_v_.name_),
        "You are attempting to initialise a LocalObject from another class "
        "type");
  }

  ~LocalObject() {
    if (ObjectRefT::object_ref_) {
      JniHelper::DeleteLocalObject(ObjectRefT::object_ref_);
    }
  }
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalObject(LocalObject<class_v_, class_loader_v_, jvm_v_>&&)
    -> LocalObject<class_v_, class_loader_v_, jvm_v_>;

}  // namespace jni

#endif  // JNI_BIND_LOCAL_OBJECT_H
