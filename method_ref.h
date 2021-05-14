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

#include "class_loader.h"
#include "class_ref.h"
#include "jni_type_proxy.h"
#include "method.h"
#include "ref_base.h"
#include "signature.h"
#include "string.h"
#include "class_loader.h"
#include "jni_dep.h"
#include "jni_helper/jni_env.h"
#include "jni_helper/jni_helper.h"
#include "jni_helper/jni_method_invoke.h"
#include "metaprogramming/double_locked_value.h"
#include "metaprogramming/invocable_map.h"
#include "metaprogramming/optional_wrap.h"
#include "params.h"
#include "return.h"

namespace jni {

// See JvmRef::~JvmRef.
static inline auto& GetDefaultLoadedMethodList() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<jmethodID>*>{};
  return *ret_val;
}

template <const auto& class_loader_v_, const auto& class_v_, size_t I,
          typename... Params_>
class MethodRef {};

// Static Interface for manipulating the Ith method on |class_v_| for
// |class_loader_v_|.
//
// The partial specialisation is necessary in order to have Params as a variadic
// pack so that operator() can be expressed directly.  You could alternatively
// do this with a std::enable_if, but this would only prevent incorrect
// signatures being used, whereas for the below a smart enough compiler can
// actually infer the correct params.
//
// Note, this class performs no cleanup on destruction.  jFieldIDs are static
// throughout the duration of a JVM's life, see jni::Jvm.
template <const auto& class_loader_v_, const auto& class_v_, size_t I,
          typename... Params_>
class MethodRef<class_loader_v_, class_v_, I, std::tuple<Params_...>> {
 public:
  MethodRef() = delete;

  static constexpr auto& GetMethod() { return std::get<I>(class_v_.methods_); }

  // TODO(b/174272629): Remove when new type proxy system is used.
  using Method = std::decay_t<decltype(GetMethod())>;
  using ReturnT = ReturnT_t<Method>;
  using ReturnRaw = ReturnRaw_t<ReturnT>;

 private:
  static const char* GetMethodSignature() {
    static std::string method_signature = GetMethod().Signature();
    return method_signature.c_str();
  }

 public:
  // This method is thread safe.
  static jmethodID GetMethodID(jclass clazz) {
    static jni::metaprogramming::DoubleLockedValue<jmethodID> return_value;

    return return_value.LoadAndMaybeInit([=]() {
      if constexpr (class_loader_v_ == kDefaultClassLoader) {
        GetDefaultLoadedMethodList().push_back(&return_value);
      }

      return jni::JniHelper::GetMethodID(clazz, GetMethod().name_,
                                         GetMethodSignature());
    });
  }

 public:
  static auto Invoke(jclass clazz, jobject object, Params_&&... params) {
    if constexpr (std::is_base_of_v<Object, ReturnRaw>) {
      return RefBaseTag<jobject>{JniMethodInvoke<jobject>::Invoke(
          object, GetMethodID(clazz),
          JniTypeProxy<std::decay_t<Params_>>::Proxy(
              std::forward<Params_>(params))...)};
    } else if constexpr (std::is_same_v<jstring, ReturnRaw>) {
      return RefBaseTag<jstring>{
          static_cast<jstring>(JniMethodInvoke<jobject>::Invoke(
              object, GetMethodID(clazz),
              JniTypeProxy<Params_>::Proxy(std::forward<Params_>(params))...))};
    } else {
      return JniMethodInvoke<ReturnRaw>::Invoke(
          object, GetMethodID(clazz),
          JniTypeProxy<std::decay_t<Params_>>::Proxy(
              std::forward<Params_>(params))...);
    }
  }
};

//==============================================================================
template <const auto& class_loader_v, const auto& class_v, size_t I>
struct MethodParamsExtractor {
  using ParamsRaw = typename std::decay_t<
      decltype(std::get<I>(class_v.methods_).Params())>::ParamsRawTup;

  // TODO:  This should tag the loader.
  using ParamsProxied = JniTypeProxyAsInputParamTup_tup<ParamsRaw>;

  using type = MethodRef<class_loader_v, class_v, I, ParamsProxied>;
};

template <const auto& class_loader_v, const auto& class_v, size_t I>
using MethodRefT_t =
    typename MethodParamsExtractor<class_loader_v, class_v, I>::type;

template <typename Methods>
struct ParamsFromMethodsTup_Tup_Helper {};

template <typename... Methods>
struct ParamsFromMethodsTup_Tup_Helper<std::tuple<Methods...>> {
  using ParamsTupUnproxied = std::tuple<ParamsRawTup_t<ParamsT_t<Methods>>...>;
  using type = JniTypeProxyMethodSetTup_Tup<ParamsTupUnproxied>;
};

template <typename MethodsTup>
using ParamsFromMethodsTup_Tup =
    typename ParamsFromMethodsTup_Tup_Helper<MethodsTup>::type;

//==============================================================================
template <const auto& class_loader_v, const auto& class_v_,
          typename IndexSequence>
struct MethodRefTupHelper {};

template <const auto& class_loader_v, const auto& class_v_, size_t... indices>
struct MethodRefTupHelper<class_loader_v, class_v_,
                          std::index_sequence<indices...>> {
  using type = std::tuple<MethodRefT_t<class_loader_v, class_v_, indices>...>;
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
