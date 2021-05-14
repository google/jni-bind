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

#include "unfurl.h"

#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "contains.h"

namespace {

using jni::metaprogramming::Contains_v;
using jni::metaprogramming::Unfurl_t;
using jni::metaprogramming::UnfurlConjunction_v;
using jni::metaprogramming::UnfurlDisjunction_v;

struct A {};
struct B {};
struct C {};

// Dummy metafunction, returns true iff |Ts...| is exactly B, C and I is 0.
template <std::size_t I, typename... Ts>
struct Func {
  static constexpr bool val =
      (I == 0) && std::is_same_v<std::tuple<Ts...>, std::tuple<B, C>>;
};

// No types forwarded.
static_assert(std::is_same_v<Unfurl_t<0, Func>, std::tuple<>>);
static_assert(std::is_same_v<Unfurl_t<1, Func>, std::tuple<Func<0>>>);
static_assert(std::is_same_v<Unfurl_t<2, Func>, std::tuple<Func<0>, Func<1>>>);
static_assert(
    std::is_same_v<Unfurl_t<3, Func>, std::tuple<Func<0>, Func<1>, Func<2>>>);

// One type forwarded (A).
static_assert(std::is_same_v<Unfurl_t<0, Func, A>, std::tuple<>>);
static_assert(std::is_same_v<Unfurl_t<1, Func, A>, std::tuple<Func<0, A>>>);
static_assert(
    std::is_same_v<Unfurl_t<2, Func, A>, std::tuple<Func<0, A>, Func<1, A>>>);
static_assert(std::is_same_v<Unfurl_t<3, Func, A>,
                             std::tuple<Func<0, A>, Func<1, A>, Func<2, A>>>);

// 3 types forwarded (A, B, C).
static_assert(std::is_same_v<Unfurl_t<0, Func, A, B, C>, std::tuple<>>);
static_assert(
    std::is_same_v<Unfurl_t<1, Func, A, B, C>, std::tuple<Func<0, A, B, C>>>);
static_assert(std::is_same_v<Unfurl_t<2, Func, A, B, C>,
                             std::tuple<Func<0, A, B, C>, Func<1, A, B, C>>>);
static_assert(std::is_same_v<Unfurl_t<3, Func, A, B, C>,
                             std::tuple<Func<0, A, B, C>, Func<1, A, B, C>,
                                        Func<2, A, B, C>>>);

////////////////////////////////////////////////////////////////////////////////
// Value tests (is index 0 and elements are {B, C}).
////////////////////////////////////////////////////////////////////////////////

// Zero size sequence (no unfurling happens).
// **WARNING** conjunctive and disjunctive folds respect the same rules as
// standard C++, namely, an empty pack in conjunction is true and vice-versa.
static_assert(UnfurlConjunction_v<0, Func>);
static_assert(UnfurlConjunction_v<0, Func, A>);
static_assert(UnfurlConjunction_v<0, Func, A, B>);
static_assert(UnfurlConjunction_v<0, Func, A, B, C>);

static_assert(!UnfurlDisjunction_v<0, Func>);
static_assert(!UnfurlDisjunction_v<0, Func, A>);
static_assert(!UnfurlDisjunction_v<0, Func, A, B>);
static_assert(!UnfurlDisjunction_v<0, Func, A, B, C>);

static_assert(!UnfurlConjunction_v<1, Func>);
static_assert(!UnfurlConjunction_v<2, Func>);
static_assert(!UnfurlConjunction_v<3, Func>);

static_assert(!UnfurlDisjunction_v<1, Func>);
static_assert(!UnfurlDisjunction_v<2, Func>);
static_assert(!UnfurlDisjunction_v<3, Func>);

// One type forwarded, size 1 sequence (no passes).
static_assert(!UnfurlConjunction_v<1, Func>);
static_assert(!UnfurlConjunction_v<1, Func, A>);
static_assert(!UnfurlConjunction_v<1, Func, B>);
static_assert(!UnfurlConjunction_v<1, Func, C>);

static_assert(!UnfurlDisjunction_v<1, Func>);
static_assert(!UnfurlDisjunction_v<1, Func, A>);
static_assert(!UnfurlDisjunction_v<1, Func, B>);
static_assert(!UnfurlDisjunction_v<1, Func, C>);

// Two types forwarded, size 1 sequence.
// (some passes, i.e. I == 0, Ts == {B, C}).
static_assert(!UnfurlConjunction_v<1, Func, A, A>);
static_assert(!UnfurlConjunction_v<1, Func, A, B>);
static_assert(UnfurlConjunction_v<1, Func, B, C>);  // pass! {0, B, C}
static_assert(!UnfurlConjunction_v<1, Func, A, C>);

static_assert(!UnfurlDisjunction_v<1, Func, A, A>);
static_assert(!UnfurlDisjunction_v<1, Func, A, B>);
static_assert(UnfurlDisjunction_v<1, Func, B, C>);  // pass! {0, B, C}
static_assert(!UnfurlDisjunction_v<1, Func, A, C>);

// Two types forwarded, size 1 sequence.
// (some passes, i.e. I == 0, Ts == {B, C}).
static_assert(!UnfurlConjunction_v<2, Func, A, A>);
static_assert(!UnfurlConjunction_v<2, Func, A, B>);
// fail! Func{0, B, C} && Func{1, B, C}
static_assert(!UnfurlConjunction_v<2, Func, B, C>);
static_assert(!UnfurlConjunction_v<2, Func, A, C>);

static_assert(!UnfurlDisjunction_v<2, Func, A, A>);
static_assert(!UnfurlDisjunction_v<2, Func, A, B>);
// pass! Func{0, B, C} || Func{1, B, C}
static_assert(UnfurlDisjunction_v<2, Func, B, C>);
static_assert(!UnfurlDisjunction_v<2, Func, A, C>);

}  // namespace
