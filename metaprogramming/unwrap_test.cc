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

#include "unwrap.h"

#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>
#include "invoke.h"
#include "same.h"

using jni::metaprogramming::Same;
using jni::metaprogramming::Unwrap_t;

namespace {

struct A {};
struct B {};
struct C {};

using Test = Same<A, B, C>;

static_assert(std::is_same_v<Unwrap_t<Test, std::tuple<A>>, std::false_type>);
static_assert(
    std::is_same_v<Unwrap_t<Test, std::tuple<A, B>>, std::false_type>);
static_assert(
    std::is_same_v<Unwrap_t<Test, std::tuple<A, B, C>>, std::true_type>);

}  // namespace
