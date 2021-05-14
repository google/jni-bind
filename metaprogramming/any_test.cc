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

#include "any.h"

#include <type_traits>

#include "invoke.h"
#include "same.h"

using jni::metaprogramming::Any;
using jni::metaprogramming::Any_t;
using jni::metaprogramming::Any_v;
using jni::metaprogramming::Invoke_t;
using jni::metaprogramming::Same;

namespace {

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};

static_assert(
    std::is_same_v<Invoke_t<Any<Same<A>>, A, B, C, D>, std::true_type>);
static_assert(
    std::is_same_v<Invoke_t<Any<Same<B>>, A, B, C, D>, std::true_type>);
static_assert(
    std::is_same_v<Invoke_t<Any<Same<C>>, A, B, C, D>, std::true_type>);
static_assert(
    std::is_same_v<Invoke_t<Any<Same<D>>, A, B, C, D>, std::true_type>);
static_assert(
    std::is_same_v<Invoke_t<Any<Same<E>>, A, B, C, D>, std::false_type>);

static_assert(std::is_same_v<Any_t<Same<A>, A, B, C, D>, std::true_type>);
static_assert(std::is_same_v<Any_t<Same<B>, A, B, C, D>, std::true_type>);
static_assert(std::is_same_v<Any_t<Same<C>, A, B, C, D>, std::true_type>);
static_assert(std::is_same_v<Any_t<Same<D>, A, B, C, D>, std::true_type>);
static_assert(std::is_same_v<Any_t<Same<E>, A, B, C, D>, std::false_type>);

static_assert(Any_v<Same<A>, A, B, C, D>);
static_assert(Any_v<Same<B>, A, B, C, D>);
static_assert(Any_v<Same<C>, A, B, C, D>);
static_assert(Any_v<Same<D>, A, B, C, D>);
static_assert(!Any_v<Same<E>, A, B, C, D>);

}  // namespace
