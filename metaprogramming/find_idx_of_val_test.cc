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

#include "find_idx_of_val.h"

#include <tuple>
#include <type_traits>

using ::jni::metaprogramming::FindIdxOfVal_idx;
using ::jni::metaprogramming::FindIdxOfValInTup_idx;
using ::jni::metaprogramming::FindIdxOfValInTupWithComparator_idx;
using ::jni::metaprogramming::FindIdxOfValWithComparator_idx;

namespace {

struct A {};
struct B {};
struct C {};

static_assert(FindIdxOfVal_idx<A, A, B, C> == 0);
static_assert(FindIdxOfVal_idx<B, A, B, C> == 1);
static_assert(FindIdxOfVal_idx<C, A, B, C> == 2);

static_assert(FindIdxOfValInTup_idx<A, std::tuple<A, B, C>> == 0);
static_assert(FindIdxOfValInTup_idx<B, std::tuple<A, B, C>> == 1);
static_assert(FindIdxOfValInTup_idx<C, std::tuple<A, B, C>> == 2);

// Doesn't compile.
// static_assert(FindIdxOfVal_idx<C, A, B, C, A> == -1);

// Non-standard comparators.
struct ConvertsToFloat {
  template <typename T>
  static constexpr bool value = std::is_convertible<T, float>::value;
};

static_assert(FindIdxOfValWithComparator_idx<ConvertsToFloat, float> == 0);
static_assert(FindIdxOfValWithComparator_idx<ConvertsToFloat, A, float> == 1);
static_assert(FindIdxOfValWithComparator_idx<ConvertsToFloat, A, B, float> ==
              2);
static_assert(FindIdxOfValWithComparator_idx<ConvertsToFloat, A, B, C, float> ==
              3);

}  // namespace
