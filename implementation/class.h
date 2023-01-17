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

#include <limits>
#include <string_view>
#include <tuple>
#include <type_traits>

#include "constructor.h"
#include "field.h"
#include "object.h"
#include "implementation/method.h"
#include "implementation/no_idx.h"
#include "implementation/static.h"
#include "jni_dep.h"
#include "metaprogramming/all_unique.h"
#include "metaprogramming/base_filter.h"

namespace jni {

static constexpr struct NoClass {
  const char* name_ = "__JNI_BIND__NO_CLASS__";
  const Static<std::tuple<>, std::tuple<>> static_{};
  const std::tuple<> methods_{};
  const std::tuple<> fields_{};

  constexpr bool operator==(const NoClass&) const { return true; }
  constexpr bool operator!=(const NoClass&) const { return true; }
} kNoClassSpecified;

template <typename Constructors_, typename Static_, typename Methods_,
          typename Fields_>
struct Class {};

template <typename... Constructors_, typename... StaticMethods_,
          typename... StaticFields_, typename... Methods_, typename... Fields_>
struct Class<std::tuple<Constructors_...>,
             std::tuple<Static<std::tuple<StaticMethods_...>,
                               std::tuple<StaticFields_...>>>,
             std::tuple<Methods_...>, std::tuple<Fields_...>> : public Object {
 public:
  const std::tuple<Constructors_...> constructors_;
  const Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
      static_;
  const std::tuple<Methods_...> methods_;
  const std::tuple<Fields_...> fields_;

  // Ctors + static.
  explicit constexpr Class(
      const char* class_name, Constructors_... constructors,
      Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
          statik,
      Methods_... methods, Fields_... fields)
      : Object(class_name),
        constructors_(constructors...),
        static_(statik),
        methods_(methods...),
        fields_(fields...) {}

  // No ctors, static.
  explicit constexpr Class(
      const char* class_name,
      Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
          statik,
      Methods_... methods, Fields_... fields)
      : Object(class_name),
        constructors_(Constructor<>{}),
        static_(statik),
        methods_(methods...),
        fields_(fields...) {}

  // Ctors, no static.
  explicit constexpr Class(const char* class_name,
                           Constructors_... constructors, Methods_... methods,
                           Fields_... fields)
      : Object(class_name),
        constructors_(constructors...),
        static_(Static{}),
        methods_(methods...),
        fields_(fields...) {}

  // No ctors, no static.
  explicit constexpr Class(const char* class_name, Methods_... methods,
                           Fields_... fields)
      : Class(class_name, Constructor<>{}, Static{}, methods..., fields...) {}

  template <typename... Params, typename... Constructors,
            typename... StaticMethods, typename... StaticFields,
            typename... Fields, typename... Methods>
  constexpr bool operator==(
      const Class<std::tuple<Constructors...>,
                  std::tuple<Static<std::tuple<StaticMethods...>,
                                    std::tuple<StaticFields...>>>,
                  std::tuple<Methods...>, std::tuple<Fields...>>& rhs) const {
    // Don't compare the other parameters so classes can be used as parameters
    // or return values before the class itself is defined.
    return std::string_view(name_) == std::string_view(rhs.name_);
  }

  constexpr bool operator==(const NoClass&) const { return false; }
  constexpr bool operator!=(const NoClass&) const { return false; }
};

template <typename... Params>
Class(const char*, Params...)
    -> Class<metaprogramming::BaseFilterWithDefault_t<ConstructorBase,
                                                      Constructor<>, Params...>,
             metaprogramming::BaseFilterWithDefault_t<
                 StaticBase, Static<std::tuple<>, std::tuple<>>, Params...>,
             metaprogramming::BaseFilter_t<MethodBase, Params...>,
             metaprogramming::BaseFilter_t<FieldBase, Params...>>;

Class(const char*)
    ->Class<std::tuple<Constructor<>>,
            std::tuple<Static<std::tuple<>, std::tuple<>>>, std::tuple<>,
            std::tuple<>>;

}  // namespace jni

#endif  // JNI_BIND_CLASS_H_
