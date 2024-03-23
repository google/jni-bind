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
#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Array;
using ::jni::Class;
using ::jni::Fake;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Rank;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

static constexpr Class kClass2{"kClass2"};

////////////////////////////////////////////////////////////////////////////////
// As Return.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, V_2I) {
  static constexpr Class kClass{"kClass",
                                Method{"I", jni::Return{Array<jint, 2>{}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("()[[I")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("I");
}

TEST_F(JniTest, V_2LkClass) {
  static constexpr Class kClass{
      "kClass", Method{"Foo", jni::Return{Array{kClass2, Rank<2>{}}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("()[[LkClass2;")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("Foo");
}

////////////////////////////////////////////////////////////////////////////////
// As Params.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Complex: Arrays in Params & Return.
////////////////////////////////////////////////////////////////////////////////
static constexpr Class kArrClass{
    "ArrClass",
    Method{"Foo", jni::Return{Array{jint{}}}, Params{Array<jint, 2>{}}},
    Method{"Baz", jni::Return{Array{kClass2}}, Params{Array<jint, 3>{}}},
    Method{"Bar", jni::Return{Array{Class{"kClass3"}, Rank<2>{}}},
           Params{Array<jint, 3>{}, Array{double{}}}},
};

TEST_F(JniTest, 3I1D_2LkClass) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Bar"), StrEq("([[[I[D)[[LkClass3;")));

  LocalObject<kArrClass> obj{Fake<jobject>()};
  obj("Bar", Fake<jobjectArray>(), Fake<jdoubleArray>());
}

TEST_F(JniTest, 2I_I) {
  static constexpr Class kClass{
      "kClass", Method{"I", jni::Return<int>{}, Params{Array<jint, 2>{}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("([[I)I")));

  LocalObject<kClass> obj{Fake<jobject>()};
  // obj("I", jintArray{nullptr}); // doesn't compile (good).
  obj("I", Fake<jobjectArray>());
  obj("I", LocalArray<jint, 2>{Fake<jobjectArray>()});
}

TEST_F(JniTest, 3I_1LkClass) {
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("([[[I)[LkClass2;")));

  LocalObject<kArrClass> obj{Fake<jobject>()};

  // The compiler complains of unused variable here. This would be worth digging
  // into to understand the underlying cause (i.e. only the following fails).
  // LocalArray<jobject, 1, kClass2> ret = obj("Baz", jobjectArray{nullptr});

  // TODO(b/143908983): CTAD is failing.
  // LocalArray ret = obj("Baz", jobjectArray{nullptr});

  LocalArray<jobject, 1, kClass2> ret = obj("Baz", Fake<jobjectArray>());
  static_assert(std::is_same_v<decltype(ret), LocalArray<jobject, 1, kClass2>>);
}

TEST_F(JniTest, 3I1D_1LkClass) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Bar"), StrEq("([[[I[D)[[LkClass3;")));

  LocalObject<kArrClass> obj{Fake<jobject>()};
  obj("Bar", Fake<jobjectArray>(), Fake<jdoubleArray>());
}

}  // namespace
