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

#ifndef JNI_BIND_METAPROGRAMMING_TYPE_MAP_H_
#define JNI_BIND_METAPROGRAMMING_TYPE_MAP_H_

#include <string_view>
#include <tuple>
#include <utility>

#include "interleave.h"
#include "lambda_string.h"
#include "tuple_from_size.h"
#include "tuple_manipulation.h"
#include "type_of_nth_element.h"

namespace jni::metaprogramming {

template <typename Key, typename Value, typename... Args>
class TypeMapEntry {};

template <typename Key, typename Value, typename... Args>
class TypeMapEntry<Key, Value, std::tuple<Args...>> {
 public:
  static_assert(std::is_base_of_v<metaprogramming::StringAsTypeBase, Key>);
  static constexpr std::string_view name_ = Key::chars_as_sv;

  template <typename Query,
            typename = std::enable_if_t<std::is_same_v<Key, Query>, void>>
  constexpr Value& Get() {
    return value_;
  }

#if __clang__
  // Android NDK seems to have a compiler issue that loses constexpr-ness on
  // std::string_view but not const char*.
  constexpr Value& Get(const char* key)
      __attribute__((enable_if(key == Key::chars_as_sv, ""))) {
    return value_;
  }
#endif

 private:
  Value value_ = {};
};

template <typename KeysTuple, typename ValuesTuple, typename ArgsTuple,
          typename IndexSequenceType>
class TypeMapBase {};

template <typename... Keys, typename... Values, typename... ArgTups,
          size_t... Indices>
class TypeMapBase<std::tuple<Keys...>, std::tuple<Values...>,
                  std::tuple<ArgTups...>, std::index_sequence<Indices...>>
    : public TypeMapEntry<TypeOfNthElement_t<Indices, Keys...>,
                          TypeOfNthElement_t<Indices, Values...>,
                          TypeOfNthElement_t<Indices, ArgTups...>>... {
 public:
  using TypeMapEntry<TypeOfNthElement_t<Indices, Keys...>,
                     TypeOfNthElement_t<Indices, Values...>,
                     TypeOfNthElement_t<Indices, ArgTups...>>::Get...;

  static_assert(sizeof...(Keys) == sizeof...(Values));
};

template <typename KeysTuple, typename ValuesTuple, typename... ArgTupsTuple>
class TypeMap {};

template <typename... Keys, typename... Values, typename... ArgTups>
class TypeMap<std::tuple<Keys...>, std::tuple<Values...>,
              std::tuple<ArgTups...>>
    : public TypeMapBase<std::tuple<Keys...>, std::tuple<Values...>,
                         std::tuple<ArgTups...>,
                         std::index_sequence_for<Keys...>> {
  static_assert(sizeof...(Keys) == sizeof...(Values));
  static_assert(sizeof...(Keys) == sizeof...(ArgTups));
};

template <typename... Keys, typename... Values>
class TypeMap<std::tuple<Keys...>, std::tuple<Values...>, void>
    : public TypeMap<std::tuple<Keys...>, std::tuple<Values...>,
                     decltype(TupleFromSize<std::tuple<>, sizeof...(Keys)>())> {
};

template <typename, typename>
struct TypeMapBuilder {};

template <typename KeysAsTuple, typename ValuesAsTuple,
          typename ArgsAsTuple = void>
using TypemapBuilder_t = TypeMap<KeysAsTuple, ValuesAsTuple, ArgsAsTuple>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_TYPE_MAP_H_
