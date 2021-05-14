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

#include "concatenate.h"

#include <tuple>
#include <type_traits>

using jni::metaprogramming::Concatenate;
using jni::metaprogramming::Concatenate_t;
using jni::metaprogramming::ConcatenateTup_t;

namespace {

// Empty packs.
static_assert(std::is_same_v<Concatenate_t<>, std::tuple<>>);

// Simple pack types.
static_assert(std::is_same_v<Concatenate_t<int, int>, std::tuple<int, int>>);
static_assert(
    std::is_same_v<Concatenate_t<int, float>, std::tuple<int, float>>);
static_assert(
    std::is_same_v<Concatenate_t<float, int>, std::tuple<float, int>>);

static_assert(
    std::is_same_v<Concatenate_t<float, float>, std::tuple<float, float>>);

// Simple variadic types.
static_assert(
    std::is_same_v<Concatenate_t<int, int, int>, std::tuple<int, int, int>>);
static_assert(std::is_same_v<Concatenate_t<int, float, double>,
                             std::tuple<int, float, double>>);
static_assert(std::is_same_v<Concatenate_t<double, float, int>,
                             std::tuple<double, float, int>>);
static_assert(std::is_same_v<Concatenate_t<int, float, double, long>,
                             std::tuple<int, float, double, long>>);

// Simple user defined aggregate types.
struct S1;
struct S2;

static_assert(std::is_same_v<Concatenate_t<S1, S2>, std::tuple<S1, S2>>);
static_assert(std::is_same_v<Concatenate_t<S2, S2>, std::tuple<S2, S2>>);
static_assert(std::is_same_v<Concatenate_t<S2, S1, int, int, S2, S2, S2>,
                             std::tuple<S2, S1, int, int, S2, S2, S2>>);

// User defined classes with no copy or move constructors
struct C1 {
  C1(const C1&) = delete;
  C1(const C1&&) = delete;
};

struct C2 {
  C2(const C2&) = delete;
  C2(const C2&&) = delete;
};

static_assert(std::is_same_v<Concatenate_t<C1>, std::tuple<C1>>);
static_assert(std::is_same_v<Concatenate_t<C1, C2>, std::tuple<C1, C2>>);
static_assert(std::is_same_v<Concatenate_t<C2, C2>, std::tuple<C2, C2>>);

////////////////////////////////////////////////////////////////////////////////
// Tuple form.
////////////////////////////////////////////////////////////////////////////////

// Empty packs.
static_assert(
    std::is_same_v<ConcatenateTup_t<std::tuple<>, std::tuple<>>, std::tuple<>>);
static_assert(
    std::is_same_v<ConcatenateTup_t<std::tuple<>, std::tuple<>, std::tuple<>>,
                   std::tuple<>>);

// Simple pack types.
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<int>, std::tuple<int>>,
                             std::tuple<int, int>>);
static_assert(
    std::is_same_v<ConcatenateTup_t<std::tuple<int>, std::tuple<float>>,
                   std::tuple<int, float>>);
static_assert(
    std::is_same_v<ConcatenateTup_t<std::tuple<float>, std::tuple<int>>,
                   std::tuple<float, int>>);
static_assert(
    std::is_same_v<ConcatenateTup_t<std::tuple<float>, std::tuple<float>>,
                   std::tuple<float, float>>);

// Simple variadic types.
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<int>, std::tuple<int>,
                                              std::tuple<int>>,
                             std::tuple<int, int, int>>);
static_assert(
    std::is_same_v<ConcatenateTup_t<std::tuple<int>, std::tuple<float>,
                                    std::tuple<double>>,
                   std::tuple<int, float, double>>);
static_assert(
    std::is_same_v<ConcatenateTup_t<std::tuple<double>, std::tuple<float>,
                                    std::tuple<int>>,
                   std::tuple<double, float, int>>);
static_assert(
    std::is_same_v<ConcatenateTup_t<std::tuple<int>, std::tuple<float>,
                                    std::tuple<double>, std::tuple<long>>,
                   std::tuple<int, float, double, long>>);

// Simple user defined aggregate types.
struct S1;
struct S2;

// User defined classes with no copy or move constructors
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<S1>, std::tuple<>>,
                             std::tuple<S1>>);
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<S1>, std::tuple<S2>>,
                             std::tuple<S1, S2>>);
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<S2>, std::tuple<S2>>,
                             std::tuple<S2, S2>>);
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<S2, S1, int, int, S2>,
                                              std::tuple<S2, S2>>,
                             std::tuple<S2, S1, int, int, S2, S2, S2>>);

static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<C1>, std::tuple<>>,
                             std::tuple<C1>>);
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<C1>, std::tuple<C2>>,
                             std::tuple<C1, C2>>);
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<C2>, std::tuple<C2>>,
                             std::tuple<C2, C2>>);
static_assert(std::is_same_v<ConcatenateTup_t<std::tuple<C2, C1, int, int, C2>,
                                              std::tuple<C2, C2>>,
                             std::tuple<C2, C1, int, int, C2, C2, C2>>);

}  // namespace
