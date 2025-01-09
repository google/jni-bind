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

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>

#include "implementation/class_ref.h"
#include "implementation/default_class_loader.h"
#include "implementation/field_ref.h"
#include "implementation/id.h"
#include "implementation/id_type.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/method_selection.h"
#include "implementation/no_idx.h"
#include "jni_dep.h"
#include "metaprogramming/invocable_map.h"
#include "metaprogramming/invocable_map_20.h"
#include "metaprogramming/queryable_map.h"
#include "metaprogramming/queryable_map_20.h"
#include "metaprogramming/string_literal.h"

namespace jni {

template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
struct StaticRefHelper {
  using _JniT = JniT<jobject, class_v_, class_loader_v_, jvm_v_>;

  // C++17 augmentations.
  using MethodMapT = metaprogramming::InvocableMap<
      CrtpBase_, _JniT::static_v, typename _JniT::StaticT,
      decltype(&_JniT::StaticT::methods_), &_JniT::StaticT::methods_>;
  using FieldMapT =
      metaprogramming::QueryableMap_t<CrtpBase_, _JniT::static_v,

                                      decltype(&_JniT::StaticT::fields_),
                                      &_JniT::StaticT::fields_>;

  // C++ 20 augmentations.
  using MethodMap20T = metaprogramming::InvocableMap20<
      CrtpBase_, _JniT::static_v,
      StaticRefHelper<CrtpBase_, class_v_, class_loader_v_, jvm_v_>,
      decltype(&_JniT::StaticT::methods_), &_JniT::StaticT::methods_>;

  using FieldMap20T = metaprogramming::QueryableMap20<
      CrtpBase_, _JniT::static_v,
      StaticRefHelper<CrtpBase_, class_v_, class_loader_v_, jvm_v_>,
      decltype(&_JniT::StaticT::fields_), &_JniT::StaticT::fields_>;
};

// C++17 augmentations.
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

// C++20 augmentations.
template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
using StaticRefHelperMethodMap20_t =
    typename StaticRefHelper<CrtpBase_, class_v_, class_loader_v_,
                             jvm_v_>::MethodMap20T;
template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
using StaticRefHelperFieldMap20_t =
    typename StaticRefHelper<CrtpBase_, class_v_, class_loader_v_,
                             jvm_v_>::FieldMap20T;

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
struct StaticRef
    : public
      // C++17 augmentations.
      StaticRefHelperMethodMap_t<StaticRef<class_v_, class_loader_v_, jvm_v_>,
                                 class_v_, class_loader_v_, jvm_v_>,
      StaticRefHelperFieldMap_t<StaticRef<class_v_, class_loader_v_, jvm_v_>,
                                class_v_, class_loader_v_, jvm_v_>,
      // C++ 20 augmentations.
      StaticRefHelperMethodMap20_t<StaticRef<class_v_, class_loader_v_, jvm_v_>,
                                   class_v_, class_loader_v_, jvm_v_>,
      StaticRefHelperFieldMap20_t<StaticRef<class_v_, class_loader_v_, jvm_v_>,
                                  class_v_, class_loader_v_, jvm_v_> {
  using _JniT = JniT<jobject, class_v_, class_loader_v_, jvm_v_>;

  jclass GetJClass() const {
    return ClassRef_t<_JniT>::GetAndMaybeLoadClassRef(nullptr);
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Implementation: C++17 + clang
  // Supports syntax like: "obj("foo", 123, args)", "obj["foo"].Get()"
  // This syntax is less portable and may be removed in a major future release.
  ////////////////////////////////////////////////////////////////////////////////
#if __clang__
  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) const {
    using IdT = Id<_JniT, IdType::STATIC_OVERLOAD_SET, I, kNoIdx, kNoIdx, 0>;
    using MethodSelectionForArgs =
        OverloadSelector<IdT, IdType::STATIC_OVERLOAD,
                         IdType::STATIC_OVERLOAD_PARAM, Args...>;

    static_assert(MethodSelectionForArgs::kIsValidArgSet,
                  "JNI Error: Invalid argument set.");

    return MethodSelectionForArgs::_OverloadRef::Invoke(
        GetJClass(), nullptr, std::forward<Args>(args)...);
  }

  template <size_t I>
  auto QueryableMapCall(const char* key) const {
    return FieldRef<_JniT, IdType::STATIC_FIELD, I>{GetJClass(), nullptr};
  }
#endif  // __clang__

#if __cplusplus >= 202002L
  // Invoked through CRTP from InvocableMap, C++20 only.
  template <size_t I, metaprogramming::StringLiteral key_literal,
            typename... Args>
  auto InvocableMap20Call(Args&&... args) const {
    using IdT = Id<_JniT, IdType::STATIC_OVERLOAD_SET, I, kNoIdx, kNoIdx, 0>;
    using MethodSelectionForArgs =
        OverloadSelector<IdT, IdType::STATIC_OVERLOAD,
                         IdType::STATIC_OVERLOAD_PARAM, Args...>;

    static_assert(MethodSelectionForArgs::kIsValidArgSet,
                  "JNI Error: Invalid argument set.");

    return MethodSelectionForArgs::_OverloadRef::Invoke(
        GetJClass(), nullptr, std::forward<Args>(args)...);
  }

  // Invoked through CRTP from QueryableMap20, C++20 only.
  template <size_t I, metaprogramming::StringLiteral key_literal>
  auto QueryableMap20Call() const {
    return FieldRef<_JniT, IdType::STATIC_FIELD, I>{GetJClass(), nullptr};
  }
#endif  // __cplusplus >= 202002L
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_STATIC_REF_H_
