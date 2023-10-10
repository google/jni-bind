/*
 * Copyright 2023 Google LLC
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
#include "metaprogramming/deep_equal.h"

#include <tuple>

namespace {

using ::jni::metaprogramming::DeepEqual_v;

struct A {
  constexpr explicit A(int i) : i_(i) {}
  const int i_;
  constexpr bool operator==(const A& rhs) const { return i_ == rhs.i_; }
};

template <typename... Ts>
struct Type {};

template <const auto... Vs>
struct Auto {};

template <const auto... Vs>
struct AutoRef {};

template <const auto&... Vs>
struct ConstAutoRef {};

////////////////////////////////////////////////////////////////////////////////
// Types.
////////////////////////////////////////////////////////////////////////////////
static_assert(DeepEqual_v<int, int>);
static_assert(!DeepEqual_v<int, float>);

static_assert(!DeepEqual_v<int, std::tuple<int>>);

static_assert(DeepEqual_v<std::tuple<int>, std::tuple<int>>);
static_assert(!DeepEqual_v<std::tuple<int>, std::tuple<float>>);
static_assert(!DeepEqual_v<std::tuple<int, int>, std::tuple<int>>);

static_assert(DeepEqual_v<std::tuple<Type<int>>, std::tuple<Type<int>>>);
static_assert(!DeepEqual_v<std::tuple<Type<int>>, std::tuple<std::tuple<int>>>);
static_assert(
    !DeepEqual_v<std::tuple<Type<int, int>>, std::tuple<std::tuple<int, int>>>);

////////////////////////////////////////////////////////////////////////////////
// Autos.
////////////////////////////////////////////////////////////////////////////////
static_assert(!DeepEqual_v<std::tuple<Type<int>>, std::tuple<Auto<'a'>>>);
static_assert(DeepEqual_v<std::tuple<Auto<'a'>>, std::tuple<Auto<'a'>>>);
static_assert(!DeepEqual_v<std::tuple<Auto<'a'>>, std::tuple<Auto<'b'>>>);
static_assert(!DeepEqual_v<std::tuple<Auto<'a'>>, std::tuple<Auto<'a', 'a'>>>);

static_assert(DeepEqual_v<Auto<1>, Auto<1>>);
static_assert(!DeepEqual_v<Auto<1>, Auto<2>>);
static_assert(!DeepEqual_v<Auto<1>, Auto<1, 1>>);

////////////////////////////////////////////////////////////////////////////////
// Autos Refs.
////////////////////////////////////////////////////////////////////////////////
const auto a = 'a';
const auto b = 'b';

static_assert(!DeepEqual_v<std::tuple<Type<int>>, std::tuple<AutoRef<a>>>);
static_assert(DeepEqual_v<std::tuple<AutoRef<a>>, std::tuple<AutoRef<a>>>);
static_assert(!DeepEqual_v<std::tuple<AutoRef<a>>, std::tuple<AutoRef<b>>>);
static_assert(!DeepEqual_v<std::tuple<AutoRef<a>>, std::tuple<AutoRef<a, a>>>);

static_assert(DeepEqual_v<AutoRef<a>, AutoRef<a>>);
static_assert(!DeepEqual_v<AutoRef<a>, AutoRef<b>>);
static_assert(!DeepEqual_v<AutoRef<a>, AutoRef<a, a>>);

////////////////////////////////////////////////////////////////////////////////
// Const Autos Refs.
////////////////////////////////////////////////////////////////////////////////
static_assert(!DeepEqual_v<std::tuple<Type<int>>, std::tuple<ConstAutoRef<a>>>);
static_assert(
    DeepEqual_v<std::tuple<ConstAutoRef<a>>, std::tuple<ConstAutoRef<a>>>);
static_assert(
    !DeepEqual_v<std::tuple<ConstAutoRef<a>>, std::tuple<ConstAutoRef<b>>>);
static_assert(
    !DeepEqual_v<std::tuple<ConstAutoRef<a>>, std::tuple<ConstAutoRef<a, a>>>);

static_assert(DeepEqual_v<ConstAutoRef<a>, ConstAutoRef<a>>);
static_assert(!DeepEqual_v<ConstAutoRef<a>, ConstAutoRef<b>>);
static_assert(!DeepEqual_v<ConstAutoRef<a>, ConstAutoRef<a, a>>);

}  // namespace
