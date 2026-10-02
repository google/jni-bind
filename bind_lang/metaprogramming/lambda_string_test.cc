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

#include "lambda_string.h"

#include <type_traits>

#include <gtest/gtest.h>

using ::jni::metaprogramming::ConstexprStrlen;
using ::jni::metaprogramming::LambdaStringToType;
using ::jni::metaprogramming::StringAsType;

namespace {

TEST(LambdaStringTest, ConstexprStrlenTest) {
  static_assert(ConstexprStrlen("") == 0);
  static_assert(ConstexprStrlen("A") == 1);
  static_assert(ConstexprStrlen("AB") == 2);
  static_assert(ConstexprStrlen("ABC") == 3);
  static_assert(ConstexprStrlen("AAAAAAAAAAAAAAAAAAAA") == 20);
}

TEST(LambdaStringTest, EmptyStrTest) {
  constexpr auto string_lambda = STR("");
  static_assert(std::is_same_v<LambdaStringToType<decltype(string_lambda)>,
                               StringAsType<>>);
}

TEST(LambdaStringTest, BasicStrTest1) {
  constexpr auto string_lambda = STR("FOO");
  static_assert(std::is_same_v<LambdaStringToType<decltype(string_lambda)>,
                               StringAsType<'F', 'O', 'O'>>);
}

TEST(LambdaStringTest, BasicStrTest2) {
  constexpr auto string_lambda =
      STR("com/google/Package1/Package2/Package3/Package4/Package5/Package6");
  static_assert(
      std::is_same_v<
          StringAsType<'c', 'o', 'm', '/', 'g', 'o', 'o', 'g', 'l', 'e', '/',
                       'P', 'a', 'c', 'k', 'a', 'g', 'e', '1', '/', 'P', 'a',
                       'c', 'k', 'a', 'g', 'e', '2', '/', 'P', 'a', 'c', 'k',
                       'a', 'g', 'e', '3', '/', 'P', 'a', 'c', 'k', 'a', 'g',
                       'e', '4', '/', 'P', 'a', 'c', 'k', 'a', 'g', 'e', '5',
                       '/', 'P', 'a', 'c', 'k', 'a', 'g', 'e', '6'>,
          LambdaStringToType<decltype(string_lambda)>>);
}

}  // namespace
