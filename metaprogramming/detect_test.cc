/*
 * Copyright 2022 Google LLC
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
#include "metaprogramming/detect.h"

#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::jni::metaprogramming::Detect_t;
using ::jni::metaprogramming::Detect_v;

namespace {

struct A {};
struct B {};
struct C {};

template <typename T>
struct Container;

template <>
struct Container<int> {
  using type = std::tuple<int>;
};

template <>
struct Container<float> {
  using type = std::tuple<float, double>;
};

template <>
struct Container<A> {
  using type = std::tuple<A, B, C>;
};

static_assert(Detect_v<Container, int>);
static_assert(Detect_v<Container, float>);
static_assert(!Detect_v<Container, double>);

static_assert(std::is_same_v<Detect_t<Container, int>, std::tuple<int>>);
static_assert(
    std::is_same_v<Detect_t<Container, float>, std::tuple<float, double>>);
static_assert(std::is_same_v<Detect_t<Container, A>, std::tuple<A, B, C>>);

}  // namespace
