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

#ifndef JNI_BIND_OVERLOAD_REF_H
#define JNI_BIND_OVERLOAD_REF_H

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <string_view>
#include <type_traits>
#include <utility>

#include "implementation/configuration.h"
#include "implementation/id_type.h"
#include "implementation/jni_helper/invoke.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jni_helper/lifecycle.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/promotion_mechanics_tags.h"
#include "implementation/proxy_convenience_aliases.h"
#include "implementation/proxy_definitions.h"
#include "implementation/proxy_definitions_array.h"
#include "implementation/proxy_definitions_string.h"
#include "implementation/ref_base.h"
#include "implementation/ref_storage.h"
#include "implementation/signature.h"
#include "jni_dep.h"
#include "metaprogramming/double_locked_value.h"
#include "metaprogramming/string_concatenate.h"

namespace jni {

// Transforms a OverloadRef IdT into a fully qualified ID. Storage is keyed
// against these IDs to reduce excess MethodID lookups.
template <typename IdT>
struct OverloadRefUniqueId {
  static constexpr std::string_view kDash = "#";
  static constexpr std::string_view kClassQualifier{IdT::Class().name_};
  static constexpr std::string_view kOverloadName{IdT::Name()};

  // IdT::Name will be the overload name (e.g. "Foo").
  // Dashes are solely for readability in debugging.
  static constexpr std::string_view TypeName() {
    return metaprogramming::StringConcatenate_v<
        kClassQualifier, kDash, kOverloadName, kDash, Signature_v<IdT>>;
  }
};

template <typename IdT_, IdType kReturnIDType>
struct OverloadRef {
  using IdT = IdT_;

  using ReturnIdT = typename IdT::template ChangeIdType<kReturnIDType>;
  using SelfIdT = typename IdT::template ChangeIdType<IdType::CLASS>;

  using ReturnProxied = std::conditional_t<
      ReturnIdT::kIsSelf,
      Return_t<typename SelfIdT::MaterializeCDeclT, SelfIdT>,
      Return_t<typename ReturnIdT::MaterializeCDeclT, ReturnIdT>>;

  static jmethodID GetMethodID(jclass clazz) {
    static auto get_lambda =
        [clazz](metaprogramming::DoubleLockedValue<jmethodID>* storage) {
          if (kConfiguration.release_method_ids_on_teardown_) {
            DefaultRefs<jmethodID>().push_back(storage);
          }

          if constexpr (IdT::kIsStatic) {
            return jni::JniHelper::GetStaticMethodID(clazz, IdT::Name(),
                                                     Signature_v<IdT>.data());
          } else {
            return jni::JniHelper::GetMethodID(clazz, IdT::Name(),
                                               Signature_v<IdT>.data());
          }
        };

    return RefStorage<decltype(get_lambda), OverloadRefUniqueId<IdT>>::Get(
        get_lambda);
  }

  template <typename... Params>
  static auto Invoke(jclass clazz, jobject object,
                              Params&&... params) {
    constexpr std::size_t kRank = ReturnIdT::kRank;
    constexpr bool kStatic = ReturnIdT::kIsStatic;
    const jmethodID mthd = OverloadRef::GetMethodID(clazz);

    if constexpr (std::is_same_v<ReturnProxied, void>) {
      return InvokeHelper<void, kRank, kStatic>::Invoke(
          object, clazz, mthd,
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...);
    } else if constexpr (IdT::kIsConstructor) {
      if constexpr (std::is_same_v<typename IdT::_JniT::SpanType, jstring> && IdT::kRank == 0)
      {
        return LocalString{
          AdoptLocal{},
              Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...
        };
      } else
      {
        return ReturnProxied{
          AdoptLocal{},
          LifecycleHelper<jobject, LifecycleType::LOCAL>::Construct(
              clazz, mthd,
              Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...)};
      }
    } else {
      if constexpr (std::is_base_of_v<RefBaseBase, ReturnProxied>) {
        return ReturnProxied{
            AdoptLocal{},
            InvokeHelper<typename ReturnIdT::CDecl, kRank, kStatic>::Invoke(
                object, clazz, mthd,
                Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...)};
      } else {
        return static_cast<ReturnProxied>(
            InvokeHelper<typename ReturnIdT::CDecl, kRank, kStatic>::Invoke(
                object, clazz, mthd,
                Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...));
      }
    }
  }
};

}  // namespace jni

#endif  // JNI_BIND_OVERLOAD_REF_H
