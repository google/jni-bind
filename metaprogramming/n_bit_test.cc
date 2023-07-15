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

#include "n_bit.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "increment.h"

namespace {

using ::jni::metaprogramming::Increment_t;
using ::jni::metaprogramming::NBit;

// Unary
static_assert(NBit<0, 0>::value_ == 0);
static_assert(Increment_t<NBit<0, 0>>::value_ == 0);
static_assert(Increment_t<NBit<0, 0>>::value_ == 0);

// Binary
static_assert(NBit<0>::value_ == 0);
static_assert(Increment_t<NBit<0>, 0>::value_ == 0);
static_assert(Increment_t<NBit<0>>::value_ == 1);
static_assert(Increment_t<NBit<0>, 2>::value_ == 0);

static_assert(NBit<1>::value_ == 1);
static_assert(Increment_t<NBit<1>>::value_ == 0);
static_assert(Increment_t<NBit<1>, 2>::value_ == 1);

// Binary Overflow
static_assert(!NBit<0>::overflow_bit_);
static_assert(!Increment_t<NBit<0>>::overflow_bit_);
static_assert(Increment_t<NBit<0>, 2>::overflow_bit_);
static_assert(!Increment_t<NBit<0>, 2>::ResetOverflow::overflow_bit_);

static_assert(!NBit<1>::overflow_bit_);
static_assert(Increment_t<NBit<1>>::overflow_bit_);
static_assert(!Increment_t<NBit<1>, 2>::overflow_bit_);

// Ternary
static_assert(NBit<0, 2>::value_ == 0);
static_assert(Increment_t<NBit<0, 2>>::value_ == 1);
static_assert(Increment_t<NBit<0, 2>, 2>::value_ == 2);
static_assert(Increment_t<NBit<0, 2>, 3>::value_ == 0);

static_assert(NBit<1, 2>::value_ == 1);
static_assert(Increment_t<NBit<1, 2>, 1>::value_ == 2);
static_assert(Increment_t<NBit<1, 2>, 2>::value_ == 0);
static_assert(Increment_t<NBit<1, 2>, 3>::value_ == 1);

static_assert(NBit<2, 2>::value_ == 2);
static_assert(Increment_t<NBit<2, 2>, 1>::value_ == 0);
static_assert(Increment_t<NBit<2, 2>, 2>::value_ == 1);
static_assert(Increment_t<NBit<2, 2>, 3>::value_ == 2);

// Ternary Overflow
static_assert(!NBit<0, 2>::overflow_bit_);
static_assert(!Increment_t<NBit<0, 2>>::overflow_bit_);
static_assert(!Increment_t<NBit<0, 2>, 2>::overflow_bit_);
static_assert(Increment_t<NBit<0, 2>, 3>::overflow_bit_);

static_assert(!Increment_t<NBit<1, 2>>::overflow_bit_);
static_assert(Increment_t<NBit<1, 2>, 2>::overflow_bit_);
static_assert(!Increment_t<NBit<1, 2>, 3>::overflow_bit_);

static_assert(!NBit<2, 2>::overflow_bit_);
static_assert(Increment_t<NBit<2, 2>>::overflow_bit_);
static_assert(!Increment_t<NBit<2, 2>, 2>::overflow_bit_);
static_assert(!Increment_t<NBit<2, 2>, 3>::overflow_bit_);

}  // namespace
