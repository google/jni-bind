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

#include "type_index_mask.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using jni::metaprogramming::TypeTupFromIndexMask_t;

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};
struct G {};

template <typename... Ts>
using Tup = std::tuple<Ts...>;

// Simple Masks.

// From the sequence of sets {{A}},  select the 0th element from the 0th item.
static_assert(std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A>>, 0>, Tup<A>>);
// From the sequence of sets {{A,B}},  select the 0th element from the 0th item.
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B>>, 0>, Tup<A>>);
// From the sequence of sets {{A,B}},  select the 1st element from the 1st item.
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B>>, 1>, Tup<B>>);

// Ternary Masks (following the same pattern as above).
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B, C>>, 0>, Tup<A>>);
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B, C>>, 1>, Tup<B>>);
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B, C>>, 2>, Tup<C>>);

// Multi-variate Masks.
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D>>, 0, 0>,
                   Tup<A, C>>);
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D>>, 0, 1>,
                   Tup<A, D>>);
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D>>, 1, 0>,
                   Tup<B, C>>);
static_assert(
    std::is_same_v<TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D>>, 1, 1>,
                   Tup<B, D>>);

// Multi-variate, non-symmetric sizes
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 0, 0>,
              Tup<A, C>>);
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 0, 1>,
              Tup<A, D>>);
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 0, 2>,
              Tup<A, E>>);
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 0, 3>,
              Tup<A, F>>);
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 0, 4>,
              Tup<A, G>>);

static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 1, 0>,
              Tup<B, C>>);
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 1, 1>,
              Tup<B, D>>);
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 1, 2>,
              Tup<B, E>>);
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 1, 3>,
              Tup<B, F>>);
static_assert(std::is_same_v<
              TypeTupFromIndexMask_t<Tup<Tup<A, B>, Tup<C, D, E, F, G>>, 1, 4>,
              Tup<B, G>>);
}  // namespace
