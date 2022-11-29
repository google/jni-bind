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

#include "per_element.h"

#include <tuple>
#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using jni::metaprogramming::PerElement;

// Metafunction that binds an amount and increments incoming integral constants.
template <size_t Amt>
struct IntegralIncrement {
  template <typename T>
  struct IncrementHelper {
    static_assert(std::is_same_v<T, void>,
                  "Only std::integral_constant implementation exists.");
  };

  template <size_t I>
  struct IncrementHelper<std::integral_constant<size_t, I>> {
    using type = std::integral_constant<size_t, I + Amt>;
  };

  template <typename IntegralConstantType>
  using type = typename IncrementHelper<IntegralConstantType>::type;
};

static_assert(std::is_same_v<
              IntegralIncrement<1>::type<std::integral_constant<size_t, 1>>,
              std::integral_constant<size_t, 2>>);
static_assert(std::is_same_v<
              IntegralIncrement<5>::type<std::integral_constant<size_t, 1>>,
              std::integral_constant<size_t, 6>>);
static_assert(std::is_same_v<
              IntegralIncrement<10>::type<std::integral_constant<size_t, 5>>,
              std::integral_constant<size_t, 15>>);

// Per Element application
static_assert(std::is_same_v<PerElement<IntegralIncrement<1>>::type<
                                 std::integral_constant<size_t, 1>,
                                 std::integral_constant<size_t, 2>,
                                 std::integral_constant<size_t, 3>>,

                             std::tuple<std::integral_constant<size_t, 2>,
                                        std::integral_constant<size_t, 3>,
                                        std::integral_constant<size_t, 4>>>);

static_assert(std::is_same_v<PerElement<IntegralIncrement<100>>::type<
                                 std::integral_constant<size_t, 1>,
                                 std::integral_constant<size_t, 2>,
                                 std::integral_constant<size_t, 3>>,

                             std::tuple<std::integral_constant<size_t, 101>,
                                        std::integral_constant<size_t, 102>,
                                        std::integral_constant<size_t, 103>>>);
}  // namespace
