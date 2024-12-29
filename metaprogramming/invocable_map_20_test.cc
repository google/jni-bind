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

#include "invocable_map_20.h"

#include <string_view>
#include <tuple>
#include <type_traits>

#include "invocable_map_20.h"
#include "modified_max.h"
#include "string_literal.h"
#include <gtest/gtest.h>

using jni::metaprogramming::InvocableMap20_t;

struct Str {
  const char* name_;
  constexpr Str(const char* name) : name_(name) {}
};

struct NameContainer {
  std::tuple<Str, Str, Str> container1_;
  std::tuple<Str, Str, Str> container2_;
};

constexpr NameContainer name_container{
    {{"Foo"}, {"Bar"}, {"Baz"}},
    {{"Fizz"}, {"Buzz"}, {"Bang"}},
};

////////////////////////////////////////////////////////////////////////////////
class SampleClassNowExposingCallOperator1
    : public InvocableMap20_t<SampleClassNowExposingCallOperator1,
                              name_container, &NameContainer::container1_> {
 protected:
  friend InvocableMap20_t<SampleClassNowExposingCallOperator1, name_container,
                          &NameContainer::container1_>;

  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... ts) {
    if (std::string_view(key) == "Foo") {
      EXPECT_TRUE(I == 0);
      EXPECT_TRUE((std::is_same_v<std::tuple<Args...>, std::tuple<int>>));
    } else if (std::string_view(key) == "Bar") {
      EXPECT_TRUE(I == 1);
      EXPECT_TRUE(
          (std::is_same_v<std::tuple<Args...>, std::tuple<float, float>>));
    } else if (std::string_view(key) == "Baz") {
      EXPECT_TRUE(I == 2);
      EXPECT_TRUE((
          std::is_same_v<std::tuple<Args...>, std::tuple<int, float, double>>));
    } else {
      FAIL();
    }
  }
};

TEST(InvocableMapTest1, HasCorrectTypesAndForwardsCalls) {
  SampleClassNowExposingCallOperator1 val;
  val.Call<"Foo">(1);
  val.Call<"Bar">(2.f, 3.f);
  val.Call<"Baz">(4, 5.f, double{6});

  // By design, doesn't compile.
  // val("BazNar", 7, 8, 9);
}
