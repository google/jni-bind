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

#include "invocable_map.h"

#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using ::jni::metaprogramming::InvocableMap_t;

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
    : public InvocableMap_t<SampleClassNowExposingCallOperator1, name_container,
                            &NameContainer::container1_> {
 protected:
  template <typename CrtpBase, const auto& tup_container_v,
            typename TupContainerT, const auto TupContainerT::*nameable_member,
            size_t I>
  friend class jni::metaprogramming::InvocableMapEntry;

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
  val("Foo", 1);
  val("Bar", 2.f, 3.f);
  val("Baz", 4, 5.f, double{6});

  // By design, doesn't compile.
  // val("BazNar", 7, 8, 9);
}

////////////////////////////////////////////////////////////////////////////////
class SampleClassNowExposingCallOperator2
    : public InvocableMap_t<SampleClassNowExposingCallOperator2, name_container,
                            &NameContainer::container2_> {
 public:
  template <typename CrtpBase, const auto& tup_container_v,
            typename TupContainerT, const auto TupContainerT::*nameable_member,
            size_t I>
  friend class jni::metaprogramming::InvocableMapEntry;

  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) {
    if (std::string_view(key) == "Fizz") {
      EXPECT_TRUE(I == 0);
      EXPECT_TRUE((std::is_same_v<std::tuple<Args...>, std::tuple<int>>));
    } else if (std::string_view(key) == "Buzz") {
      EXPECT_TRUE(I == 1);
      EXPECT_TRUE(
          (std::is_same_v<std::tuple<Args...>, std::tuple<float, float>>));
    } else if (std::string_view(key) == "Bang") {
      EXPECT_TRUE(I == 2);
      EXPECT_TRUE((
          std::is_same_v<std::tuple<Args...>, std::tuple<int, float, double>>));
    } else {
      FAIL();
    }
  }
};

TEST(InvocableMapTest2, HasCorrectTypesAndForwardsCalls) {
  SampleClassNowExposingCallOperator2 val;
  val("Fizz", 1);
  val("Buzz", 2.f, 3.f);
  val("Bang", 4, 5.f, double{6});

  // By design, doesn't compile.
  // val("BazNar", 7, 8, 9);
}

}  // namespace
