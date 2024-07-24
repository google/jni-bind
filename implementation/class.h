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

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <string_view>
#include <tuple>

#include "implementation/constructor.h"
#include "implementation/extends.h"
#include "implementation/field.h"
#include "implementation/method.h"
#include "implementation/no_class_specified.h"
#include "implementation/object.h"
#include "implementation/static.h"
#include "metaprogramming/base_filter.h"
#include "metaprogramming/type_of_nth_element.h"

namespace jni {

template <typename Extends_, typename Constructors_, typename Static_,
          typename Methods_, typename Fields_>
struct Class {};

template <typename Extends_, typename... Constructors_,
          typename... StaticMethods_, typename... StaticFields_,
          typename... Methods_, typename... Fields_>
struct Class<Extends_, std::tuple<Constructors_...>,
             std::tuple<Static<std::tuple<StaticMethods_...>,
                               std::tuple<StaticFields_...>>>,
             std::tuple<Methods_...>, std::tuple<Fields_...>> : public Object {
 public:
  // Filtering outputs a std::tuple<T>, the caller will use just T in ctor.
  using ExtendsArgT = metaprogramming::TypeOfNthTupleElement_t<0, Extends_>;

  // The type of the parent class (default `RootObject`).
  const ExtendsStrip_t<Extends_> parent_;

  const std::tuple<Constructors_...> constructors_;
  const Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
      static_;
  const std::tuple<Methods_...> methods_;
  const std::tuple<Fields_...> fields_;

  ////////////////////////////////////////////////////////////////////////////////
  // Constructors can pass any correctly ordered permutation of:
  // -  Extends (parent declaration)
  // -  Constructors
  // -  Statics
  // -  Methods
  // -  Fields
  //
  // For types that are not packs (e.g. Statics), they must have permutations
  // provided where they are and aren't present.
  ////////////////////////////////////////////////////////////////////////////////

  // Methods + Fields.
  explicit constexpr Class(const char* class_name, Methods_... methods,
                           Fields_... fields)
      : Class(class_name, Constructor<>{}, Static{}, methods..., fields...) {}

  // Constructors + Statics + Methods + Fields.
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

  // Statics + Methods + Fields.
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

  // Constructors only + Methods + Fields.
  explicit constexpr Class(const char* class_name,
                           Constructors_... constructors, Methods_... methods,
                           Fields_... fields)
      : Object(class_name),
        constructors_(constructors...),
        static_(Static{}),
        methods_(methods...),
        fields_(fields...) {}

  ////////////////////////////////////////////////////////////////////////////////
  // Constructors with `Extends`.
  ////////////////////////////////////////////////////////////////////////////////

  // Extends + Methods + Fields.
  explicit constexpr Class(const char* class_name, ExtendsArgT extends,
                           Methods_... methods, Fields_... fields)
      : parent_(extends.parent_),
        Object(class_name),
        methods_(methods...),
        fields_(fields...) {}

  // Extends + Statics + Methods + Fields.
  explicit constexpr Class(
      const char* class_name, ExtendsArgT extends,
      Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
          statik,
      Methods_... methods, Fields_... fields)
      : Object(class_name),
        parent_(extends.parent_),
        static_(statik),
        methods_(methods...),
        fields_(fields...) {}

  // Extends + Constructors + Methods + Fields.
  explicit constexpr Class(const char* class_name, ExtendsArgT extends,
                           Constructors_... constructors, Methods_... methods,
                           Fields_... fields)
      : Object(class_name),
        parent_(extends.parent_),
        constructors_(constructors...),
        methods_(methods...),
        fields_(fields...) {}

  // Extends + Statics + Constructor + Methods + Fields.
  explicit constexpr Class(
      const char* class_name, ExtendsArgT extends,
      Constructors_... constructors,
      Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
          statik,
      Methods_... methods, Fields_... fields)
      : Object(class_name),
        parent_(extends.parent_),
        constructors_(constructors...),
        static_(statik),
        methods_(methods...),
        fields_(fields...) {}

  ////////////////////////////////////////////////////////////////////////////////
  // Equality operators.
  ////////////////////////////////////////////////////////////////////////////////
  template <typename ParentClass, typename... Params, typename... Constructors,
            typename... StaticMethods, typename... StaticFields,
            typename... Fields, typename... Methods>
  constexpr bool operator==(
      const Class<ParentClass, std::tuple<Constructors...>,
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
    -> Class<metaprogramming::BaseFilterWithDefault_t<
                 ExtendsBase, Extends<RootObject>, Params...>,
             metaprogramming::BaseFilterWithDefault_t<ConstructorBase,
                                                      Constructor<>, Params...>,
             metaprogramming::BaseFilterWithDefault_t<
                 StaticBase, Static<std::tuple<>, std::tuple<>>, Params...>,
             metaprogramming::BaseFilter_t<MethodBase, Params...>,
             metaprogramming::BaseFilter_t<FieldBase, Params...>>;

Class(const char*)
    -> Class<std::tuple<Extends<RootObject>>, std::tuple<Constructor<>>,
             std::tuple<Static<std::tuple<>, std::tuple<>>>, std::tuple<>,
             std::tuple<>>;

}  // namespace jni

#endif  // JNI_BIND_CLASS_H_
