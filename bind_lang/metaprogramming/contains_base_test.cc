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

#include "contains_base.h"

#include "contains.h"

namespace {

using ::jni::metaprogramming::Contains_v;
using ::jni::metaprogramming::ContainsBase_v;

struct A {};
struct B : A {};
struct C : B {};

static_assert(ContainsBase_v<A, A>);
static_assert(ContainsBase_v<A, A, B>);
static_assert(ContainsBase_v<A, A, B, C>);

static_assert(!ContainsBase_v<B, A>);
static_assert(ContainsBase_v<B, A, B>);
static_assert(ContainsBase_v<B, A, B, C>);

static_assert(!ContainsBase_v<C, A>);
static_assert(!ContainsBase_v<C, A, B>);
static_assert(ContainsBase_v<C, A, B, C>);

// Test where ContainsBase is true, but Contains is not.
static_assert(ContainsBase_v<A, B>);
static_assert(!Contains_v<A, B>);

static_assert(ContainsBase_v<A, C>);
static_assert(!Contains_v<A, C>);

}  // namespace
