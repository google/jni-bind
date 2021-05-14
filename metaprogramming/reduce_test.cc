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

#include "reduce.h"

#include "combine.h"
#include "concatenate.h"
#include "conditional.h"
#include "invoke.h"
#include "same.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using jni::metaprogramming::Combine;
using jni::metaprogramming::Combine_t;
using jni::metaprogramming::Reduce_t;

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};
struct G {};
struct H {};

// Empty List (also empty).
static_assert(std::is_same_v<Reduce_t<Combine, std::tuple<>>, std::tuple<>>);

// Single element list (arbitrarily selected as the input type).
static_assert(std::is_same_v<Reduce_t<Combine, std::tuple<A>>, std::tuple<A>>);

// Non-zero size lists.
static_assert(
    std::is_same_v<Combine_t<std::tuple<A>, std::tuple<B>>, std::tuple<A, B>>);

static_assert(
    std::is_same_v<Combine_t<std::tuple<A>, std::tuple<B>>, std::tuple<A, B>>);

static_assert(std::is_same_v<Reduce_t<Combine, std::tuple<A>, std::tuple<B>>,
                             std::tuple<A, B>>);
static_assert(std::is_same_v<
              Reduce_t<Combine, std::tuple<A>, std::tuple<B>, std::tuple<C>>,
              std::tuple<A, B, C>>);

static_assert(std::is_same_v<Reduce_t<Combine, std::tuple<A, B, C>,
                                      std::tuple<D, E>, std::tuple<F, G, H>>,
                             std::tuple<A, B, C, D, E, F, G, H>>);

}  // namespace
