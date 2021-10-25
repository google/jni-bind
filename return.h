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

#ifndef JNI_BIND_RETURN_H_
#define JNI_BIND_RETURN_H_

#include <string_view>

namespace jni {

template <typename ReturnRaw_>
struct Return {
  const ReturnRaw_ return_raw_ = {};

  using ReturnRaw = ReturnRaw_;

  constexpr Return() = default;

  template <typename ReturnRaw>
  constexpr explicit Return(ReturnRaw return_raw) : return_raw_(return_raw) {}
};

// TODO(b/174272629): Remove when shifted to the new TypeProxy.
template <>
struct Return<void> {
  using ReturnRaw = void;

  constexpr Return() = default;
};

Return()->Return<void>;

template <typename ReturnRaw>
Return(ReturnRaw) -> Return<ReturnRaw>;

//==============================================================================

template <typename T>
using ReturnRaw_t = typename T::ReturnRaw;

}  // namespace jni

#endif  // JNI_BIND_RETURN_H_
