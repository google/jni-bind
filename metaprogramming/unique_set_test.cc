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

#include "unique_set.h"

#include <utility>

namespace {

using jni::metaprogramming::UniqueSet_t;

struct A {};
struct B {};
struct C {};

// No duplicates
static_assert(std::is_same_v<UniqueSet_t<>, std::tuple<>>);
static_assert(std::is_same_v<UniqueSet_t<A>, std::tuple<A>>);
static_assert(std::is_same_v<UniqueSet_t<A, B>, std::tuple<A, B>>);
static_assert(std::is_same_v<UniqueSet_t<A, B, C>, std::tuple<A, B, C>>);

// One duplicate (note order is preserved).
static_assert(std::is_same_v<UniqueSet_t<A, B, A>, std::tuple<A, B>>);
static_assert(std::is_same_v<UniqueSet_t<A, B, A, A>, std::tuple<A, B>>);
static_assert(std::is_same_v<UniqueSet_t<A, B, A, A, A>, std::tuple<A, B>>);

// Two duplicates.
static_assert(std::is_same_v<UniqueSet_t<A, B, B>, std::tuple<A, B>>);
static_assert(std::is_same_v<UniqueSet_t<A, B, B, A>, std::tuple<A, B>>);
static_assert(std::is_same_v<UniqueSet_t<A, B, A, A, B>, std::tuple<A, B>>);

// Three duplicates.
static_assert(std::is_same_v<UniqueSet_t<A, B, C>, std::tuple<A, B, C>>);
static_assert(std::is_same_v<UniqueSet_t<A, B, C, C>, std::tuple<A, B, C>>);
static_assert(std::is_same_v<UniqueSet_t<A, A, B, C, C>, std::tuple<A, B, C>>);
static_assert(
    std::is_same_v<UniqueSet_t<A, A, B, C, C, C>, std::tuple<A, B, C>>);
static_assert(
    std::is_same_v<UniqueSet_t<A, A, B, B, C, C>, std::tuple<A, B, C>>);
static_assert(
    std::is_same_v<UniqueSet_t<A, A, B, B, C, C>, std::tuple<A, B, C>>);
static_assert(
    std::is_same_v<UniqueSet_t<A, A, B, B, C, C>, std::tuple<A, B, C>>);
static_assert(std::is_same_v<UniqueSet_t<A, A, B, B, C, C, A, B, C, C, B, A>,
                             std::tuple<A, B, C>>);

}  // namespace
