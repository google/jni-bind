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

#include "class.h"
#include "local_object.h"
#include "object_ref.h"
#include "jni_dep.h"
#include "jni_helper/jni_helper.h"

namespace jni {

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class GlobalObject : public ObjectRefBuilder_t<
                         jvm_v_, class_v_, class_loader_v_,
                         GlobalObject<class_v_, class_loader_v_, jvm_v_>> {
 public:
  using ObjectRefT =
      ObjectRefBuilder_t<jvm_v_, class_v_, class_loader_v_,
                         GlobalObject<class_v_, class_loader_v_, jvm_v_>>;

  using ObjectRefT::ObjectRefT;

  // TODO(b/174256299):  This API is impossible to reason about.  For both local
  // and global objects, the object is blindly wrapped and released.  If a
  // caller wraps a jobject input into a function, wrapping the jobject will
  // have no effect (irrespective of whether the reference is released, as for
  // no apparent reason input parameters can be released infinite times,
  // although locally created objects cannot).
  //
  // If a jobject is wrapped with a global however, a commeasurate number of New
  // and Release calls *must* be made.  The result is that the following
  // sequence is valid for Local, but not for Global:
  //
  // void SomeJniFunct(jobject) {[Local|Global]Object obj {jobject};}
  //
  // For LocalObject this will Release with no effect, but for Global it will
  // attempt to delete the jobject as it falls out of scope which will fail as
  // the jobject is not a GlobalRef.
  //
  // This should probably always wrap the incoming object by creating a
  // NewGlobalRef as if the incoming object was local it won't matter if it
  // falls out of scope, and if it isn't the caller must have intentionally
  // created a global reference and thus should delete it.
  GlobalObject(jobject object) : ObjectRefT(object) {}

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject(LocalObject<class_v, class_loader_v, jvm_v>&& local_object)
      : ObjectRefT(
            JniHelper::PromoteLocalToGlobalObject(local_object.Release())) {
    static_assert(
        std::string_view(class_v.name_) == std::string_view(class_v_.name_),
        "You are attempting to initialise a LocalObject from another class "
        "type");
  }

  // TODO(b/174256299, b/174272629): Remove this and force callers to explicitly
  // declare even void constructors.
  explicit GlobalObject()
      : GlobalObject(JniHelper::PromoteLocalToGlobalObject(
            LocalObject<class_v_, class_loader_v_, jvm_v_>{}.Release())) {}

  GlobalObject(const GlobalObject&) = delete;
  GlobalObject(GlobalObject&& rhs) = default;

 private:
  template <const auto&, const auto&, const auto&, typename, typename, size_t,
            typename...>
  friend class ImbueConstructor;

  template <const auto&, const auto&, const auto&, typename>
  friend class ObjectRef;

  // Invoked through CRTP on ctor.
  template <typename... Ts>
  constexpr jobject ClassSpecificNewObjectRef(jmethodID class_ctor_method_ref,
                                              Ts&&... ts) {
    using ClassRefT = ClassRef_t<jvm_v_, class_loader_v_, class_v_>;

    return JniHelper::PromoteLocalToGlobalObject(JniHelper::NewLocalObject(
        ClassRefT::GetAndMaybeLoadClassRef(), class_ctor_method_ref,
        std::forward<Ts>(ts)...));
  }

  // Invoked through CRTP on dtor.
  constexpr void ClassSpecificDeleteObjectRef(jobject object_ref) {
    JniHelper::DeleteGlobalObject(object_ref);
  }
};

template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
GlobalObject(LocalObject<class_v, class_loader_v, jvm_v>&&)
    -> GlobalObject<class_v, class_loader_v, jvm_v>;

}  // namespace jni

#endif  // JNI_BIND_GLOBAL_OBJECT_H_
