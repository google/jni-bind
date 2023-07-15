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

#include "filter.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "invoke.h"
#include "same.h"

namespace {

using ::jni::metaprogramming::Filter_t;
using ::jni::metaprogramming::Same;

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};

static_assert(std::is_same_v<Filter_t<Same<A>, A>, std::tuple<A>>);
static_assert(std::is_same_v<Filter_t<Same<A>, B>, std::tuple<>>);

static_assert(std::is_same_v<Filter_t<Same<A>, A, B>, std::tuple<A>>);
static_assert(std::is_same_v<Filter_t<Same<A>, A, B, C, D, E>, std::tuple<A>>);
static_assert(std::is_same_v<Filter_t<Same<B>, A, B, C, D, E>, std::tuple<B>>);

}  // namespace
