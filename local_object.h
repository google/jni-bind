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

#include "class.h"
#include "jvm.h"
#include "jvm_ref.h"
#include "object_ref.h"
#include "class_loader.h"
#include "jni_dep.h"
#include "jni_helper/jni_helper.h"

namespace jni {

struct DangerousMoveCtor;
struct NoDangerousMoveCtor;

// (b/174256299):  Moving locals (and promoting them to globals) should be
// handled in a more idiomatic and understandable way, and then the "dangerous
// move" tag can be removed.
template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm,
          typename DangerousMoveConstructorTag = NoDangerousMoveCtor>
class LocalObject;

template <typename... Params, const Class<Params...>& class_v_,
          const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject<class_v_, class_loader_v_, jvm_v_, NoDangerousMoveCtor>
    : public ObjectRefBuilder_t<
          jvm_v_, class_v_, class_loader_v_,
          LocalObject<class_v_, class_loader_v_, jvm_v_, NoDangerousMoveCtor>> {
 public:
  using ObjectRefT = ObjectRefBuilder_t<
      jvm_v_, class_v_, class_loader_v_,
      LocalObject<class_v_, class_loader_v_, jvm_v_, NoDangerousMoveCtor>>;
  using ObjectRefT::ObjectRefT;

  LocalObject(jobject object) : ObjectRefT(object) {}

 protected:
  // In order to prevent accidental moving of a LocalObject outside of a JNI
  // function's scope, LocalObject cannot be moved.  If this were permitted, an
  // object could be moved by accident (say into a datastructure), stored, and
  // then re-used on a subsequent JNI invocation (which is illegal).
  //
  // That said, if you truly want to move a LocalObject, you may, but be aware
  // that you may introduce unseen failures and should probably just use a
  // GlobalObject.  E.g. If you accidentally moved this into a datastructure
  // with multi-threading, you would guarantee undefined behaviour.
  LocalObject(LocalObject&& rhs) = default;

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

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_,
          typename DangerousMoveConstructorTag>
LocalObject(LocalObject<class_v_, class_loader_v_, jvm_v_,
                        DangerousMoveConstructorTag>&&)
    -> LocalObject<class_v_, class_loader_v_, jvm_v_,
                   DangerousMoveConstructorTag>;

// See move constructor comment above. You almost certainly do not want to use
// this unless you need bleeding edge performance combined with STL containers.
//
// For example, if you needed to pack local objects into std::array, this would
// permit that.
template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject<class_v_, class_loader_v_, jvm_v_, DangerousMoveCtor>
    : public LocalObject<class_v_, class_loader_v_, jvm_v_,
                         NoDangerousMoveCtor> {
 public:
  using LocalObjectT =
      LocalObject<class_v_, class_loader_v_, jvm_v_, NoDangerousMoveCtor>;

  explicit LocalObject() : LocalObjectT() {}
  LocalObject(jobject object) : LocalObjectT(object) {}

  // See class comment and base class move ctor comment.
  LocalObject(LocalObject&& rhs) = default;
};

}  // namespace jni

#endif  // JNI_BIND_LOCAL_OBJECT_H
