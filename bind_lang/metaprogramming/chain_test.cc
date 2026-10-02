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

#include "chain.h"

#include <cstddef>
#include <type_traits>

#include "increment.h"
#include "invoke.h"
#include "same.h"

using ::jni::metaprogramming::Chain;
using ::jni::metaprogramming::Increment;
using ::jni::metaprogramming::Increment_t;
using ::jni::metaprogramming::Invoke_t;
using ::jni::metaprogramming::Same;

namespace {

struct A {};
struct B {};

// Defines how to increment a std::integral_constant one time.
// Other types can write these increment-like proxy adapters to define their own
// custom implementations of incrementing.
template <size_t I>
struct IncrementableInt {
  // Note, here, the type alias is "Increment", not the canonical "type" which
  // is for metafunctions.
  using Increment = IncrementableInt<I + 1>;

  // Returns a convenience integral_constant.
  using type = std::integral_constant<size_t, I>;

  constexpr static auto value = type::value;
};

// Simple single element chains.
static_assert(std::is_same_v<Invoke_t<Chain<Same<A>>, A>, std::true_type>);
static_assert(std::is_same_v<Invoke_t<Chain<Same<A>>, B>, std::false_type>);

static_assert(std::is_same_v<Invoke_t<Chain<Same<A>, Same<std::true_type>>, A>,
                             std::true_type>);

static_assert(Increment_t<IncrementableInt<1>>::value == 2);

// Single type incremementing in a chain.
static_assert(Invoke_t<Chain<Increment<1>>, IncrementableInt<1>>::value == 2);
static_assert(
    Invoke_t<Chain<Increment<1>, Increment<1>>, IncrementableInt<1>>::value ==
    3);
static_assert(
    Invoke_t<Chain<Increment<1>, Increment<2>>, IncrementableInt<1>>::value ==
    4);
static_assert(Invoke_t<Chain<Increment<1>, Increment<1>, Increment<1>>,
                       IncrementableInt<1>>::value == 4);

// Chain with different types.
static_assert(Invoke_t<Chain<Increment<1>, Same<IncrementableInt<2>>>,
                       IncrementableInt<1>>::value);
static_assert(
    !Invoke_t<Chain<Increment<1>, Increment<1>, Same<IncrementableInt<2>>>,
              IncrementableInt<1>>::value);
static_assert(Invoke_t<Chain<Increment<1>, Increment<1>, Increment<1>,
                             Same<IncrementableInt<4>>>,
                       IncrementableInt<1>>::value);

}  // namespace
