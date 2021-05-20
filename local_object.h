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

#include "class.h"
#include "jvm.h"
#include "jvm_ref.h"
#include "object_ref.h"
#include "class_loader.h"
#include "jni_dep.h"
#include "jni_helper/jni_helper.h"

namespace jni {

// Represents a "Local" JNI reference of a class.  This object is *not* thread
// safe, but can be promoted to a GlobalObject which *is* thread safe.
template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalObject : public ObjectRefBuilder_t<
                        jvm_v_, class_v_, class_loader_v_,
                        LocalObject<class_v_, class_loader_v_, jvm_v_>> {
 public:
  using ObjectRefT =
      ObjectRefBuilder_t<jvm_v_, class_v_, class_loader_v_,
                         LocalObject<class_v_, class_loader_v_, jvm_v_>>;
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

 protected:
  // TODO(b/174256299): The rationale below no longer makes sense. Preventing
  // this being used cross thread or being moved is impossible. Callers will
  // need to read the documentation to know globals are needed for threading.
  //
  // In order to prevent accidental moving of a LocalObject outside of a JNI
  // function's scope, LocalObject cannot be moved.  If this were permitted, an
  // object could be moved by accident (say into a datastructure), stored, and
  // then re-used on a subsequent JNI invocation (which is illegal).
  //
  // That said, if you truly want to move a LocalObject, you may, but be aware
  // that you may introduce unseen failures and should probably just use a
  // GlobalObject.  E.g. If you accidentally moved this into a datastructure
  // with multi-threading, you would guarantee undefined behaviour.
  //
  // LocalObject(LocalObject&& rhs) = default;

  template <const auto&, const auto&, const auto&, typename, typename, size_t,
            typename...>
  friend class ImbueConstructor;

  template <const auto&, const auto&, const auto&, typename>
  friend class ObjectRef;

  template <const auto&, const auto&, typename>
  friend class ClassLoaderRef;

  // Invoked through CRTP on ctor.
  template <typename... Ts>
  constexpr jobject ClassSpecificNewObjectRef(jmethodID class_ctor_method_ref,
                                              Ts&&... ts) {
    return JniHelper::NewLocalObject(ObjectRefT::GetJClass(),
                                     class_ctor_method_ref,
                                     std::forward<Ts>(ts)...);
  }

  // Invoked through CRTP on dtor.
  constexpr void ClassSpecificDeleteObjectRef(jobject object_ref) {
    JniHelper::DeleteLocalObject(object_ref);
  }
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalObject(LocalObject<class_v_, class_loader_v_, jvm_v_>&&)
    -> LocalObject<class_v_, class_loader_v_, jvm_v_>;

}  // namespace jni

#endif  // JNI_BIND_LOCAL_OBJECT_H
