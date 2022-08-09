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

#ifndef JNI_BIND_IMPLEMENTATION_JNI_TYPE_H_
#define JNI_BIND_IMPLEMENTATION_JNI_TYPE_H_

#include <type_traits>

#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/jvm.h"

namespace jni {

template <typename SpanType_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
struct JniType {
  template <typename T>
  struct Helper {
    static constexpr bool val = false;
  };

  template <typename SpanType, const auto& class_v, const auto& class_loader_v,
            const auto& jvm_v>
  struct Helper<JniType<SpanType, class_v, class_loader_v, jvm_v>> {
    static constexpr bool val =
        std::is_same_v<SpanType_, SpanType> && (class_v_ == class_v) &&
        (class_loader_v_ == class_loader_v) && (jvm_v_ == jvm_v);
  };

  constexpr JniType() = default;

  template <typename T>
  constexpr bool operator==(T&& rhs) const {
    return Helper<std::decay_t<T>>::val;
  }

  template <typename T>
  constexpr bool operator!=(T&& rhs) const {
    return !(*this == rhs);
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JNI_TYPE_H_
