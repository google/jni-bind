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
#include "reduce_vals.h"

#include <algorithm>

#include "combine.h"
#include "concatenate.h"
#include "conditional.h"
#include "invoke.h"
#include "same.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using jni::metaprogramming::ReduceVals_v;

struct AddVal {
  template <auto v1, auto v2>
  static constexpr auto val = v1 + v2;
};

struct MaxVal {
  template <auto v1, auto v2>
  static constexpr auto val = std::max(v1, v2);
};

static_assert(ReduceVals_v<AddVal, 0> == 0);
static_assert(ReduceVals_v<AddVal, 1> == 1);
static_assert(ReduceVals_v<AddVal, 1, 2> == 3);
static_assert(ReduceVals_v<AddVal, 1, 2, 3> == 6);

static_assert(ReduceVals_v<MaxVal, 1> == 1);
static_assert(ReduceVals_v<MaxVal, 1, 2> == 2);
static_assert(ReduceVals_v<MaxVal, 1, 2, 3> == 3);

}  // namespace
