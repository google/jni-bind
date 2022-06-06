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

namespace jni {

template <typename TUndecayed>
struct ProxyHelper;

// See jni::Proxy.
template <typename T>
using Proxy_t = typename ProxyHelper<T>::Proxy_t;

template <typename T>
using Index_t = typename ProxyHelper<T>::Index;

template <typename T, typename Overload = void>
using CDecl_t = typename ProxyHelper<T>::template CDecl<Overload>;

template <typename T, typename OverloadSelection>
using Return_t =
    typename ProxyHelper<T>::template AsReturn_t<OverloadSelection>;

template <typename T, typename ParamSelection>
using Arg_t = typename ProxyHelper<T>::template AsArg_t<ParamSelection>;

template <typename T>
using AsDecl_t = typename ProxyHelper<T>::AsDecl_t;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROXY_CONVENIENCE_ALIASES_H_
