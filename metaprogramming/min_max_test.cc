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
#include "min_max.h"

#include "reduce.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using jni::metaprogramming::Max;
using jni::metaprogramming::Max_t;
using jni::metaprogramming::Max_v;
using jni::metaprogramming::Min;
using jni::metaprogramming::Min_v;
using jni::metaprogramming::Reduce_t;
using jni::metaprogramming::ReduceAsPack_t;
using jni::metaprogramming::ReduceAsPack_v;
using jni::metaprogramming::Val_t;

static_assert(Max_v<Val_t<1>, Val_t<2>> == 2);
static_assert(Max_v<Val_t<2>, Val_t<1>> == 2);
static_assert(Max_v<Val_t<3>, Val_t<3>> == 3);

static_assert(Min_v<Val_t<1>, Val_t<2>> == 1);
static_assert(Min_v<Val_t<2>, Val_t<1>> == 1);
static_assert(Min_v<Val_t<3>, Val_t<3>> == 3);

static_assert(Reduce_t<Max, Val_t<1>>::val == 1);
static_assert(Reduce_t<Max, Val_t<1>, Val_t<2>>::val == 2);
static_assert(Reduce_t<Max, Val_t<1>, Val_t<2>, Val_t<3>>::val == 3);

static_assert(Reduce_t<Min, Val_t<1>>::val == 1);
static_assert(Reduce_t<Min, Val_t<1>, Val_t<2>>::val == 1);
static_assert(Reduce_t<Min, Val_t<1>, Val_t<2>, Val_t<3>>::val == 1);

static_assert(ReduceAsPack_t<Min, std::tuple<Val_t<1>>>::val == 1);
static_assert(ReduceAsPack_t<Min, std::tuple<Val_t<1>, Val_t<2>>>::val == 1);
static_assert(
    ReduceAsPack_t<Min, std::tuple<Val_t<1>, Val_t<2>, Val_t<3>>>::val == 1);

static_assert(ReduceAsPack_v<Min, std::tuple<Val_t<1>>> == 1);
static_assert(ReduceAsPack_v<Min, std::tuple<Val_t<1>, Val_t<2>>> == 1);
static_assert(ReduceAsPack_v<Min, std::tuple<Val_t<1>, Val_t<2>, Val_t<3>>> ==
              1);

static_assert(Max_t<Val_t<1>, Val_t<2>>::val == 2);

}  // namespace
