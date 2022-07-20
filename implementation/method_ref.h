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
#include "implementation/jni_helper/jni_env.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jni_helper/jni_method_invoke.h"
#include "implementation/method.h"
#include "implementation/params.h"
#include "implementation/proxy.h"
#include "implementation/ref_base.h"
#include "implementation/return.h"
#include "implementation/string.h"
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

template <typename Method, typename Overload>
struct OverloadRef {
  using ReturnProxied = typename Overload::AsReturn;

  static constexpr std::string_view GetMethodSignature() {
    return Overload::GetOverloadSignature();
  }

  static jmethodID GetMethodID(jclass clazz) {
    static jni::metaprogramming::DoubleLockedValue<jmethodID> return_value;

    return return_value.LoadAndMaybeInit([=]() {
      if constexpr (Method::GetClassLoader() == kDefaultClassLoader) {
        GetDefaultLoadedMethodList().push_back(&return_value);
      }

      return jni::JniHelper::GetMethodID(clazz, Method::Name(),
                                         GetMethodSignature().data());

    });
  }

  template <typename... Params>
  static ReturnProxied Invoke(jclass clazz, jobject object,
                              Params&&... params) {
    if constexpr (std::is_same_v<ReturnProxied, void>) {
      JniMethodInvoke<void, 0>::Invoke(
          object, OverloadRef::GetMethodID(clazz),
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...);
    } else if constexpr (Method::kIsConstructor) {
      return {JniHelper::NewLocalObject(
          clazz, OverloadRef::GetMethodID(clazz),
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...)};
    } else {
      static constexpr bool is_array =
          kIsArrayType<decltype(Overload::GetReturn().raw_)>;

      return {JniMethodInvoke < typename Overload::CDecl,
              is_array ? 2
                       : 1 > ::Invoke(object, OverloadRef::GetMethodID(clazz),
                                      Proxy_t<Params>::ProxyAsArg(
                                          std::forward<Params>(params))...)};
    }
  }
};

//==============================================================================
// Helper class for ObjectRef to inherit from.
// Inheriting from MethodMapHelper::type exposes an operator() which keys on
// method names.
template <const auto& class_loader_v, const auto& class_v_, typename CrtpBase_>
struct MethodMapHelper {
  using MethodTup = std::decay_t<decltype(class_v_.methods_)>;
  using ClassTDecayed = std::decay_t<decltype(class_v_)>;

  using type = metaprogramming::InvocableMap<CrtpBase_, class_v_, ClassTDecayed,
                                             &ClassTDecayed::methods_>;
};

template <const auto& class_loader_v, const auto& class_v_, typename CrtpBase_>
using MethodMap_t =
    typename MethodMapHelper<class_loader_v, class_v_, CrtpBase_>::type;

}  // namespace jni

#endif  // JNI_BIND_METHOD_REF_H
