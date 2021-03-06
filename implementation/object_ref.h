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

#ifndef JNI_BIND_OBJECT_REF_H
#define JNI_BIND_OBJECT_REF_H

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "class.h"
#include "class_ref.h"
#include "constructor.h"
#include "field_ref.h"
#include "method_ref.h"
#include "proxy.h"
#include "ref_base.h"
#include "implementation/class_loader.h"
#include "implementation/default_class_loader.h"
#include "implementation/jni_helper/jni_env.h"
#include "implementation/jvm_ref.h"
#include "implementation/method_selection.h"
#include "jni_dep.h"
#include "metaprogramming/invocable_map.h"
#include "metaprogramming/optional_wrap.h"
#include "metaprogramming/queryable_map.h"
#include "metaprogramming/tuple_manipulation.h"

namespace jni {

// Represents a runtime instance of a JNI Object.  Instead of using this class
// directly, instead the more specialised types such as LocalObject,
// GlobalObject, etc.
//
// To call methods on the object, use the  operator(), to access fields, use
// operator[].
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_>
class ObjectRef
    : public MethodMap_t<class_loader_v_, class_v_,
                         ObjectRef<jvm_v_, class_v_, class_loader_v_>>,
      public metaprogramming::QueryableMap_t<
          ObjectRef<jvm_v_, class_v_, class_loader_v_>, class_v_,
          &std::decay_t<decltype(class_v_)>::fields_>,
      public RefBase<jobject, class_v_, class_loader_v_> {
 protected:
  static_assert(
      class_loader_v_.template SupportedDirectlyOrIndirectly<class_v_>(),
      "This class is not directly or indirectly supported by this loader.");
  using RefBase = RefBase<jobject, class_v_, class_loader_v_>;

  ObjectRef() = delete;
  explicit ObjectRef(ObjectRef&& rhs) = default;
  ObjectRef(const ObjectRef& rhs) = delete;
  ObjectRef& operator=(const ObjectRef& rhs) = delete;

  jclass GetJClass() const {
    return ClassRef_t<jvm_v_, class_loader_v_,
                      class_v_>::GetAndMaybeLoadClassRef(*RefBase::object_ref_);
  }

 public:
  explicit ObjectRef(RefBaseTag<jobject>&& rhs) : RefBase(std::move(rhs)) {}

  // Invoked through CRTP from InvocableMap.
  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) const {
    using MethodSelectionForArgs =
        MethodSelectionForArgs_t<class_loader_v_, class_v_, false, I, Args...>;

    static_assert(MethodSelectionForArgs::kIsValidArgSet,
                  "JNI Error: Invalid argument set.");

    return MethodSelectionForArgs::OverloadRef::Invoke(
        GetJClass(), *RefBase::object_ref_, std::forward<Args>(args)...);
  }

  // Invoked through CRTP from QueryableMap.
  template <size_t I>
  auto QueryableMapCall(const char* key) const {
    return FieldRef<class_loader_v_, class_v_, I>{GetJClass(),
                                                  *RefBase::object_ref_};
  }
};

// Imbues constructors for ObjectRefs and handles calling the correct
// intermediate constructors.  Access to this class is constrainted for non
// default classloaders (see |ValidatorProxy|).
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_>
class ConstructorValidator
    : public ObjectRef<jvm_v_, class_v_, class_loader_v_> {
 public:
  using Base = ObjectRef<jvm_v_, class_v_, class_loader_v_>;
  using Base::Base;

  // Objects can still be wrapped.  This could happen if a classloaded object
  // is built in Java and then passed through to JNI.
  ConstructorValidator(jobject obj) : Base(obj) {}

  template <const auto& jvm_v, const auto& class_loader_v>
  friend class ClassLoaderRef;

  static constexpr std::size_t kNumConstructors =
      std::tuple_size_v<decltype(class_v_.constructors_)>;

  template <typename... Args>
  struct Helper {
    // 0 is (always) used to represent the constructor.
    using type =
        MethodSelectionForArgs_t<class_loader_v_, class_v_, true, 0, Args...>;
  };

  template <typename... Args>
  using Permutation_t = typename Helper<Args...>::type;

  template <typename... Args,
            typename std::enable_if<sizeof...(Args) != 0, int>::type = 0>
  ConstructorValidator(Args&&... args)
      : Base(Permutation_t<Args...>::OverloadRef::Invoke(
                 Base::GetJClass(), *Base::object_ref_,
                 std::forward<Args>(args)...)
                 .Release()) {
    static_assert(Permutation_t<Args...>::kIsValidArgSet,
                  "You have passed invalid arguments to construct this type.");
  }

  ConstructorValidator()
      : Base(Permutation_t<>::OverloadRef::Invoke(Base::GetJClass(),
                                                  *Base::object_ref_)
                 .Release()) {
    static_assert(
        kNumConstructors != 0,
        "You are attempting to construct an object which has no Constructor "
        "defined.  If you intended to construct this object with no arguments, "
        "ensure you define a jni::Constructor with no arguments.");
  }
};

template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_>
struct ValidatorProxy
    : public ConstructorValidator<jvm_v_, class_v_, class_loader_v_> {
  ValidatorProxy(jobject obj) : Base(obj) {}

 protected:
  using Base = ConstructorValidator<jvm_v_, class_v_, class_loader_v_>;
  using Base::Base;
};

template <const auto& jvm_v_, const auto& class_v_>
struct ValidatorProxy<jvm_v_, kDefaultClassLoader, class_v_>
    : public ConstructorValidator<jvm_v_, class_v_, kDefaultClassLoader> {
  using Base = ConstructorValidator<jvm_v_, class_v_, kDefaultClassLoader>;
  using Base::Base;
};

template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_>
using ObjectRefBuilder_t = ValidatorProxy<jvm_v_, class_v_, class_loader_v_>;

}  // namespace jni

#endif  // JNI_BIND_OBJECT_REF_H
