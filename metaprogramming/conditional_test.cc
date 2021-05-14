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

#include "conditional.h"

#include <tuple>

#include <gtest/gtest.h>
#include "same.h"

namespace {

using jni::metaprogramming::Conditional_t;
using jni::metaprogramming::Same;

struct A {};
struct B {};
struct C {};
struct D {};

// Basic conditional, using the type instead of value.
static_assert(std::is_same_v<Conditional_t<Same<A>, A>, std::tuple<A>>);
static_assert(std::is_same_v<Conditional_t<Same<A>, B>, std::tuple<>>);

static_assert(
    std::is_same_v<Conditional_t<Same<A, B, C>, A, B, C>, std::tuple<A, B, C>>);
static_assert(std::is_same_v<Conditional_t<Same<A, B, C>, A>, std::tuple<>>);

static_assert(std::is_same_v<Conditional_t<Same<A, B, C>, A>, std::tuple<>>);

}  // namespace
