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

#include "implementation/array.h"
#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/class_ref.h"
#include "implementation/constructor.h"
#include "implementation/default_class_loader.h"
#include "implementation/field_ref.h"
#include "implementation/jni_helper/jni_env.h"
#include "implementation/jni_type.h"
#include "implementation/method_selection.h"
#include "implementation/proxy.h"
#include "implementation/ref_base.h"
#include "jni_dep.h"
#include "metaprogramming/invocable_map.h"
#include "metaprogramming/optional_wrap.h"
#include "metaprogramming/queryable_map.h"
#include "metaprogramming/string_contains.h"
#include "metaprogramming/tuple_manipulation.h"

namespace jni {

// Represents a runtime instance of a JNI Object.  Instead of using this class
// directly, instead the more specialised types such as LocalObject,
// GlobalObject, etc.
//
// To call methods on the object, use the  operator(), to access fields, use
// operator[].
template <typename JniT>
class ObjectRef
    : public metaprogramming::InvocableMap<
          ObjectRef<JniT>, JniT::stripped_class_v, typename JniT::ClassT,
          &JniT::ClassT::methods_>,
      public metaprogramming::QueryableMap_t<
          ObjectRef<JniT>, JniT::stripped_class_v, &JniT::ClassT::fields_>,
      public RefBase<JniT> {
 protected:
  static_assert(
      JniT::class_loader_v
          .template SupportedDirectlyOrIndirectly<JniT::class_v>(),
      "This class is not directly or indirectly supported by this loader.");
  static_assert(!metaprogramming::StringContains_v<JniT::kName, '.'>,
                "Use '/', not '.' in class names (for maximum) portability.");

  using RefBase = RefBase<JniT>;

  ObjectRef() = delete;
  ObjectRef& operator=(const ObjectRef& rhs) = delete;

  jclass GetJClass() const {
    // jobject != jstring but both derive from this class.
    // Args are passed to recover class ref from class loader, and nullptr is
    // always safe. `GetAndMaybeLoadClassRef` requires jobject, so using
    // `RefBase::object_ref_` below obiates ubsan failures.
    if constexpr (std::is_same_v<typename JniT::SpanType, jobject>) {
      return ClassRef_t<JniT>::GetAndMaybeLoadClassRef(RefBase::object_ref_);
    } else {
      return ClassRef_t<JniT>::GetAndMaybeLoadClassRef(nullptr);
    }
  }

 public:
  explicit ObjectRef(RefBaseTag<typename JniT::StorageType>&& rhs)
      : RefBase(std::move(rhs)) {}

  // Invoked through CRTP from InvocableMap.
  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) const {
    using IdT = Id<JniT, IdType::OVERLOAD_SET, I>;
    using MethodSelectionForArgs =
        OverloadSelector<IdT, IdType::OVERLOAD, IdType::OVERLOAD_PARAM,
                         Args...>;

    static_assert(MethodSelectionForArgs::kIsValidArgSet,
                  "JNI Error: Invalid argument set.");

    return MethodSelectionForArgs::OverloadRef::Invoke(
        GetJClass(), RefBase::object_ref_, std::forward<Args>(args)...);
  }

  // Invoked through CRTP from QueryableMap.
  template <size_t I>
  auto QueryableMapCall(const char* key) const {
    return FieldRef<JniT, IdType::FIELD, I>{GetJClass(), RefBase::object_ref_};
  }
};

// Imbues constructors for ObjectRefs and handles calling the correct
// intermediate constructors.  Access to this class is constrainted for non
// default classloaders (see |ValidatorProxy|).
template <typename JniT>
class ConstructorValidator : public ObjectRef<JniT> {
 public:
  using Base = ObjectRef<JniT>;
  using Base::Base;

  // Objects can still be wrapped.  This could happen if a classloaded object
  // is built in Java and then passed through to JNI.
  ConstructorValidator(typename JniT::StorageType obj) : Base(obj) {}

  template <LifecycleType, const auto& jvm_v, const auto& class_loader_v>
  friend class ClassLoaderRef;

  static constexpr std::size_t kNumConstructors =
      std::tuple_size_v<decltype(JniT::class_v.constructors_)>;

  template <typename... Args>
  struct Helper {
    using IdT = Id<JniT, IdType::OVERLOAD_SET, kNoIdx>;

    // 0 is (always) used to represent the constructor.
    using type = OverloadSelector<IdT, IdType::OVERLOAD, IdType::OVERLOAD_PARAM,
                                  Args...>;
  };

  template <typename... Args>
  using Permutation_t = typename Helper<Args...>::type;

  template <typename... Args,
            typename std::enable_if<(sizeof...(Args) != 0 && JniT::kRank == 0),
                                    int>::type = 0>
  ConstructorValidator(Args&&... args)
      : Base(static_cast<typename JniT::StorageType>(
            Permutation_t<Args...>::OverloadRef::Invoke(
                Base::GetJClass(), Base::object_ref_,
                std::forward<Args>(args)...)
                .Release())) {
    static_assert(Permutation_t<Args...>::kIsValidArgSet,
                  "You have passed invalid arguments to construct this type.");
  }

  ConstructorValidator()
      : Base(Permutation_t<>::OverloadRef::Invoke(Base::GetJClass(),
                                                  Base::object_ref_)
                 .Release()) {
  }
};

// Forward declaration for constructor validator (ctor augmentations).
template <typename JniT>
struct ValidatorProxy : public ConstructorValidator<JniT> {
  using Base = ConstructorValidator<JniT>;
  using Base::Base;
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
using ObjectRefBuilder_t =
    ValidatorProxy<JniT<jobject, class_v_, class_loader_v_, jvm_v_>>;

}  // namespace jni

#endif  // JNI_BIND_OBJECT_REF_H
