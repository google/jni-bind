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
#include "implementation/jni_helper/jni_helper.h"
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
    : public ObjectRefBuilder_t<class_v_, class_loader_v_, jvm_v_> {
 public:
  template <const auto& jvm_v, const auto& class_loader_v>
  friend class ClassLoaderRef;

  using Base = ObjectRefBuilder_t<class_v_, class_loader_v_, jvm_v_>;
  using Base::Base;

  // Constructs a new global object using the local object's default
  // constructor.
  explicit GlobalObject()
      : GlobalObject(JniHelper::PromoteLocalToGlobalObject(
            LocalObject<class_v_, class_loader_v_, jvm_v_>{}.Release())) {}

  // "Copy" constructor, creates new reference (standard).
  explicit GlobalObject(CreateCopy, jobject obj)
      : Base(JniHelper::NewGlobalRef(obj)) {}

  // "Promote" constructor, creates a new global, frees passed arg (standard).
  explicit GlobalObject(PromoteToGlobal, jobject obj)
      : Base(JniHelper::PromoteLocalToGlobalObject(obj)) {}

  // "Adopts" a global by wrapping a jstring (non-standard).
  explicit GlobalObject(AdoptGlobal, jobject obj) : Base(obj) {}

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject(LocalObject<class_v, class_loader_v, jvm_v>&& local_object)
      : Base(JniHelper::PromoteLocalToGlobalObject(jobject{local_object})) {
    static_assert(
        std::string_view(class_v.name_) == std::string_view(class_v_.name_),
        "You are attempting to initialise a LocalObject from another class "
        "type");
  }

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject(GlobalObject<class_v, class_loader_v, jvm_v>&& rhs)
      : Base(rhs.Release()) {
    static_assert(
        std::string_view(class_v.name_) == std::string_view(class_v_.name_),
        "You are attempting to initialise a GlobalObject from another class "
        "type");
  }

  // Constructs a new object using arg based ctor.
  template <typename... Ts>
  explicit GlobalObject(Ts&&... ts)
      : GlobalObject(JniHelper::PromoteLocalToGlobalObject(
            LocalObject<class_v_, class_loader_v_, jvm_v_>{
                std::forward<Ts>(ts)...}
                .Release())) {}


  GlobalObject(const GlobalObject&) = delete;
  GlobalObject(GlobalObject&& rhs) = default;

  ~GlobalObject() {
    if (Base::object_ref_) {
      JniHelper::DeleteGlobalObject(Base::object_ref_);
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
