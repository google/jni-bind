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
#include "implementation/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptGlobal;
using ::jni::AdoptLocal;
using ::jni::Array;
using ::jni::CDecl_t;
using ::jni::Class;
using ::jni::Field;
using ::jni::GlobalObject;
using ::jni::kJavaLangString;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::Method;
using ::jni::Params;
using ::jni::test::AsNewLocalReference;
using ::jni::test::Fake;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};

////////////////////////////////////////////////////////////////////////////////
// Convertability.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, IsImplicitlyConvertibleToSpanType) {
  EXPECT_EQ(
      static_cast<jintArray>(LocalArray<jint>{AdoptLocal{}, Fake<jintArray>()}),
      Fake<jintArray>());
}

////////////////////////////////////////////////////////////////////////////////
// Construction Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, BuildsAndDestroys) {
  EXPECT_CALL(*env_, NewIntArray(1)).WillOnce(Return(Fake<jintArray>()));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>()));

  LocalArray<jint> int_array_1{1};
}

TEST_F(JniTest, MakesNewReferenceByDefault) {
  EXPECT_EQ(static_cast<jintArray>(LocalArray<jint>{Fake<jintArray>()}),
            AsNewLocalReference(Fake<jintArray>()));
}

TEST_F(JniTest, ConstructsIntArrayWithCorrectSize) {
  EXPECT_CALL(*env_, NewIntArray(1)).WillOnce(Return(Fake<jintArray>(1)));
  EXPECT_CALL(*env_, NewIntArray(2)).WillOnce(Return(Fake<jintArray>(2)));
  EXPECT_CALL(*env_, NewIntArray(3)).WillOnce(Return(Fake<jintArray>(3)));
  EXPECT_CALL(*env_, NewIntArray(4)).WillOnce(Return(Fake<jintArray>(4)));

  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(1)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(3)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(4)));

  LocalArray<jint> int_array_1{1};
  LocalArray<jint> int_array_2{2};
  LocalArray<jint> int_array_3{3};
  LocalArray<jint> int_array_4{4};
}

TEST_F(JniTest, ConstructsTheRightType) {
  EXPECT_CALL(*env_, NewBooleanArray(1));
  EXPECT_CALL(*env_, NewByteArray(2));
  EXPECT_CALL(*env_, NewCharArray(3));
  EXPECT_CALL(*env_, NewShortArray(4));
  EXPECT_CALL(*env_, NewIntArray(5));
  EXPECT_CALL(*env_, NewLongArray(6));
  EXPECT_CALL(*env_, NewFloatArray(7));
  EXPECT_CALL(*env_, NewDoubleArray(8));

  LocalArray<jboolean> boolean_array{1};
  LocalArray<jbyte> byte_array{2};
  LocalArray<jchar> char_array{3};
  LocalArray<jshort> short_array{4};
  LocalArray<jint> int_array{5};
  LocalArray<jlong> long_array{6};
  LocalArray<jfloat> float_array{7};
  LocalArray<jdouble> double_array{8};
}

TEST_F(JniTest, ConstructsAnArrayOfNullValues) {
  EXPECT_CALL(*env_, NewObjectArray(5, _, nullptr));
  EXPECT_CALL(*env_, FindClass(StrEq("kClass")));
  LocalArray<jobject, 1, kClass> local_object_array{5};
}

TEST_F(JniTest, ConstructsObjectsForLValues) {
  // Unlike POD, objects are constructed with a size, a jclass, and an init
  // object.  This makes for a slightly different API then other objects.
  EXPECT_CALL(*env_, NewObjectArray(5, _, Fake<jobject>()));

  LocalObject<kClass> default_object{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jobject, 1, kClass> local_object_array{5, default_object};
}

TEST_F(JniTest, ConstructsTheRightTypeForRValues) {
  EXPECT_CALL(*env_, NewObjectArray(5, _, _)).Times(2);

  LocalObject<kClass> default_object{};
  LocalArray<jobject, 1, kClass> local_object_array_1{5, LocalObject<kClass>{}};
  LocalArray<jobject, 1, kClass> local_object_array_2{
      5, std::move(default_object)};
}

TEST_F(JniTest, ConstructsBooleanArray) {
  EXPECT_CALL(*env_, NewBooleanArray(1))
      .WillOnce(::testing::Return(Fake<jbooleanArray>()));
  EXPECT_CALL(*env_, DeleteLocalRef(_));

  LocalArray<jboolean> boolean_array_1{1};
}

TEST_F(JniTest, ConstructsByteArray) {
  EXPECT_CALL(*env_, NewByteArray(1))
      .WillOnce(::testing::Return(Fake<jbyteArray>()));
  EXPECT_CALL(*env_, DeleteLocalRef(_));

  LocalArray<jbyte> byte_array_1{1};
}

TEST_F(JniTest, ConstructsIntArray) {
  EXPECT_CALL(*env_, NewIntArray(1))
      .WillOnce(::testing::Return(Fake<jintArray>()));
  EXPECT_CALL(*env_, DeleteLocalRef(_));

  LocalArray<jint> int_array_1{1};
}

TEST_F(JniTest, ConstructsFromRValue) {
  LocalArray<jint> arr{jintArray{nullptr}};
  LocalArray<jint> arr2{std::move(arr)};
}

}  // namespace
