// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "type_map.h"

#include <optional>
#include <string>
#include <tuple>
#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using jni::metaprogramming::LambdaStringToType;
using jni::metaprogramming::StringAsType;
using jni::metaprogramming::TypemapBuilder_t;

TEST(TypeMap, CompilesWhenEmptyWithOrWithoutCallableArgs) {
  static_assert(
      std::is_same_v<TypemapBuilder_t<std::tuple<>, std::tuple<>>,
                     TypemapBuilder_t<std::tuple<>, std::tuple<>, void>>);
  static_assert(std::is_same_v<
                TypemapBuilder_t<std::tuple<>, std::tuple<>, std::tuple<>>,
                TypemapBuilder_t<std::tuple<>, std::tuple<>, std::tuple<>>>);
}

TEST(TypeMap, SimpleTest) {
  struct B {};
  TypemapBuilder_t<
      std::tuple<StringAsType<'F', 'O', 'O'>, StringAsType<'B', 'A', 'R'>>,
      std::tuple<double, B>>
      tp;
  static_assert(std::is_same_v<decltype(tp.Get("FOO")), double&>);
  static_assert(std::is_same_v<decltype(tp.Get("BAR")), B&>);
}

TEST(TypeMap, TypeMapWithStrings) {
  constexpr auto string_lambda_1 = STR("FOO");
  constexpr auto string_lambda_2 = STR("BAR");
  constexpr auto string_lambda_3 = STR("BAZ");

  using StringType1 = LambdaStringToType<decltype(string_lambda_1)>;
  using StringType2 = LambdaStringToType<decltype(string_lambda_2)>;
  using StringType3 = LambdaStringToType<decltype(string_lambda_3)>;

  TypemapBuilder_t<std::tuple<StringType1, StringType2, StringType3>,
                   std::tuple<int, float, std::optional<std::string>>>
      type_map;

  static_assert(std::is_same_v<decltype(type_map.Get<StringType1>()), int&>);
  static_assert(std::is_same_v<decltype(type_map.Get("FOO")), int&>);
  static_assert(std::is_same_v<decltype(type_map.Get("BAR")), float&>);
  static_assert(std::is_same_v<decltype(type_map.Get<StringType2>()), float&>);
  static_assert(std::is_same_v<decltype(type_map.Get("BAZ")),
                               std::optional<std::string>&>);
  static_assert(std::is_same_v<decltype(type_map.Get<StringType3>()),
                               std::optional<std::string>&>);
}

}  // namespace
