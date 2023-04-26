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

#ifndef JNI_BIND_LOCAL_CLASS_LOADER_H_
#define JNI_BIND_LOCAL_CLASS_LOADER_H_

#include "implementation/class_loader.h"
#include "implementation/class_loader_ref.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jvm.h"
#include "implementation/jvm_ref.h"
#include "jni_dep.h"

namespace jni {

template <const auto& class_loader_v_, const auto& jvm_v_ = kDefaultJvm>
class LocalClassLoader : public ClassLoaderRef<jvm_v_, class_loader_v_> {
 public:
  using Base = ClassLoaderRef<jvm_v_, class_loader_v_>;

  LocalClassLoader(jobject class_loader)
      : ClassLoaderRef<jvm_v_, class_loader_v_>(class_loader) {}

  ~LocalClassLoader() {
    if (Base::object_ref_) {
      LifecycleHelper<jobject, LifecycleType::LOCAL>::Delete(Base::object_ref_);
    }
  }

 private:
  template <typename>
  friend class ObjectRef;
};

}  // namespace jni

#endif  // JNI_BIND_LOCAL_CLASS_LOADER_H_
