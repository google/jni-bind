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

#ifndef JNI_BIND_METAPROGRAMMING_TUPLE_MANIPULATION_H_
#define JNI_BIND_METAPROGRAMMING_TUPLE_MANIPULATION_H_

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Helper struct for figuring out the contents of a tuple.  Use Unwrap if you
// plan to use this as a metafunction (this breaks the paradigm as type is not
// templatized, but instead embedded in the type).
template <typename Func, typename TupleType>
struct TupleUnroller {
  static_assert(std::is_same_v<TupleType, void>,
                "You used a non tuple type when a tuple was expected.");
};

template <typename Func, typename... Ts>
struct TupleUnroller<Func, std::tuple<Ts...>> {
  // Templated to avoid invalid instantiations.
  template <typename = void>
  struct TypeHelperStruct {
    using type = typename Func::template type<Ts...>;
  };

  // Templated to avoid invalid instantiations.
  template <typename = void>
  struct ValueHelperStruct {
    static constexpr auto value = Func::template value<Ts...>;
  };
};

template <typename Func, typename TupleType>
using TupleUnroller_t =
    typename TupleUnroller<Func,
                           TupleType>::template TypeHelperStruct<void>::type;

template <typename Func, typename TupleType>
static constexpr auto TupleUnroller_v =
    TupleUnroller<Func, TupleType>::template ValueHelperStruct<void>::value;

// Takes a variadic pack and injects it inside another type.
// e.g. TupleToType<std::tuple<int, float>>::type<std::pair> =
//        std::pair<int, float>.
template <typename T>
struct TupleToType {
  static_assert(std::is_same_v<T, std::tuple<>>,
                "TupleToType should be templated on a std::tuple type");
  using type = std::tuple<>;
};

template <typename... Ts>
struct TupleToType<std::tuple<Ts...>> {
  template <template <typename...> class Container>
  using type = Container<Ts...>;
};

template <typename TupleType, template <typename...> class Container>
using TupleToType_t = typename TupleToType<TupleType>::template type<Container>;

// Takes a container and extracts its variadic arguments into a tuple.
template <typename T>
struct ExtractTupleFromType {
  static_assert(
      std::is_same_v<T, void>,
      "ExtractTupleFromType should be used on a type with variadic params.");
};

template <template <typename...> class ContainerType, typename... Ts>
struct ExtractTupleFromType<ContainerType<Ts...>> {
  using type = std::tuple<Ts...>;
};

template <typename ContainerType>
using ExtractTupleFromType_t =
    typename ExtractTupleFromType<ContainerType>::type;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_TUPLE_MANIPULATION_H_
