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

#include "optional_wrap.h"

#include <optional>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

using jni::metaprogramming::OptionalTup_t;

namespace {

TEST(OptionalTest, EmptyWrap) {
  static_assert(std::is_same_v<OptionalTup_t<std::tuple<>>, std::tuple<>>);
}

TEST(OptionalTest, SimpleElementWrap) {
  static_assert(std::is_same_v<OptionalTup_t<std::tuple<int>>,
                               std::tuple<std::optional<int>>>);
}

TEST(OptionalTest, MultiElememtWrap) {
  struct ComplexType {
    int a;
    int b;
    int c;
  };
  static_assert(
      std::is_same_v<OptionalTup_t<std::tuple<int, float, ComplexType>>,
                     std::tuple<std::optional<int>, std::optional<float>,
                                std::optional<ComplexType>>>);
}

}  // namespace
