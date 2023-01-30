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

#ifndef JNI_BIND_METHOD_REF_H
#define JNI_BIND_METHOD_REF_H

#include <mutex>
#include <tuple>
#include <utility>

#include "implementation/class_loader.h"
#include "implementation/class_ref.h"
#include "implementation/id_type.h"
#include "implementation/jni_helper/invoke.h"
#include "implementation/jni_helper/invoke_static.h"
#include "implementation/jni_helper/jni_env.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jni_type.h"
#include "implementation/method.h"
#include "implementation/params.h"
#include "implementation/proxy.h"
#include "implementation/proxy_definitions.h"
#include "implementation/proxy_definitions_array.h"
#include "implementation/proxy_definitions_string.h"
#include "implementation/ref_base.h"
#include "implementation/return.h"
#include "implementation/signature.h"
#include "implementation/void.h"
#include "jni_dep.h"
#include "metaprogramming/double_locked_value.h"
#include "metaprogramming/invocable_map.h"
#include "metaprogramming/optional_wrap.h"
#include "metaprogramming/string_concatenate.h"

namespace jni {

// See JvmRef::~JvmRef.
static inline auto& GetDefaultLoadedMethodList() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<jmethodID>*>{};
  return *ret_val;
}

template <typename IdT_, IdType kReturnIDType>
struct OverloadRef {
  using IdT = IdT_;
  using ReturnIdT = typename IdT::template ChangeIdType<kReturnIDType>;
  using ReturnProxied =
      Return_t<typename ReturnIdT::MaterializeCDeclT, ReturnIdT>;

  static jmethodID GetMethodID(jclass clazz) {
    static jni::metaprogramming::DoubleLockedValue<jmethodID> return_value;

    return return_value.LoadAndMaybeInit([=]() {
      if constexpr (IdT_::JniT::GetClassLoader() == kDefaultClassLoader) {
        GetDefaultLoadedMethodList().push_back(&return_value);
      }

      if constexpr (IdT::kIsStatic) {
        return jni::JniHelper::GetStaticMethodID(clazz, IdT::Name(),
                                                 Signature_v<IdT>.data());

      } else {
        return jni::JniHelper::GetMethodID(clazz, IdT::Name(),
                                           Signature_v<IdT>.data());
      }
    });
  }

  template <typename... Params>
  static ReturnProxied Invoke(jclass clazz, jobject object,
                              Params&&... params) {
    constexpr std::size_t kRank = ReturnIdT::kRank;
    constexpr bool kStatic = ReturnIdT::kIsStatic;
    const jmethodID mthd = OverloadRef::GetMethodID(clazz);

    if constexpr (std::is_same_v<ReturnProxied, void>) {
      return InvokeHelper<void, kRank, kStatic>::Invoke(
          object, clazz, mthd,
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...);
    } else if constexpr (IdT::kIsConstructor) {
      return ReturnProxied{JniHelper::NewLocalObject(
          clazz, mthd,
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...)};
    } else {
      return static_cast<ReturnProxied>(
          InvokeHelper<typename ReturnIdT::CDecl, kRank, kStatic>::Invoke(
              object, clazz, mthd,
              Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...));
    }
  }
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_REF_H
