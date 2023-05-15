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

#include "implementation/class.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/local_object.h"
#include "implementation/object_ref.h"
#include "implementation/promotion_mechanics.h"
#include "jni_dep.h"

namespace jni {

template <const auto& jvm_v_, const auto& class_loader_v_>
class ClassLoaderRef;

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class GlobalObject
    : public GlobalCtor<GlobalObject<class_v_, class_loader_v_, jvm_v_>,
                        ObjectRefBuilder_t<class_v_, class_loader_v_, jvm_v_>,
                        JniT<jobject, class_v_, class_loader_v_, jvm_v_>,

                        jobject> {
 public:
  template <const auto& jvm_v, const auto& class_loader_v>
  friend class ClassLoaderRef;

  using Base =
      GlobalCtor<GlobalObject<class_v_, class_loader_v_, jvm_v_>,
                 ObjectRefBuilder_t<class_v_, class_loader_v_, jvm_v_>,
                 JniT<jobject, class_v_, class_loader_v_, jvm_v_>, jobject>;
  using Base::Base;

  using LifecycleT = LifecycleHelper<jobject, LifecycleType::GLOBAL>;

  // Constructs a new global object using the local object's default
  // constructor.
  explicit GlobalObject()
      : GlobalObject(LifecycleT::Promote(
            LocalObject<class_v_, class_loader_v_, jvm_v_>{}.Release())) {}

  // Note: Comparison is only constrained to the class name, and not the
  // classloader or JVM (fully constraining classloaders is very complex).
  template <typename T,
            typename = std::enable_if_t<
                ::jni::metaprogramming::DeepEqual_v<GlobalObject, T>>>
  GlobalObject(T&& rhs) : Base(rhs.Release()) {}

  // Constructs a new object using arg based ctor.
  template <typename... Ts>
  explicit GlobalObject(Ts&&... ts)
      : GlobalObject(
            LifecycleT::Promote(LocalObject<class_v_, class_loader_v_, jvm_v_>{
                std::forward<Ts>(ts)...}
                                    .Release())) {}

  ~GlobalObject() {
    if (Base::object_ref_) {
      LifecycleT::Delete(Base::object_ref_);
    }
  }

 private:
  // Construction from jobject requires |PromoteToGlobal| or |AdoptGlobal|.
  explicit GlobalObject(jobject obj) : Base(obj) {}
};

template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
GlobalObject(LocalObject<class_v, class_loader_v, jvm_v>&&)
    -> GlobalObject<class_v, class_loader_v, jvm_v>;

}  // namespace jni

#endif  // JNI_BIND_GLOBAL_OBJECT_H_
