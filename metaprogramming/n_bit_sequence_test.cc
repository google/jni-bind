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

#include "n_bit_sequence.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "increment.h"
#include "n_bit.h"

namespace {

using jni::metaprogramming::Increment_t;
using jni::metaprogramming::NBit;
using jni::metaprogramming::NBitSequence;

// 1 binary value.
using T1 = NBitSequence<NBit<0, 1>>;
static_assert(std::is_same_v<Increment_t<T1>, NBitSequence<NBit<1, 1>>>);

// 2 binary values.
using T2 = NBitSequence<NBit<0, 1>, NBit<0, 1>>;
static_assert(
    std::is_same_v<Increment_t<T2, 1>, NBitSequence<NBit<0, 1>, NBit<1, 1>>>);
static_assert(std::is_same_v<Increment_t<T2, 2>,
                             NBitSequence<NBit<1, 1>, NBit<0, 1, true>>>);
static_assert(
    std::is_same_v<Increment_t<T2, 3>, NBitSequence<NBit<1, 1>, NBit<1, 1>>>);
static_assert(std::is_same_v<Increment_t<T2, 4>,
                             NBitSequence<NBit<0, 1, true>, NBit<0, 1, true>>>);

// 3 binary values.
using T3 = NBitSequence<NBit<0, 1>, NBit<0, 1>, NBit<0, 1>>;
static_assert(std::is_same_v<Increment_t<T3, 1>,
                             NBitSequence<NBit<0, 1>, NBit<0, 1>, NBit<1, 1>>>);
static_assert(
    std::is_same_v<Increment_t<T3, 2>,
                   NBitSequence<NBit<0, 1>, NBit<1, 1>, NBit<0, 1, true>>>);
static_assert(std::is_same_v<Increment_t<T3, 3>,
                             NBitSequence<NBit<0, 1>, NBit<1, 1>, NBit<1, 1>>>);
static_assert(std::is_same_v<
              Increment_t<T3, 4>,
              NBitSequence<NBit<1, 1>, NBit<0, 1, true>, NBit<0, 1, true>>>);
static_assert(std::is_same_v<Increment_t<T3, 5>,
                             NBitSequence<NBit<1, 1>, NBit<0, 1>, NBit<1, 1>>>);
static_assert(
    std::is_same_v<Increment_t<T3, 6>,
                   NBitSequence<NBit<1, 1>, NBit<1, 1>, NBit<0, 1, true>>>);
static_assert(std::is_same_v<Increment_t<T3, 7>,
                             NBitSequence<NBit<1, 1>, NBit<1, 1>, NBit<1, 1>>>);
static_assert(std::is_same_v<Increment_t<T3, 8>,
                             NBitSequence<NBit<0, 1, true>, NBit<0, 1, true>,
                                          NBit<0, 1, true>>>);

// 4 Binary values.
using T4 = NBitSequence<NBit<0, 1>, NBit<0, 1>, NBit<0, 1>, NBit<0, 1>>;
static_assert(T4::same_<0, 0, 0, 0>);
static_assert(Increment_t<T4, 1>::same_<0, 0, 0, 1>);
static_assert(Increment_t<T4, 2>::same_<0, 0, 1, 0>);
static_assert(Increment_t<T4, 3>::same_<0, 0, 1, 1>);
static_assert(Increment_t<T4, 4>::same_<0, 1, 0, 0>);
static_assert(Increment_t<T4, 5>::same_<0, 1, 0, 1>);
static_assert(Increment_t<T4, 6>::same_<0, 1, 1, 0>);
static_assert(Increment_t<T4, 7>::same_<0, 1, 1, 1>);
static_assert(Increment_t<T4, 8>::same_<1, 0, 0, 0>);
static_assert(Increment_t<T4, 9>::same_<1, 0, 0, 1>);
static_assert(Increment_t<T4, 10>::same_<1, 0, 1, 0>);
static_assert(Increment_t<T4, 11>::same_<1, 0, 1, 1>);
static_assert(Increment_t<T4, 12>::same_<1, 1, 0, 0>);
static_assert(Increment_t<T4, 13>::same_<1, 1, 0, 1>);
static_assert(Increment_t<T4, 14>::same_<1, 1, 1, 0>);
static_assert(Increment_t<T4, 15>::same_<1, 1, 1, 1>);

// Values past here just cycle (2^4 = 16, so 0 cycles every 16).
static_assert(Increment_t<T4, 16>::same_<0, 0, 0, 0>);
static_assert(Increment_t<T4, 17>::same_<0, 0, 0, 1>);
static_assert(Increment_t<T4, 32>::same_<0, 0, 0, 0>);
static_assert(Increment_t<T4, 48>::same_<0, 0, 0, 0>);
static_assert(Increment_t<T4, 64>::same_<0, 0, 0, 0>);

// One quadernary, one ternary, one binary value.
using T5 = NBitSequence<NBit<0, 3>, NBit<0, 2>, NBit<0, 1>>;
static_assert(T5::same_<0, 0, 0>);
static_assert(Increment_t<T5, 1>::same_<0, 0, 1>);
static_assert(Increment_t<T5, 2>::same_<0, 1, 0>);
static_assert(Increment_t<T5, 3>::same_<0, 1, 1>);
static_assert(Increment_t<T5, 4>::same_<0, 2, 0>);
static_assert(Increment_t<T5, 5>::same_<0, 2, 1>);
static_assert(Increment_t<T5, 6>::same_<1, 0, 0>);
static_assert(Increment_t<T5, 7>::same_<1, 0, 1>);
static_assert(Increment_t<T5, 8>::same_<1, 1, 0>);
static_assert(Increment_t<T5, 9>::same_<1, 1, 1>);
static_assert(Increment_t<T5, 10>::same_<1, 2, 0>);
static_assert(Increment_t<T5, 11>::same_<1, 2, 1>);
static_assert(Increment_t<T5, 12>::same_<2, 0, 0>);
static_assert(Increment_t<T5, 13>::same_<2, 0, 1>);
static_assert(Increment_t<T5, 14>::same_<2, 1, 0>);
static_assert(Increment_t<T5, 15>::same_<2, 1, 1>);
static_assert(Increment_t<T5, 16>::same_<2, 2, 0>);
static_assert(Increment_t<T5, 17>::same_<2, 2, 1>);
static_assert(Increment_t<T5, 18>::same_<3, 0, 0>);
static_assert(Increment_t<T5, 19>::same_<3, 0, 1>);
static_assert(Increment_t<T5, 20>::same_<3, 1, 0>);
static_assert(Increment_t<T5, 21>::same_<3, 1, 1>);
static_assert(Increment_t<T5, 22>::same_<3, 2, 0>);
static_assert(Increment_t<T5, 23>::same_<3, 2, 1>);
static_assert(Increment_t<T5, 24>::same_<0, 0, 0>);
static_assert(Increment_t<T5, 48>::same_<0, 0, 0>);
static_assert(Increment_t<T5, 72>::same_<0, 0, 0>);

}  // namespace
