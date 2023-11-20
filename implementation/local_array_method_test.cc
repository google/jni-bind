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

#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Array;
using ::jni::Class;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Rank;
using ::jni::RegularToArrayTypeMap_t;
using ::jni::test::Fake;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};
static constexpr Class kClass2{"kClass2"};

////////////////////////////////////////////////////////////////////////////////
// As Return.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ReturnSmokeTest) {
  static constexpr Class kClass{
      "kClass",
      Method{"BooleanArray", jni::Return{Array{jboolean{}}}, Params{}},
      Method{"ByteArray", jni::Return{Array{jbyte{}}}, Params{}},
      Method{"CharArray", jni::Return{Array{jchar{}}}, Params{}},
      Method{"ShortArray", jni::Return{Array{jshort{}}}, Params{}},
      Method{"IntArray", jni::Return{Array{jint{}}}, Params{}},
      Method{"FloatArray", jni::Return{Array{jfloat{}}}, Params{}},
      Method{"DoubleArray", jni::Return{Array{jdouble{}}}, Params{}},
      Method{"LongArray", jni::Return{Array{jlong{}}}, Params{}},
      Method{"ObjectArray", jni::Return{Array{kClass2}}, Params{}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("BooleanArray"), StrEq("()[Z")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ByteArray"), StrEq("()[B")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("CharArray"), StrEq("()[C")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ShortArray"), StrEq("()[S")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("()[I")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("FloatArray"), StrEq("()[F")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("DoubleArray"), StrEq("()[D")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("LongArray"), StrEq("()[J")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("()[LkClass2;")));
  obj("BooleanArray");
  obj("ByteArray");
  obj("CharArray");
  obj("ShortArray");
  obj("IntArray");
  obj("LongArray");
  obj("FloatArray");
  obj("DoubleArray");
  obj("ObjectArray");
}

}  // namespace
