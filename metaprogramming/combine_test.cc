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

#include "combine.h"

#include <tuple>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using jni::metaprogramming::Combine_t;

namespace {

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};

static_assert(
    std::is_same_v<Combine_t<std::tuple<>, std::tuple<>>, std::tuple<>>);
static_assert(
    std::is_same_v<Combine_t<std::tuple<A>, std::tuple<>>, std::tuple<A>>);
static_assert(
    std::is_same_v<Combine_t<std::tuple<>, std::tuple<A>>, std::tuple<A>>);
static_assert(
    std::is_same_v<Combine_t<std::tuple<A>, std::tuple<B>>, std::tuple<A, B>>);
static_assert(std::is_same_v<Combine_t<std::tuple<A, B, C>, std::tuple<>>,
                             std::tuple<A, B, C>>);
static_assert(std::is_same_v<Combine_t<std::tuple<A>, std::tuple<B, C>>,
                             std::tuple<A, B, C>>);
static_assert(std::is_same_v<Combine_t<std::tuple<A>, std::tuple<B, C>>,
                             std::tuple<A, B, C>>);
static_assert(
    std::is_same_v<Combine_t<std::tuple<A, B, C>, std::tuple<D, E, F>>,
                   std::tuple<A, B, C, D, E, F>>);

}  // namespace
