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

#include "flatten.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using jni::metaprogramming::Flatten_t;

namespace {

struct A {};
struct B {};
struct C {};

// No arguments.
static_assert(std::is_same_v<Flatten_t<>, std::tuple<>>);

// Simple non tuple like arguments.
static_assert(std::is_same_v<Flatten_t<A>, std::tuple<A>>);
static_assert(std::is_same_v<Flatten_t<A, B>, std::tuple<A, B>>);

// Empty tuples flatten to an empty list.
static_assert(std::is_same_v<Flatten_t<std::tuple<>>, std::tuple<>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<>, std::tuple<>>, std::tuple<>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<>, std::tuple<>, std::tuple<>>,
                   std::tuple<>>);

// Single element flattened with empty tuples yields single argument.
static_assert(
    std::is_same_v<Flatten_t<std::tuple<A>, std::tuple<>>, std::tuple<A>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<>, std::tuple<A>, std::tuple<>>,
                   std::tuple<A>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<>, std::tuple<>, std::tuple<A>>,
                   std::tuple<A>>);

// Two elements flattened with empty tuples yields single argument.
static_assert(
    std::is_same_v<Flatten_t<std::tuple<A>, std::tuple<B>, std::tuple<>>,
                   std::tuple<A, B>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<A>, std::tuple<>, std::tuple<B>>,
                   std::tuple<A, B>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<>, std::tuple<A>, std::tuple<B>>,
                   std::tuple<A, B>>);

// Nested empty tuples flatten to a single empty tuple.
static_assert(
    std::is_same_v<Flatten_t<std::tuple<std::tuple<>>>, std::tuple<>>);
static_assert(std::is_same_v<Flatten_t<std::tuple<std::tuple<std::tuple<>>>>,
                             std::tuple<>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<std::tuple<std::tuple<std::tuple<>>>>>,
                   std::tuple<>>);

// Nested tuples with a single element flatten to the single element.
static_assert(
    std::is_same_v<Flatten_t<std::tuple<std::tuple<A>>>, std::tuple<A>>);
static_assert(std::is_same_v<Flatten_t<std::tuple<std::tuple<std::tuple<A>>>>,
                             std::tuple<A>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<std::tuple<std::tuple<std::tuple<A>>>>>,
                   std::tuple<A>>);

// Nested tuples with a two elements flatten to the two elements.
static_assert(
    std::is_same_v<Flatten_t<std::tuple<std::tuple<A, B>>>, std::tuple<A, B>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<std::tuple<std::tuple<A, B>>>>,
                   std::tuple<A, B>>);
static_assert(std::is_same_v<
              Flatten_t<std::tuple<std::tuple<std::tuple<std::tuple<A, B>>>>>,
              std::tuple<A, B>>);

// Various non-trivial cases flatten to what you'd expect.
static_assert(
    std::is_same_v<Flatten_t<std::tuple<std::tuple<std::tuple<std::tuple<A>>,
                                                   std::tuple<B>>>,
                             C>,
                   std::tuple<A, B, C>>);
static_assert(
    std::is_same_v<Flatten_t<std::tuple<std::tuple<std::tuple<std::tuple<A>>,
                                                   std::tuple<std::tuple<B>>>>,
                             C>,
                   std::tuple<A, B, C>>);
static_assert(
    std::is_same_v<Flatten_t<A, std::tuple<B>, std::tuple<std::tuple<C>>>,
                   std::tuple<A, B, C>>);

}  // namespace
