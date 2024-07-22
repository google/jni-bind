/*
 * Copyright 2023 Google LLC
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

#include "metaprogramming/lambda_compatible.h"

#include <tuple>

#include "metaprogramming/vals.h"

using ::jni::metaprogramming::LambdaCompatible_v;
using ::jni::metaprogramming::Vals;

namespace {

constexpr auto l1{[]() { return std::tuple{'a'}; }};
constexpr auto l1_prime{[]() { return std::tuple{'a'}; }};
constexpr auto l2{[]() { return std::tuple{'a', 'a', 'a'}; }};

static_assert(LambdaCompatible_v<l1, l1>);
static_assert(LambdaCompatible_v<l1, l1_prime>);
static_assert(!LambdaCompatible_v<l1, l2>);
static_assert(LambdaCompatible_v<l2, l2>);
static_assert(!LambdaCompatible_v<l2, l1>);

// Deliberately does *not* have constexpr construction.
struct A {
  A(int a) : a_(a) {}

  int a_;
};

constexpr auto l3{[]() { return std::tuple{A{1}}; }};
constexpr auto l3_prime{[]() { return std::tuple{A{1}}; }};
constexpr auto l3_prime_prime{[]() { return std::tuple{A{9999}}; }};

// Note: Value portion of A cannot be compared, so l3 == l3''.
static_assert(LambdaCompatible_v<l3, l3>);
static_assert(LambdaCompatible_v<l3, l3_prime>);
static_assert(LambdaCompatible_v<l3, l3_prime_prime>);
static_assert(!LambdaCompatible_v<l3, l1>);

// Non-type template arguments compare fine.
constexpr auto l4{[]() { return std::tuple{Vals<'a'>{}}; }};
constexpr auto l4_prime{[]() { return std::tuple{Vals<'a'>{}}; }};
constexpr auto l4_prime_prime{
    []() { return std::tuple{Vals<'a', 'a', 'a'>{}}; }};
constexpr auto l4b_prime_prime{
    []() { return std::tuple{Vals<'b', 'b', 'b'>{}}; }};

static_assert(LambdaCompatible_v<l4, l4>);
static_assert(LambdaCompatible_v<l4, l4_prime>);
static_assert(LambdaCompatible_v<l4_prime, l4_prime>);
static_assert(LambdaCompatible_v<l4_prime, l4>);
static_assert(LambdaCompatible_v<l4_prime_prime, l4_prime_prime>);
static_assert(!LambdaCompatible_v<l4_prime_prime, l4b_prime_prime>);

static_assert(!LambdaCompatible_v<l4, l1>);
static_assert(!LambdaCompatible_v<l1, l4>);

}  // namespace
