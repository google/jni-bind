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

#include "contains.h"

#include <tuple>

namespace {

using jni::metaprogramming::Contains_v;
using jni::metaprogramming::ContainsValue;
using jni::metaprogramming::TupContains_v;

struct A {};

// Empty parameter packs.
static_assert(!Contains_v<int>);
static_assert(!Contains_v<void>);
static_assert(!Contains_v<float>);

static_assert(TupContains_v<int, std::tuple<int, float>>);
static_assert(TupContains_v<int, std::tuple<int, float, double>>);
static_assert(TupContains_v<int, std::tuple<int, float, double, A>>);

static_assert(!TupContains_v<int, std::tuple<float>>);
static_assert(!TupContains_v<int, std::tuple<float, double>>);
static_assert(!TupContains_v<int, std::tuple<float, double, A>>);

// Simple paramater packs.
static_assert(Contains_v<void, void>);
static_assert(Contains_v<void, int, void>);
static_assert(Contains_v<void, void, int>);
static_assert(Contains_v<void, int, void, int>);
static_assert(!Contains_v<void, int, int, int>);

static_assert(!Contains_v<int, void, void, float, double>);

static_assert(Contains_v<double, void, void, float, double>);

// Constexpr value tests.

// Testing for a value in empty set.
static_assert(!ContainsValue(1));

// Testing for values in non-empty set.
static_assert(ContainsValue(1, 1));
static_assert(ContainsValue(1, 1, 2));
static_assert(ContainsValue(1, 1, 2, 3));

static_assert(!ContainsValue(2, 1));
static_assert(ContainsValue(2, 1, 2));
static_assert(ContainsValue(2, 1, 2, 3));

static_assert(!ContainsValue(3, 1));
static_assert(!ContainsValue(3, 1, 2));
static_assert(ContainsValue(3, 1, 2, 3));

static_assert(!ContainsValue(4, 1));
static_assert(!ContainsValue(4, 1, 2));
static_assert(!ContainsValue(4, 1, 2, 3));

}  // namespace
