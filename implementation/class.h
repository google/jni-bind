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

#ifndef JNI_BIND_CLASS_H_
#define JNI_BIND_CLASS_H_

#include <string_view>
#include <tuple>
#include <type_traits>

#include "constructor.h"
#include "field.h"
#include "object.h"
#include "implementation/method.h"
#include "jni_dep.h"
#include "metaprogramming/all_unique.h"
#include "metaprogramming/base_filter.h"

namespace jni {

static constexpr struct NoClass {
  const char* name_ = "__JNI_BIND__NO_CLASS__";
} kNoClassSpecified;

template <typename Constructors_, typename Fields_, typename Methods_>
struct Class {};

template <typename... Constructors_, typename... Fields_, typename... Methods_>
struct Class<std::tuple<Constructors_...>, std::tuple<Fields_...>,
             std::tuple<Methods_...>> : public Object {
 public:
  const std::tuple<Constructors_...> constructors_;
  const std::tuple<Methods_...> methods_;
  const std::tuple<Fields_...> fields_;

  explicit constexpr Class(const char* class_name,
                           Constructors_... constructors, Methods_... methods,
                           Fields_... fields)
      : Object(class_name),
        constructors_(constructors...),
        methods_(methods...),
        fields_(fields...) {}

  explicit constexpr Class(const char* class_name, Methods_... methods,
                           Fields_... fields)
      : Class(class_name, Constructor<>{}, methods..., fields...) {}

  template <typename... Params, typename... Constructors, typename... Fields,
            typename... Methods>
  constexpr bool operator==(
      const Class<std::tuple<Constructors...>, std::tuple<Fields...>,
                  std::tuple<Methods...>>& rhs) const {
    // Don't compare the other parameters so classes can be used as parameters
    // or return values before the class itself is defined.
    return std::string_view(name_) == std::string_view(rhs.name_);
  }
};

template <typename... Params>
Class(const char*, Params...)
    -> Class<metaprogramming::BaseFilterWithDefault_t<ConstructorBase,
                                                      Constructor<>, Params...>,
             metaprogramming::BaseFilter_t<FieldBase, Params...>,
             metaprogramming::BaseFilter_t<MethodBase, Params...>>;

Class(const char*)
    ->Class<std::tuple<Constructor<>>, std::tuple<>, std::tuple<>>;

}  // namespace jni

#endif  // JNI_BIND_CLASS_H_
