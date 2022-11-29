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

#include "type_to_type_map.h"

#include <tuple>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};

using ::jni::metaprogramming::TypeToTypeMap;
using ::jni::metaprogramming::TypeToTypeMap_Keys_t;
using ::jni::metaprogramming::TypeToTypeMap_Values_t;
using ::jni::metaprogramming::TypeToTypeMapFromKeyValues_t;
using ::jni::metaprogramming::TypeToTypeMapQuery_t;

// Keys and values are expected.
using MapUnderTest = TypeToTypeMapFromKeyValues_t<A, B, C, D, E, F>;
static_assert(
    std::is_same_v<std::tuple<A, C, E>, TypeToTypeMap_Keys_t<MapUnderTest>>);
static_assert(
    std::is_same_v<std::tuple<B, D, F>, TypeToTypeMap_Values_t<MapUnderTest>>);

// Simple queries from basic non-trivial maps.
using TestMap1 = TypeToTypeMapFromKeyValues_t<A, B, C, D>;
static_assert(std::is_same_v<
              TestMap1, TypeToTypeMap<std::tuple<A, C>, std::tuple<B, D>>>);

static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap1, A>, B>);
static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap1, C>, D>);

// Simple queries from more complex non-trivial maps.
using TestMap2 = TypeToTypeMapFromKeyValues_t<A, B, C, D, E, F>;
static_assert(
    std::is_same_v<TestMap2,
                   TypeToTypeMap<std::tuple<A, C, E>, std::tuple<B, D, F>>>);

static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap2, A>, B>);
static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap2, C>, D>);
static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap2, E>, F>);

// Queries after inverting the map.
static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap1::Invert, B>, A>);

static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap2::Invert, B>, A>);
static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap2::Invert, D>, C>);
static_assert(std::is_same_v<TypeToTypeMapQuery_t<TestMap2::Invert, F>, E>);

}  // namespace
