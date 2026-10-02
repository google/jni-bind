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

#include "tuple_from_size.h"

#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

namespace {

using ::jni::metaprogramming::TupleFromSize_t;

struct A {};

TEST(TupleFromSize, GeneratesEmptyTupleGivenNoArgs) {
  static_assert(std::is_same_v<TupleFromSize_t<int, 0>, std::tuple<>>);
  static_assert(std::is_same_v<TupleFromSize_t<float, 0>, std::tuple<>>);
  static_assert(std::is_same_v<TupleFromSize_t<double, 0>, std::tuple<>>);
  static_assert(std::is_same_v<TupleFromSize_t<A, 0>, std::tuple<>>);
}

TEST(TupleFromSize, GeneratesSingleTypeTupleForSize1) {
  static_assert(std::is_same_v<TupleFromSize_t<int, 1>, std::tuple<int>>);
  static_assert(std::is_same_v<TupleFromSize_t<float, 1>, std::tuple<float>>);
  static_assert(std::is_same_v<TupleFromSize_t<double, 1>, std::tuple<double>>);
  static_assert(std::is_same_v<TupleFromSize_t<A, 1>, std::tuple<A>>);
}

TEST(TupleFromSize, GeneratesSingleTypeTupleForSize2) {
  static_assert(std::is_same_v<TupleFromSize_t<int, 2>, std::tuple<int, int>>);
  static_assert(
      std::is_same_v<TupleFromSize_t<float, 2>, std::tuple<float, float>>);
  static_assert(
      std::is_same_v<TupleFromSize_t<double, 2>, std::tuple<double, double>>);
  static_assert(std::is_same_v<TupleFromSize_t<A, 2>, std::tuple<A, A>>);
}

}  // namespace
