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

#ifndef JNI_BIND_IMPLEMENTATION_PROXY_CONVENIENCE_ALIASES_H_
#define JNI_BIND_IMPLEMENTATION_PROXY_CONVENIENCE_ALIASES_H_

#include <type_traits>

namespace jni {

template <typename TUndecayed>
struct ProxyHelper;

// See jni::Proxy.
template <typename T>
using Proxy_t = typename ProxyHelper<T>::Proxy_t;

template <typename T>
using Index_t = typename ProxyHelper<T>::Index;

template <typename T>
using CDecl_t = typename ProxyHelper<T>::CDecl;

template <typename T, typename OverloadSelection>
using Return_t =
    typename ProxyHelper<T>::template AsReturn_t<OverloadSelection>;

template <typename T, typename ParamSelection>
using Arg_t = typename ProxyHelper<T>::template AsArg_t<ParamSelection>;

template <typename T>
using AsDecl_t = typename ProxyHelper<T>::AsDecl_t;

// Instead of directly searching for the type, convertible types are sought.
// E.g. A string like "Foo" the type will be const char[4] not const char*.
template <typename Query>
struct IsConvertibleKey {
  template <typename T>
  static constexpr bool value =
      std::is_same_v<Query, std::decay_t<T>> ||
      std::is_base_of_v<std::decay_t<T>, std::decay_t<Query>> ||
      std::is_base_of_v<std::decay_t<Query>, std::decay_t<T>>;
};

template <typename Query, typename T>
static constexpr bool IsConvertibleKey_v =
    IsConvertibleKey<Query>::template value<T>;

}  // namespace jni
#endif  // JNI_BIND_IMPLEMENTATION_PROXY_CONVENIENCE_ALIASES_H_
