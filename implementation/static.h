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

#ifndef JNI_BIND_IMPLEMENTATION_STATIC_H_
#define JNI_BIND_IMPLEMENTATION_STATIC_H_

// IWYU pragma: private, include "../jni_bind.h"

#include <tuple>

#include "implementation/field.h"
#include "implementation/method.h"
#include "metaprogramming/base_filter.h"

namespace jni {

class StaticBase {};

template <typename Methods_, typename Fields_>
class Static;

template <typename... Methods_, typename... Fields_>
class Static<std::tuple<Methods_...>, std::tuple<Fields_...>>
    : public StaticBase {
 public:
  const std::tuple<Methods_...> methods_;
  const std::tuple<Fields_...> fields_;

  constexpr Static(Methods_... methods, Fields_... fields)
      : methods_(methods...), fields_(fields...) {}
};

Static() -> Static<std::tuple<>, std::tuple<>>;

template <typename... Params>
Static(Params...)
    -> Static<metaprogramming::BaseFilter_t<MethodBase, Params...>,
              metaprogramming::BaseFilter_t<FieldBase, Params...>>;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_STATIC_H_
