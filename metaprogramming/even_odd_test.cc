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

#include "even_odd.h"

#include <tuple>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using jni::metaprogramming::Even_t;
using jni::metaprogramming::Odd_t;

namespace {

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};

static_assert(std::is_same_v<Even_t<>, std::tuple<>>);
static_assert(std::is_same_v<Even_t<A>, std::tuple<A>>);
static_assert(std::is_same_v<Even_t<A, B>, std::tuple<A>>);
static_assert(std::is_same_v<Even_t<A, B, C>, std::tuple<A, C>>);
static_assert(std::is_same_v<Even_t<A, B, C, D>, std::tuple<A, C>>);
static_assert(std::is_same_v<Even_t<A, B, C, D, E>, std::tuple<A, C, E>>);
static_assert(std::is_same_v<Even_t<A, B, C, D, E, F>, std::tuple<A, C, E>>);

static_assert(std::is_same_v<Odd_t<>, std::tuple<>>);
static_assert(std::is_same_v<Odd_t<A>, std::tuple<>>);
static_assert(std::is_same_v<Odd_t<A, B>, std::tuple<B>>);
static_assert(std::is_same_v<Odd_t<A, B, C>, std::tuple<B>>);
static_assert(std::is_same_v<Odd_t<A, B, C, D>, std::tuple<B, D>>);
static_assert(std::is_same_v<Odd_t<A, B, C, D, E>, std::tuple<B, D>>);
static_assert(std::is_same_v<Odd_t<A, B, C, D, E, F>, std::tuple<B, D, F>>);

}  // namespace
