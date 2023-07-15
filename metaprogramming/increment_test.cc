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

#include "increment.h"

#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "invoke.h"

using ::jni::metaprogramming::Increment_t;

namespace {

// Defines how to increment a std::integral_constant one time.
// Other types can write these increment-like proxy adapters to define their own
// custom implementations of incrementing.
template <std::size_t I>
struct IncrementableInt {
  // Note, here, the type alias is "Increment", not the canonical "type" which
  // is for metafunctions.
  using Increment = IncrementableInt<I + 1>;

  constexpr static int value = I;
};

// 1 + 1 = 2
static_assert(Increment_t<IncrementableInt<1>>::value == 2);
// 2 + 1 = 3
static_assert(Increment_t<IncrementableInt<2>>::value == 3);
// 1 + 2 = 3
static_assert(Increment_t<IncrementableInt<1>, 2>::value == 3);

}  // namespace
