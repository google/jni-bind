/*
 * Copyright 2022 Google LLC
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
#ifndef JNI_BIND_IMPLEMENTATION_STATIC_REF_H_
#define JNI_BIND_IMPLEMENTATION_STATIC_REF_H_

#include "implementation/default_class_loader.h"
#include "implementation/field_ref.h"
#include "implementation/id.h"
#include "implementation/id_type.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/method_selection.h"
#include "metaprogramming/invocable_map.h"
#include "metaprogramming/queryable_map.h"

namespace jni {

template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
struct StaticRefHelper {
  using JniT = JniT<jobject, class_v_, class_loader_v_, jvm_v_>;

  using MethodMapT = metaprogramming::InvocableMap<CrtpBase_, JniT::static_v,
                                                   typename JniT::StaticT,
                                                   &JniT::StaticT::methods_>;
  using FieldMapT = metaprogramming::QueryableMap_t<CrtpBase_, JniT::static_v,
                                                    &JniT::StaticT::fields_>;
};

template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
using StaticRefHelperMethodMap_t =
    typename StaticRefHelper<CrtpBase_, class_v_, class_loader_v_,
                             jvm_v_>::MethodMapT;

template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
using StaticRefHelperFieldMap_t =
    typename StaticRefHelper<CrtpBase_, class_v_, class_loader_v_,
                             jvm_v_>::FieldMapT;

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
struct StaticRef
    : public StaticRefHelperMethodMap_t<
          StaticRef<class_v_, class_loader_v_, jvm_v_>, class_v_,
          class_loader_v_, jvm_v_>,
      StaticRefHelperFieldMap_t<StaticRef<class_v_, class_loader_v_, jvm_v_>,
                                class_v_, class_loader_v_, jvm_v_> {
  using JniT = JniT<jobject, class_v_, class_loader_v_, jvm_v_>;

  jclass GetJClass() const {
    return ClassRef_t<JniT>::GetAndMaybeLoadClassRef(nullptr);
  }

  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) const {
    using IdT = Id<JniT, IdType::STATIC_OVERLOAD_SET, I>;
    using MethodSelectionForArgs =
        OverloadSelector<IdT, IdType::STATIC_OVERLOAD,
                         IdType::STATIC_OVERLOAD_PARAM, Args...>;

    static_assert(MethodSelectionForArgs::kIsValidArgSet,
                  "JNI Error: Invalid argument set.");

    return MethodSelectionForArgs::OverloadRef::Invoke(
        GetJClass(), nullptr, std::forward<Args>(args)...);
  }

  template <size_t I>
  auto QueryableMapCall(const char* key) const {
    return FieldRef<JniT, IdType::STATIC_FIELD, I>{GetJClass(), nullptr};
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_STATIC_REF_H_
