/*
 * Copyright 2022 Google LLC
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
#include "per_element_val.h"

#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using jni::metaprogramming::PerElementVal_v;

namespace {

// operator== isn't constexpr on some older STL, so this custom func is used.
// Furthermore, these parameters cannot be const auto pre-C++20.
template <const auto& v1, const auto& v2>
struct Equals;

template <typename T1, std::size_t sz1, const std::array<T1, sz1>& v1,
          typename T2, std::size_t sz2, const std::array<T2, sz2>& v2>
struct Equals<v1, v2> {
  template <typename T>
  struct Helper {};

  template <std::size_t... Is>
  struct Helper<std::index_sequence<Is...>> {
    static constexpr bool val{((std::get<Is>(v1) == std::get<Is>(v2)) && ...)};
  };

  static constexpr bool Val() {
    if constexpr (sz1 == sz2) {
      return Helper<std::make_index_sequence<sz1>>::val;
    }
    return false;
  }

  static constexpr bool val = Val();
};

// Helper struct to report if a value is divisible by 3.
struct Div3 {
  template <const std::size_t vs>
  static constexpr const bool val = (vs % 3) == 0;
};

// Test set 0:
static constexpr auto ans_0_0 = std::array{true};
static constexpr auto ans_0_1 = std::array{false};
static constexpr auto ans_0_2 = std::array{false};
static constexpr auto ans_0_3 = std::array{true};
static constexpr auto ans_0_4 = std::array{false};

static_assert(Equals<PerElementVal_v<Div3, 0>, ans_0_0>::val);
static_assert(Equals<PerElementVal_v<Div3, 1>, ans_0_1>::val);
static_assert(Equals<PerElementVal_v<Div3, 2>, ans_0_2>::val);
static_assert(Equals<PerElementVal_v<Div3, 3>, ans_0_3>::val);
static_assert(Equals<PerElementVal_v<Div3, 4>, ans_0_4>::val);

// Test set 1:
static constexpr auto ans_1_0 = std::array{true, false};
static constexpr auto ans_1_1 = std::array{false};
static constexpr auto ans_1_2 = std::array{false};
static constexpr auto ans_1_3 = std::array{true};
static constexpr auto ans_1_4 = std::array{false};

static_assert(Equals<PerElementVal_v<Div3, 0, 1>, ans_1_0>::val);
static_assert(Equals<PerElementVal_v<Div3, 1>, ans_1_1>::val);
static_assert(Equals<PerElementVal_v<Div3, 2>, ans_1_2>::val);
static_assert(Equals<PerElementVal_v<Div3, 3>, ans_1_3>::val);
static_assert(Equals<PerElementVal_v<Div3, 4>, ans_1_4>::val);

}  // namespace
