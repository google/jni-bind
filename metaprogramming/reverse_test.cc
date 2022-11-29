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

#include "reverse.h"

#include <tuple>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

struct A {};
struct B {};
struct C {};
struct D {};

using jni::metaprogramming::Reverse_t;
using jni::metaprogramming::Reverse_Tup;

static_assert(std::is_same_v<Reverse_t<>, std::tuple<>>);
static_assert(std::is_same_v<Reverse_t<A>, std::tuple<A>>);
static_assert(std::is_same_v<Reverse_t<A, B>, std::tuple<B, A>>);
static_assert(std::is_same_v<Reverse_t<A, B, C>, std::tuple<C, B, A>>);
static_assert(std::is_same_v<Reverse_t<A, B, C, D>, std::tuple<D, C, B, A>>);
static_assert(
    std::is_same_v<Reverse_t<A, A, B, C, D>, std::tuple<D, C, B, A, A>>);

static_assert(std::is_same_v<Reverse_Tup<Reverse_t<A, A, B, C, D>>,
                             std::tuple<A, A, B, C, D>>);

}  // namespace
