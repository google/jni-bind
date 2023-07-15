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

#include "all.h"

#include <type_traits>

#include "invoke.h"
#include "same.h"
#include "unwrap.h"

using ::jni::metaprogramming::All;
using ::jni::metaprogramming::All_t;
using ::jni::metaprogramming::All_v;
using ::jni::metaprogramming::Invoke_t;
using ::jni::metaprogramming::Same;
using ::jni::metaprogramming::Unwrap;
using ::jni::metaprogramming::Unwrap_t;

namespace {

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};

static_assert(std::is_same_v<Invoke_t<All<Same<A>>, A>, std::true_type>);
static_assert(std::is_same_v<Invoke_t<All<Same<A>>, A, A>, std::true_type>);
static_assert(std::is_same_v<Invoke_t<All<Same<A>>, A, A, A>, std::true_type>);
static_assert(
    std::is_same_v<Invoke_t<All<Same<A>>, A, A, A, A>, std::true_type>);
static_assert(
    std::is_same_v<Invoke_t<All<Same<A>>, A, B, C, D>, std::false_type>);
static_assert(
    std::is_same_v<Invoke_t<All<Same<B>>, A, B, C, D>, std::false_type>);
static_assert(
    std::is_same_v<Invoke_t<All<Same<C>>, A, B, C, D>, std::false_type>);
static_assert(
    std::is_same_v<Invoke_t<All<Same<D>>, A, B, C, D>, std::false_type>);
static_assert(
    std::is_same_v<Invoke_t<All<Same<E>>, A, B, C, D>, std::false_type>);

static_assert(All_v<Same<A>, A>);
static_assert(All_v<Same<A>, A, A>);
static_assert(All_v<Same<A>, A, A, A>);
static_assert(All_v<Same<A>, A, A, A, A>);
static_assert(!All_v<Same<A>, A, B, C, D>);
static_assert(!All_v<Same<B>, A, B, C, D>);
static_assert(!All_v<Same<C>, A, B, C, D>);
static_assert(!All_v<Same<D>, A, B, C, D>);
static_assert(!All_v<Same<E>, A, B, C, D>);

static_assert(All_v<Unwrap<Same<E>>, std::tuple<E>>);
static_assert(All_v<Unwrap<Same<E>>, std::tuple<E>, std::tuple<E>>);
static_assert(
    All_v<Unwrap<Same<E>>, std::tuple<E>, std::tuple<E>, std::tuple<E>>);

static_assert(All_v<Unwrap<Same<E, E>>, std::tuple<E, E>>);
static_assert(All_v<Unwrap<Same<E, E>>, std::tuple<E, E>, std::tuple<E, E>>);
static_assert(All_v<Unwrap<Same<E, E>>, std::tuple<E, E>, std::tuple<E, E>,
                    std::tuple<E, E>>);

static_assert(All_v<Unwrap<Same<E, E, E>>, std::tuple<E, E, E>>);
static_assert(
    All_v<Unwrap<Same<E, E, E>>, std::tuple<E, E, E>, std::tuple<E, E, E>>);
static_assert(All_v<Unwrap<Same<E, E, E>>, std::tuple<E, E, E>,
                    std::tuple<E, E, E>, std::tuple<E, E, E>>);

static_assert(
    std::is_same_v<std::integral_constant<bool, true>, std::true_type>);
static_assert(
    std::is_same_v<Unwrap_t<Same<std::true_type>, std::tuple<std::true_type>>,
                   std::true_type>);

}  // namespace
