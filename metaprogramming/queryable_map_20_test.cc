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

#include "queryable_map_20.h"

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>

#include "string_literal.h"
#include <gtest/gtest.h>

#if __cplusplus >= 202002L

using jni::metaprogramming::QueryableMap20_t;
using jni::metaprogramming::StringLiteral;

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
    : public QueryableMap20_t<SampleClassNowExposingCallOperator1,
                              name_container, &NameContainer::container1_> {
 protected:
  friend QueryableMap20_t<SampleClassNowExposingCallOperator1, name_container,
                          &NameContainer::container1_>;

  template <size_t I, StringLiteral key_literal>
  auto QueryableMap20Call() {
    if (std::string_view(key_literal.value) == "Foo") {
      EXPECT_TRUE(I == 0);
    } else if (std::string_view(key_literal.value) == "Bar") {
      EXPECT_TRUE(I == 1);
    } else if (std::string_view(key_literal.value) == "Baz") {
      EXPECT_TRUE(I == 2);
    } else {
      FAIL();
    }
  }
};

TEST(QueryableMap20Test1, HasCorrectTypesAndForwardsCalls) {
  SampleClassNowExposingCallOperator1 val;
  val.Access<"Foo">();
  val.Access<"Bar">();
  val.Access<"Baz">();

  // By design, doesn't compile.
  // val("BazNar", 7, 8, 9);
}

#endif  // __cplusplus >= 202002L
