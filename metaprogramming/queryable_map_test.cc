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

#include "queryable_map.h"

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

namespace {

using ::jni::metaprogramming::QueryableMap_t;

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

struct SomeCustomKeyStruct {
  int abc = 123;
};

using ValuesTup = std::tuple<int, SomeCustomKeyStruct, double>;

class SampleClassNowExposingCallOperator1
    : public QueryableMap_t<SampleClassNowExposingCallOperator1, name_container,
                            &NameContainer::container1_> {
 protected:
  template <typename CrtpBase, const auto& tup_container_v,
            typename TupContainerT, const auto TupContainerT::*nameable_member,
            size_t I>
  friend class jni::metaprogramming::QueryableMapEntry;

  template <size_t I>
  auto QueryableMapCall(const char* key) {
    if (std::string_view(key) == "Foo") {
      EXPECT_EQ(I, 0);
    } else if (std::string_view(key) == "Bar") {
      EXPECT_EQ(I, 1);
    } else if (std::string_view(key) == "Baz") {
      EXPECT_EQ(I, 2);
    } else {
      FAIL();
    }
  }
};

struct SomeIndexerStruct {
  const char* key_;
};

constexpr SomeIndexerStruct kSomeKey{"Foo"};

template <const auto& val>
void FuncThatTrampolinesStaticValue(SampleClassNowExposingCallOperator1& map) {
  map["Foo"];
  map[val.key_];
}

template <typename, typename = void>
struct InSet {
  using type = std::false_type;
};

TEST(QueryableMapTest1, HasCorrectTypesAndForwardsCalls) {
  SampleClassNowExposingCallOperator1 val;
  val["Foo"];
  val["Bar"];
  val["Baz"];

  // Note these are compile time.
  static_assert(val.Contains("Foo"));
  static_assert(!val.Contains("Fooz"));

  // This essentially validates classloader indexing
  val[kSomeKey.key_];

  FuncThatTrampolinesStaticValue<kSomeKey>(val);

  // By design, doesn't compile.
  // val["BazNar"];
}

class SampleClassNowExposingCallOperator2
    : public QueryableMap_t<SampleClassNowExposingCallOperator2, name_container,
                            &NameContainer::container2_> {
 protected:
  template <typename CrtpBase, const auto& tup_container_v,
            typename TupContainerT, const auto TupContainerT::*nameable_member,
            size_t I>
  friend class jni::metaprogramming::QueryableMapEntry;

  template <size_t I>
  auto QueryableMapCall(const char* key) {
    if (std::string_view(key) == "Fizz") {
      EXPECT_EQ(I, 0);
    } else if (std::string_view(key) == "Buzz") {
      EXPECT_EQ(I, 1);
    } else if (std::string_view(key) == "Bang") {
      EXPECT_EQ(I, 2);
    } else {
      FAIL();
    }
  }
};

TEST(QueryableMapTest2, HasCorrectTypesAndForwardsCalls) {
  SampleClassNowExposingCallOperator2 val;
  val["Fizz"];
  val["Buzz"];
  val["Bang"];

  // By design, doesn't compile.
  // val("BazNar", 7, 8, 9);
}

}  // namespace
