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

#ifndef JNI_BIND_JNI_SIGNATURE_H_
#define JNI_BIND_JNI_SIGNATURE_H_

#include <string>
#include <utility>

#include "object.h"
#include "jni_helper/jni_typename_to_string.h"

namespace jni {

template <typename T>
inline static std::string SignatureForSingleParameter(const T& val) {
  if constexpr (std::is_base_of_v<Object, T>) {
    return "L" + std::string{val.name_} + ";";
  } else {
    return std::string{JavaTypeToString<T>()};
  }
}

template <typename... ParamTypes, std::size_t... I>
inline std::string GetSignatureForValueHelper(
    const std::tuple<ParamTypes...>& param_sequence,
    std::index_sequence<I...>) {
  return (SignatureForSingleParameter(std::get<I>(param_sequence)) + ... + "");
}

template <typename... ParamTypes>
inline static std::string SignatureForParameterTuple(
    const std::tuple<ParamTypes...>& param_sequence) {
  return GetSignatureForValueHelper(
      param_sequence, std::make_index_sequence<sizeof...(ParamTypes)>{});
}

}  // namespace jni

#endif  // JNI_BIND_JNI_SIGNATURE_H_
