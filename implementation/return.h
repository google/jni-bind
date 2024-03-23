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

#include "implementation/void.h"

namespace jni {

struct ReturnBase {};

template <typename Raw_>
struct Return : ReturnBase {
  const Raw_ raw_ = {};

  using Raw = Raw_;

  constexpr Return() = default;

  template <typename Raw>
  constexpr explicit Return(Raw raw) : raw_(raw) {}
};

template <>
struct Return<void> : ReturnBase {
  using Raw = void;
  const Void raw_{};

  constexpr Return() = default;
};

Return()->Return<void>;

template <typename Raw>
Return(Raw) -> Return<Raw>;

template <typename T>
using Raw_t = typename T::Raw;

}  // namespace jni

#endif  // JNI_BIND_RETURN_H_
