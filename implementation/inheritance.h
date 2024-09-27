/*
 * Copyright 2024 Google LLC
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

#ifndef JNI_BIND_IMPLEMENTATION_INHERITANCE_H_
#define JNI_BIND_IMPLEMENTATION_INHERITANCE_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <type_traits>

#include "implementation/id_type.h"

namespace jni {

struct Inheritance {
  template <typename IdT, typename Enable = void>
  struct Helper {
    static_assert(false,
                  "You must use an overload set or field for `Inheritance`.");
  };

  template <typename IdT>
  struct Helper<IdT, std::enable_if_t<IdT::kIDType == IdType::OVERLOAD_SET>> {
    using type = void;
  };

  template <typename IdT>
  using type = typename Helper<IdT>::type;
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_INHERITANCE_H_
