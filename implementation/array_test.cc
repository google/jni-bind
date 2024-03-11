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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Array;
using ::jni::ArrayStrip_t;
using ::jni::CDecl_t;
using ::jni::Class;
using ::jni::FullArrayStripV;
using ::jni::Rank;
using ::jni::Rankifier;
using ::jni::RegularToArrayTypeMap_t;

static constexpr Class kClass{"kClass"};
static constexpr Array arr1{jint{}};
static constexpr Array arr2{jfloat{}};
static constexpr Array arr3{jdouble{}};
static constexpr Array arr4{Class{"kClass"}};
static constexpr Array arr5{kClass};
static constexpr Array arr6{arr1};
static constexpr Array arr7{jint{}, Rank<2>{}};
static constexpr Array arr8{kClass, Rank<2>{}};
static constexpr Array arr9{kClass, Rank<3>{}};

////////////////////////////////////////////////////////////////////////////////
// Equality Tests.
////////////////////////////////////////////////////////////////////////////////
static_assert(arr1 == arr1);
static_assert(arr1 == arr1);
static_assert(arr1 != arr2);
static_assert(arr1 != arr2);
static_assert(arr1 != arr3);
static_assert(arr2 != arr3);
static_assert(arr6 == arr6);
static_assert(arr1 == arr6);
static_assert(arr6 == arr1);

// Classes.
static_assert(arr4 == arr4);
static_assert(arr4 == arr5);
static_assert(arr3 != arr4);

////////////////////////////////////////////////////////////////////////////////
// Rank Tests.
////////////////////////////////////////////////////////////////////////////////
static constexpr jint kArrRank0{0};
static constexpr Array kArrRank1{jint{}};
static constexpr Array kArrRank2{jint{}, Rank<2>{}};
static constexpr Array kArrRank3{jint{}, Rank<3>{}};
static constexpr Array kArrRank4{jint{}, Rank<4>{}};

using ArrRank0T = std::decay_t<decltype(kArrRank0)>;
using ArrRank1T = std::decay_t<decltype(kArrRank1)>;
using ArrRank2T = std::decay_t<decltype(kArrRank2)>;
using ArrRank3T = std::decay_t<decltype(kArrRank3)>;
using ArrRank4T = std::decay_t<decltype(kArrRank4)>;

// Surprisingly these do actually require some care with the CTAD guides.
static_assert(std::is_same_v<ArrRank0T, jint>);
static_assert(std::is_same_v<ArrRank1T, Array<jint, 1>>);
static_assert(std::is_same_v<ArrRank2T, Array<jint, 2>>);
static_assert(std::is_same_v<ArrRank3T, Array<jint, 3>>);
static_assert(std::is_same_v<ArrRank4T, Array<jint, 4>>);

static_assert(Rankifier::Rank(kArrRank0) == 0);
static_assert(Rankifier::Rank(kArrRank1) == 1);
static_assert(Rankifier::Rank(kArrRank2) == 2);
static_assert(Rankifier::Rank(kArrRank3) == 3);
static_assert(Rankifier::Rank(kArrRank4) == 4);

////////////////////////////////////////////////////////////////////////////////
// Strip Tests.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::is_same_v<ArrayStrip_t<ArrRank0T>, jint>);
static_assert(std::is_same_v<ArrayStrip_t<ArrRank1T>, jint>);
static_assert(std::is_same_v<ArrayStrip_t<ArrRank2T>, jint>);
static_assert(std::is_same_v<ArrayStrip_t<ArrRank3T>, jint>);

static_assert(jni::kIsArrayType<Array<int>>);

static_assert(jni::kIsArrayType<Array<int, 2>>);
static_assert(
    std::is_same_v<int, std::decay_t<decltype(FullArrayStripV(arr1))>>);
static_assert(
    std::is_same_v<int, std::decay_t<decltype(FullArrayStripV(arr6))>>);
static_assert(
    std::is_same_v<int, std::decay_t<decltype(FullArrayStripV(arr7))>>);

static_assert(FullArrayStripV(arr8) == kClass);
static_assert(FullArrayStripV(arr9) == kClass);


}  // namespace
