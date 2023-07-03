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
using ::jni::ArrayView;
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
using ::jni::test::Fake;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Return;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};

////////////////////////////////////////////////////////////////////////////////
// Construction Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, LocalArray_BuildsAndDestroys) {
  EXPECT_CALL(*env_, NewIntArray(1)).WillOnce(Return(Fake<jintArray>()));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>()));

  LocalArray<jint> int_array_1{1};
}

TEST_F(JniTest, LocalArray_IsImplicitlyConvertibleToSpanType) {
  EXPECT_EQ(static_cast<jintArray>(LocalArray<jint>{Fake<jintArray>()}),
            Fake<jintArray>());
}

TEST_F(JniTest, LocalArray_ConstructsIntArrayWithCorrectSize) {
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

TEST_F(JniTest, LocalArray_ConstructsAnArrayOfNullValues) {
  EXPECT_CALL(*env_, NewObjectArray(5, _, nullptr));
  EXPECT_CALL(*env_, FindClass(StrEq("kClass")));
  LocalArray<jobject, 1, kClass> local_object_array{5};
}

TEST_F(JniTest, LocalArray_ConstructsObjectsForLValues) {
  // Unlike POD, objects are constructed with a size, a jclass, and an init
  // object.  This makes for a slightly different API then other objects.
  EXPECT_CALL(*env_, NewObjectArray(5, _, Fake<jobject>()));

  LocalObject<kClass> default_object{Fake<jobject>()};
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

  LocalObject<kClass> obj{Fake<jobject>()};
  LocalArray<jobject, 1, kClass2>{obj["Foo"].Get()};
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

TEST_F(JniTest, Array_CachesLengthExactlyOnceOnFirstRequest) {
  EXPECT_CALL(*env_, GetArrayLength).Times(0);

  LocalArray<jint> obj{Fake<jintArray>()};

  EXPECT_CALL(*env_, GetArrayLength).Times(1).WillOnce(Return(5));
  EXPECT_EQ(obj.Length(), 5);
  EXPECT_EQ(obj.Length(), 5);
  EXPECT_EQ(obj.Length(), 5);
}

////////////////////////////////////////////////////////////////////////////////
// String Array Tests.
// Strings are unusual in that they have their own type (jstring) but are
// almost completely objects otherwise.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_ConstructsFromAnotherStringArray) {
  LocalArray<jstring> arr_1{Fake<jobjectArray>()};
  LocalArray<jstring> arr_2{std::move(arr_1)};
}

TEST_F(JniTest, Array_CorrectSignatureForStringParams) {
  EXPECT_CALL(*env_, FindClass(StrEq("ClassThatReturnsArrays")));
  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/String")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("StringArray"),
                                 StrEq("([Ljava/lang/String;)V")));

  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"StringArray", jni::Return{}, jni::Params{Array{jstring{}}}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  LocalArray<jstring> arr{3};
  obj("StringArray", arr);
}

TEST_F(JniTest, Array_StringsCanBeSetOnLocalString) {
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 0, _));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 1, _));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 2, _));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 3, _));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 4, _));

  const char* kFoo = "kFoo";
  const std::string kNar = "kNar";

  LocalArray<jstring> arr{5, LocalObject<kJavaLangString>{"Foo"}};
  arr.Set(0, "Bar");
  arr.Set(1, std::string{"Baz"});
  arr.Set(2, std::string_view{"Bar"});
  arr.Set(3, std::string_view{kFoo});
  arr.Set(4, std::string_view{kNar});
}

TEST_F(JniTest, Array_LocalVanillaObjectRValuesCanBeSet) {
  // Unfortunately this is getting cached separately by `LocalArray`.
  // In the future, this should drop to 1.
  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/String"))).Times(2);

  EXPECT_CALL(*env_, DeleteLocalRef(_)).Times(2);  // array, in place obj
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>())).Times(2);  // FindClass
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jstring>())).Times(0);

  LocalArray<jobject, 1, kJavaLangString> arr{
      3, LocalObject<kJavaLangString>{"Foo"}};
  arr.Set(0, LocalObject<kJavaLangString>{Fake<jstring>()});
}

TEST_F(JniTest, Array_LocalStringRValuesCanBeSet) {
  EXPECT_CALL(*env_, DeleteLocalRef(_))
      .Times(1);  // array (object is moved from)
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>()));  // FindClass
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jstring>())).Times(0);

  LocalArray<jstring> arr{3};
  arr.Set(0, LocalString{Fake<jstring>()});
}

////////////////////////////////////////////////////////////////////////////////
// Multi-Dimensional Construction.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_BuildsFromSizeForMultiDimensionalArray_no_value) {
  EXPECT_CALL(*env_, NewObjectArray(10, _, Fake<jintArray>()));

  LocalArray<jint, 2>{std::size_t{10}, Fake<jintArray>()};
}

TEST_F(JniTest, Array_BuildsFromSizeForMultiDimensionalArray_primitive_xref) {
  EXPECT_CALL(*env_, FindClass(StrEq("[[I")));
  EXPECT_CALL(*env_, NewObjectArray(10, _, Fake<jintArray>()));

  LocalArray<jint, 2>{std::size_t{10}, LocalArray<jint>{Fake<jintArray>()}};
}

TEST_F(JniTest, Array_BuildsFromSizeForMultiDimensionalArray_primitive_lvalue) {
  EXPECT_CALL(*env_, NewObjectArray(10, _, Fake<jintArray>()));

  LocalArray<jint> arr{Fake<jintArray>()};
  LocalArray<jint, 2>{std::size_t{10}, arr};
}

TEST_F(JniTest, Array_IteratesOver1DRange) {
  std::array expected{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(10));
  EXPECT_CALL(*env_, GetIntArrayElements)
      .WillRepeatedly(Return(expected.data()));

  LocalArray<jint, 1> new_array{Fake<jintArray>()};

  int i{0};
  for (jint val : new_array.Pin()) {
    EXPECT_EQ(val, expected[i]);
    ++i;
  }
  EXPECT_EQ(i, 10);
}

TEST_F(JniTest, Array_WorksWithSTLComparison) {
  std::array expected{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(10));
  EXPECT_CALL(*env_, GetIntArrayElements)
      .WillRepeatedly(Return(expected.data()));

  LocalArray<jint, 1> new_array{Fake<jintArray>()};
  ArrayView array_view = new_array.Pin();
  EXPECT_TRUE(
      std::equal(array_view.begin(), array_view.end(), expected.begin()));
}

TEST_F(JniTest, Array_WorksWithSTLComparisonOfObjects) {
  std::array expected{Fake<jobject>(1), Fake<jobject>(2), Fake<jobject>(3)};
  static constexpr Class kClass{"kClass"};

  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(Return(Fake<jobject>(1)))
      .WillOnce(Return(Fake<jobject>(2)))
      .WillOnce(Return(Fake<jobject>(3)));

  LocalArray<jobject, 1, kClass> new_array{Fake<jobjectArray>()};
  ArrayView array_view = new_array.Pin();
  EXPECT_TRUE(
      std::equal(array_view.begin(), array_view.end(), expected.begin()));
}

TEST_F(JniTest, Array_WorksWithSTLComparisonOfRichlyDecoratedObjects) {
  static constexpr Class kClass{"kClass"};
  std::array expected{LocalObject<kClass>{Fake<jobject>(1)},
                      LocalObject<kClass>{Fake<jobject>(2)},
                      LocalObject<kClass>{Fake<jobject>(3)}};

  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(Return(Fake<jobject>(1)))
      .WillOnce(Return(Fake<jobject>(2)))
      .WillOnce(Return(Fake<jobject>(3)));

  LocalArray<jobject, 1, kClass> new_array{Fake<jobjectArray>()};
  ArrayView array_view = new_array.Pin();
  EXPECT_TRUE(
      std::equal(array_view.begin(), array_view.end(), expected.begin()));
}

TEST_F(JniTest, Array_2D_Iterates) {
  int a[5] = {1, 2, 3, 4, 5};
  std::array expected{1, 2, 3, 4, 5};

  EXPECT_CALL(*env_, FindClass(StrEq("[[I")));
  EXPECT_CALL(*env_, NewObjectArray(5, _, Fake<jobjectArray>()));
  EXPECT_CALL(*env_, GetArrayLength)
      .WillOnce(Return(5))   // outer
      .WillOnce(Return(1))   // inner: 1, running sum: 1
      .WillOnce(Return(2))   // inner: 1, running sum: 3
      .WillOnce(Return(3))   // inner: 1, running sum: 6
      .WillOnce(Return(4))   // inner: 1, running sum: 10
      .WillOnce(Return(5));  // inner: 1, running sum: 15

  // 5: Once per outer array element.
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(Return(Fake<jintArray>(1)))
      .WillOnce(Return(Fake<jintArray>(2)))
      .WillOnce(Return(Fake<jintArray>(3)))
      .WillOnce(Return(Fake<jintArray>(4)))
      .WillOnce(Return(Fake<jintArray>(5)));

  // All the returned intArrays are deleted.
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>()));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(1)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(3)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(4)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(5)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>()));

  // 5 (outer array length) * 2 (pins per) = 10
  EXPECT_CALL(*env_, GetIntArrayElements).Times(10).WillRepeatedly(Return(a));
  EXPECT_CALL(*env_, ReleaseIntArrayElements).Times(10);

  LocalArray<jint, 2> new_array{5, Fake<jobjectArray>()};

  int sum = 0;
  for (LocalArray<jint> arr_1d : new_array.Pin()) {
    // Note: Each `Pin` below triggers a separate `GetIntArrayElements`.
    {
      auto pinn = arr_1d.Pin();
      EXPECT_TRUE(std::equal(pinn.begin(), pinn.end(), expected.begin()));
    }

    // Note: GetArrayLength is not called again (it's cached).
    {
      for (jint val : arr_1d.Pin()) {
        sum += val;
      }
    }
  }

  // 1 + 3 + 6 + 10 + 15 = 35
  EXPECT_EQ(sum, 35);
}

}  // namespace
