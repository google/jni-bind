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
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};

////////////////////////////////////////////////////////////////////////////////
// Construction Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, LocalArray_BuildsAndDestroys) {
  EXPECT_CALL(*env_, NewIntArray(1)).WillOnce(Return(FakeJIntArray()));
  EXPECT_CALL(*env_, DeleteLocalRef(_));

  LocalArray<jint> int_array_1{1};
}

TEST_F(JniTest, LocalArray_IsImplicitlyConvertibleToSpanType) {
  EXPECT_EQ(static_cast<jintArray>(
                LocalArray<jint>{reinterpret_cast<jintArray>(0xAAAA)}),
            reinterpret_cast<jintArray>(0xAAAA));
}

TEST_F(JniTest, LocalArray_ConstructsIntArrayWithCorrectSize) {
  EXPECT_CALL(*env_, NewIntArray(1)).WillOnce(Return(FakeJIntArray()));
  EXPECT_CALL(*env_, NewIntArray(2)).WillOnce(Return(FakeJIntArray()));
  EXPECT_CALL(*env_, NewIntArray(3)).WillOnce(Return(FakeJIntArray()));
  EXPECT_CALL(*env_, NewIntArray(4)).WillOnce(Return(FakeJIntArray()));

  EXPECT_CALL(*env_, DeleteLocalRef(FakeJIntArray())).Times(4);

  LocalArray<jint> int_array_1{1};
  LocalArray<jint> int_array_2{2};
  LocalArray<jint> int_array_3{3};
  LocalArray<jint> int_array_4{4};
}

TEST_F(JniTest, LocalArray_ConstructsTheRightType) {
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

TEST_F(JniTest, LocalArray_ConstructsObjectsForLValues) {
  // Unlike POD, objects are constructed with a size, a jclass, and an init
  // object.  This makes for a slightly different API then other objects.
  EXPECT_CALL(*env_, NewObjectArray(5, _, _));

  LocalObject<kClass> default_object{};
  LocalArray<jobject, 1, kClass> local_object_array{5, default_object};
}

// Same as above for jstring.
TEST_F(JniTest, LocalStringArray_ConstructsObjectsForLValues) {
  // Unlike POD, objects are constructed with a size, a jclass, and an init
  // object.  This makes for a slightly different API then other objects.
  EXPECT_CALL(*env_, NewObjectArray);

  LocalObject<kClass> default_object{};
  LocalArray<jobject, 1, kClass> local_object_array{5, default_object};
}

TEST_F(JniTest, LocalArray_ConstructsTheRightTypeForRValues) {
  EXPECT_CALL(*env_, NewObjectArray(5, _, _)).Times(2);

  LocalObject<kClass> default_object{};
  LocalArray<jobject, 1, kClass> local_object_array_1{5, LocalObject<kClass>{}};
  LocalArray<jobject, 1, kClass> local_object_array_2{
      5, std::move(default_object)};
}

////////////////////////////////////////////////////////////////////////////////
// Object Array Tests.
//
// These have been Made separate to guarantee signature isn't cached and RValue
// path is still exercised. Note that testing a method is all or nothing, so
// if a constructor is queried there is a test for a GetMethod with "<init>".
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_Method_HandlesLValueLocalObject) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ArrayMultiTest",
      Method{"Foo", jni::Return<void>{}, Params{Array{kClass2}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass2;)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo", LocalArray<jobject, 1, kClass2>{123, obj});
}

TEST_F(JniTest, Array_Field_HandlesLValueLocalObject) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ArrayMultiTest",
      Field{"Foo", Array{kClass2}},
  };

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("Foo"), StrEq("[LkClass2;")));

  LocalObject<kClass> obj{FakeJObject()};
  LocalArray<jobject, 1, kClass2>{obj["Foo"].Get()};
  // obj["Foo"].Set(LocalArray<jobject, 1, kClass2>{FakeJArray()});
  // obj["Foo"].Set(LocalArray<jobject, 1, kClass2>{FakeJArray()}.Release());
  // obj["Foo"].Set(jobjectArray{nullptr});
}

TEST_F(JniTest, Array_HandlesLValueGlobalObject) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ArrayMultiTest",
      Method{"Foo", jni::Return<void>{}, Params{Array{kClass2}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass2;)V")));

  GlobalObject<kClass> obj{AdoptGlobal{}, jobject{nullptr}};
  obj("Foo", LocalArray<jobject, 1, kClass2>{jobjectArray{nullptr}});
}

TEST_F(JniTest, Array_HandlesRValueLocal) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ArrayMultiTest",
      Method{"Foo", jni::Return<void>{}, Params{Array{kClass2}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), _));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass2;)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo", LocalArray<jobject, 1, kClass2>{123, LocalObject<kClass2>{}});
}

TEST_F(JniTest, Array_HandlesRValueGlobal) {
  static constexpr Class kClass2{"kClass2"};
  static constexpr Class kClass{
      "ArrayMultiTest",
      Method{"Foo", jni::Return<void>{}, Params{Array{kClass2}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), _));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass2;)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo", LocalArray<jobject, 1, kClass2>{123, LocalObject<kClass2>{}});
}

TEST_F(JniTest, Array_DifferentiatesBetweenClassesWithEqualRank) {
  static constexpr Class kClass2{"kClass2"};
  static constexpr Class kClass3{"kClass3"};
  static constexpr Class kClass4{"kClass4"};

  static constexpr Class kClass{
      "kClass1", Method{"Foo", jni::Return<void>{}, Params{Array{kClass2}}},
      Method{"Bar", jni::Return<void>{}, Params{Array{kClass3}}},
      Method{"Baz", jni::Return<void>{}, Params{Array{kClass4}}},
      Method{"Gnar", jni::Return<void>{}, Params{Array{Class{"kClass4"}}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), _))
      .Times(testing::AnyNumber());
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass2;)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Bar"), StrEq("([LkClass3;)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("([LkClass4;)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Gnar"), StrEq("([LkClass4;)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo", LocalArray<jobject, 1, kClass2>{123, LocalObject<kClass2>{}});
  obj("Bar", LocalArray<jobject, 1, kClass3>{123, LocalObject<kClass3>{}});
  // obj("Bar", LocalArray{123, LocalObject<kClass2>{}}); // doesn't compile.
  obj("Baz", LocalArray<jobject, 1, kClass4>{123, LocalObject<kClass4>{}});
  obj("Gnar", LocalArray<jobject, 1, kClass4>{123, LocalObject<kClass4>{}});
}

TEST_F(JniTest, Array_DifferentiatesWithOverloads) {
  static constexpr Class kClass2{"kClass2"};
  static constexpr Class kClass{
      "ArrayMultiTest",
      Method{"Foo", jni::Return<void>{}, Params{Array{kClass2}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), _));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass2;)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo", LocalArray<jobject, 1, kClass2>{123, LocalObject<kClass2>{}});
}

////////////////////////////////////////////////////////////////////////////////
// String Array Tests.
// Strings are unusual in that they have their own type (jstring) but are
// almost completely objects otherwise.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_CorrectSignatureForStringParams) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"StringArray", jni::Return{}, jni::Params{Array{jstring{}}}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("StringArray"),
                                 StrEq("([Ljava/lang/String;)V")));

  LocalArray<jstring> arr{3};
  obj("StringArray", arr);
}

TEST_F(JniTest, Array_LocalVanillaObjectRValuesCanBeSet) {
  EXPECT_CALL(*env_, DeleteLocalRef(_)).Times(2);  // array, in place obj
  EXPECT_CALL(*env_, DeleteLocalRef(FakeJString())).Times(0);

  LocalArray<jobject, 1, kJavaLangString> arr{
      3, LocalObject<kJavaLangString>{"Foo"}};
  arr.Set(0, LocalObject<kJavaLangString>{FakeJString()});
}

TEST_F(JniTest, Array_LocalStringRValuesCanBeSet) {
  EXPECT_CALL(*env_, DeleteLocalRef(_))
      .Times(1);  // array (object is moved from)
  EXPECT_CALL(*env_, DeleteLocalRef(FakeJString())).Times(0);

  LocalArray<jstring> arr{3};
  arr.Set(0, LocalString{FakeJString()});
}

}  // namespace
