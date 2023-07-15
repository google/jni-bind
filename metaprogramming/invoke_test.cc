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

#include "invoke.h"

#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "increment.h"
#include "per_element.h"
#include "same.h"

using ::jni::metaprogramming::Increment;
using ::jni::metaprogramming::Invoke_t;
using ::jni::metaprogramming::InvokeAsPack_t;
using ::jni::metaprogramming::InvokePerTupArg_t;
using ::jni::metaprogramming::InvokePerTupArgAsPack_t;
using ::jni::metaprogramming::PerElement;
using ::jni::metaprogramming::Same;

namespace {

struct A {};
struct B {};
struct C {};
struct D {};

// Defines how to increment a std::integral_constant one time.
// Other types can write these increment-like proxy adapters to define their own
// custom implementations of incrementing.
template <size_t I>
struct IncrementableInt {
  // Note, here, the type alias is "Increment", not the canonical "type" which
  // is for metafunctions.
  using Increment = IncrementableInt<I + 1>;
};

template <typename IncrementableType>
using IncrementableInt_t = typename IncrementableType::type;

// Simple invocation with type that takes non-variadic args.
// (Increment is non-variadic).
static_assert(std::is_same_v<Invoke_t<Increment<1>, IncrementableInt<1>>,
                             IncrementableInt<2>>);

// Variadic packs with non trivial number of arguments.
// Examples of pre-binding the operation you want.
using OpType = Same<A, A>;
static_assert(std::is_same_v<Invoke_t<OpType, A>, std::false_type>);
static_assert(std::is_same_v<Invoke_t<OpType, A, A>, std::true_type>);
static_assert(std::is_same_v<Invoke_t<OpType, A>, std::false_type>);

// Per element, args as tup.
static_assert(
    std::is_same_v<InvokePerTupArg_t<Same<>, std::tuple<>>, std::tuple<>>);
static_assert(
    std::is_same_v<InvokePerTupArg_t<Same<A>, std::tuple<>>, std::tuple<>>);

static_assert(
    std::is_same_v<InvokePerTupArg_t<Same<A>, std::tuple<>>, std::tuple<>>);
static_assert(std::is_same_v<InvokePerTupArg_t<Same<A>, std::tuple<A>>,
                             std::tuple<std::true_type>>);
static_assert(std::is_same_v<InvokePerTupArg_t<Same<A>, std::tuple<B>>,
                             std::tuple<std::false_type>>);
static_assert(std::is_same_v<InvokePerTupArg_t<Same<A>, std::tuple<A, A>>,
                             std::tuple<std::true_type, std::true_type>>);
static_assert(std::is_same_v<InvokePerTupArg_t<Same<B>, std::tuple<A, B>>,
                             std::tuple<std::false_type, std::true_type>>);

// Per element, args as tup.
static_assert(
    std::is_same_v<InvokeAsPack_t<Same<A>, std::tuple<A>>, std::true_type>);
static_assert(std::is_same_v<InvokeAsPack_t<Same<A, A>, std::tuple<A, A>>,
                             std::true_type>);

// Invoked as pack from tup.
static_assert(
    std::is_same_v<InvokeAsPack_t<Same<A>, std::tuple<A>>, std::true_type>);
static_assert(std::is_same_v<InvokeAsPack_t<Same<A, A>, std::tuple<A, A>>,
                             std::true_type>);
static_assert(std::is_same_v<InvokeAsPack_t<Same<A, B, C>, std::tuple<A, B, C>>,
                             std::true_type>);

// Multiple variadic packs (tuples of tuples).
static_assert(std::is_same_v<
              InvokePerTupArgAsPack_t<
                  Same<A, A>, std::tuple<std::tuple<A, A>, std::tuple<A, A>>>,
              std::tuple<std::true_type, std::true_type>>);
static_assert(
    std::is_same_v<InvokePerTupArgAsPack_t<
                       Same<A, B, C>,
                       std::tuple<std::tuple<A>, std::tuple<A, B>,
                                  std::tuple<A, A, A>, std::tuple<A, B, C>>>,
                   std::tuple<std::false_type, std::false_type, std::false_type,
                              std::true_type>>);

using OpType2 = Same<A, B, C>;
static_assert(std::is_same_v<Invoke_t<OpType2, A>, std::false_type>);
static_assert(std::is_same_v<Invoke_t<OpType2, A, B>, std::false_type>);
static_assert(std::is_same_v<Invoke_t<OpType2, A, B, C>, std::true_type>);
static_assert(std::is_same_v<Invoke_t<OpType2, A, B, C, D>, std::false_type>);

// Applying an op to every individual element.
using OpPerElement = PerElement<Same<A>>;

static_assert(std::is_same_v<Invoke_t<OpPerElement, A, B>,
                             std::tuple<std::true_type, std::false_type>>);
static_assert(std::is_same_v<
              Invoke_t<OpPerElement, A, B, B>,
              std::tuple<std::true_type, std::false_type, std::false_type>>);
}  // namespace
