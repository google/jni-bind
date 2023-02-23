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

using ::jni::Array;
using ::jni::ArrayStrip_t;
using ::jni::CDecl_t;
using ::jni::Class;
using ::jni::Field;
using ::jni::FullArrayStripV;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Rank;
using ::jni::Rankifier;
using ::jni::RegularToArrayTypeMap_t;
using ::jni::test::Fake;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

jobject FakeJObject() { return reinterpret_cast<jobject>(0XBBBBBBBB); }
jfieldID FakeJFieldId() { return reinterpret_cast<jfieldID>(0XABCDEFAB); }

static constexpr Class kClass{"kClass"};
static constexpr Class kClass2{"kClass2"};
/*
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

////////////////////////////////////////////////////////////////////////////////
// Implementation Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_HandlesSingleIntArray) {
  static constexpr Class kClass{
      "ClassThatReturnsIntArrays",
      Method{"IntArray", jni::Return{Array{jint{}}}, Params{}}};

  LocalObject<kClass> obj{Fake<jobject>()};

  static_assert(
      !std::is_base_of_v<jni::RefBaseTag<jobjectArray>, decltype(obj)>);

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("()[I")));
  LocalArray<int> a = obj("IntArray");
  LocalArray<int> b{a.Release()};
}

TEST_F(JniTest, Array_ConstructsFromRValue) {
  LocalArray<jint> arr{jintArray{nullptr}};
  LocalArray<jint> arr2{std::move(arr)};
}

TEST_F(JniTest, Array_HandlesSingleObjectArrayAsReturn) {
  static constexpr Class kClass{
      "ClassThatReturnsIntArrays",
      Method{"ObjectArray", jni::Return{Array{kClass2}}, Params{}}};

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("()[LkClass2;")));

  LocalArray<jobject, 1, kClass2> arr{obj("ObjectArray")};
  // arr2 { obj("ObjectArray") }; // won't compile (good).
  LocalArray arr2{obj("ObjectArray")};
  LocalArray<jobject, 1, kClass2> arr3{std::move(arr2)};
}

TEST_F(JniTest, Array_HandlesSingleArrayParam_NativeJNIType) {
  static constexpr Class kClass{
      "kClass",
      Method{"takesFloatArray", jni::Return<void>{}, Params{Array{float{}}}}};

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("takesFloatArray", Fake<jfloatArray>());
}

TEST_F(JniTest, Array_HandlesSingleObjectArray) {
  static constexpr Class kClass{
      "kClass",
      Method{"takesObjectArray", jni::Return<void>{}, Params{Array{kClass2}}}};

  EXPECT_CALL(
      *env_, GetMethodID(_, StrEq("takesObjectArray"), StrEq("([LkClass2;)V")));

  LocalArray<jobject, 1, kClass2> input_arg{Fake<jobjectArray>()};
  LocalObject<kClass> obj{Fake<jobject>()};
  obj("takesObjectArray", input_arg);
}

TEST_F(JniTest, Array_HandlesSingleBoolAsParamWithRankfulReturnType) {
  static constexpr Class kClass{
      "kClass", Method{"Foo", jni::Return<int>{}, Params{Array{jboolean{}}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([Z)I")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("Foo", Fake<jbooleanArray>());
}

TEST_F(JniTest, Array_HandlesSingleBoolAsParamWithVoidReturnT) {
  static constexpr Class kClass{
      "kClass", Method{"Foo", jni::Return{}, Params{Array{jboolean{}}}}

  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([Z)V")));

  LocalObject<kClass> obj{Fake<jobject>()};
  // obj("Foo", Fake<jarray>());     // doesn't compile (good).
  // obj("Foo", Fake<jintArray>());  // doesn't compile (good).
  obj("Foo", Fake<jbooleanArray>());
}

TEST_F(JniTest, Array_HandlesMultipleBoolAsParam) {
  static constexpr Class kClass{
      "kClass",
      Method{"Z", jni::Return{}, Params{Array{jboolean{}}}},
      Method{"ZZ", jni::Return{}, Params{Array{jboolean{}}, Array{jboolean{}}}},
      Method{"ZZZ", jni::Return{},
             Params{Array{jboolean{}}, Array{jboolean{}}, Array{jboolean{}}}},

  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Z"), StrEq("([Z)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ZZ"), StrEq("([Z[Z)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ZZZ"), StrEq("([Z[Z[Z)V")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("Z", Fake<jbooleanArray>(1));
  obj("ZZ", Fake<jbooleanArray>(2), Fake<jbooleanArray>(3));
  obj("ZZZ", Fake<jbooleanArray>(3), Fake<jbooleanArray>(4),
      Fake<jbooleanArray>(5));
}

TEST_F(JniTest, Array_HandlesComplexArrays) {
  static constexpr Class kClass{
      "kClass",
      Method{"Foo", jni::Return{Array{kClass2}},
             Params{Array{jint{}}, Array{jboolean{}}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([I[Z)[LkClass2;")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("Foo", Fake<jintArray>(), Fake<jbooleanArray>());
}

TEST_F(JniTest, Array_AllowsRValuesOfLocalArrays) {
  static constexpr Class kClass{
      "ClassThatTakesRValues",
      Method{"Foo", jni::Return{}, Params{Array{jint{}}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([I)V")));

  LocalObject<kClass> obj{Fake<jobject>()};
  LocalArray<jint> local_array{3};

  obj("Foo", std::move(local_array));
}

TEST_F(JniTest, Array_HandlesSingle2DIntAsReturnT) {
  static constexpr Class kClass{
      "kClass", Method{"I", jni::Return{Array<jint, 2>{}}, Params{}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("()[[I")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("I");
}

TEST_F(JniTest, Array_HandlesSingle2DIntAsParamWithRankfulReturnT) {
  static constexpr Class kClass{
      "kClass", Method{"I", jni::Return<int>{}, Params{Array<jint, 2>{}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("([[I)I")));

  LocalObject<kClass> obj{Fake<jobject>()};
  // obj("I", jintArray{nullptr}); // doesn't compile (good).
  obj("I", Fake<jobjectArray>());
  obj("I", LocalArray<jint, 2>{Fake<jobjectArray>()});
}

TEST_F(JniTest, Array_HandlesSingle2DClassAsReturn) {
  static constexpr Class kClass{
      "kClass",
      Method{"Foo", jni::Return{Array{kClass2, Rank<2>{}}}, Params{}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("()[[LkClass2;")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("Foo");
}

TEST_F(JniTest, Array_HandlesSinglePredefinedClassAsParam) {
  static constexpr Class kClass{"kClass"};
  static constexpr Class kClassThatAcceptsArrays{
      "ClassThatAcceptsArrays",
      Method{"Foo", jni::Return<void>{}, Params{Array{kClass}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass;)V")));

  LocalObject<kClass> obj{Fake<jobject>()};
  LocalObject<kClassThatAcceptsArrays> obj_to_call_on{Fake<jobject>()};
  LocalArray<jobject, 1, kClass> local_array{5, obj};
  obj_to_call_on("Foo", local_array);
}

TEST_F(JniTest, Array_HandlesSingleUndefinedClassAsParam) {
  static constexpr Class kClassThatAcceptsArrays{
      "ClassThatAcceptsArrays",
      // Note, Class is defined inline here.
      Method{"Foo", jni::Return<void>{}, Params{Array{Class{"kClass"}}}}};
  static constexpr Class kClass{"kClass"};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass;)V")));

  LocalObject<kClass> obj{Fake<jobject>()};
  LocalObject<kClassThatAcceptsArrays> obj_to_call_on{Fake<jobject>()};
  LocalArray<jobject, 1, kClass> local_array{5, obj};
  obj_to_call_on("Foo", local_array);
}

////////////////////////////////////////////////////////////////////////////////
// Methods.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_LooksUpCorrectSignaturesForReturns) {
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

////////////////////////////////////////////////////////////////////////////////
// Methods: String Tests.
//  These are isolated since they are special composite types.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_CorrectReturnSignatureForStrings) {
  static constexpr Class kClass{
      "kClass",
      Method{"StringArray", jni::Return{Array{jstring{}}}, Params{}},
  };

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("StringArray"),
                                 StrEq("()[Ljava/lang/String;")));
  LocalArray<jstring> arr = obj("StringArray");
}

TEST_F(JniTest, Array_CorrectParamSignatureForStrings) {
  static constexpr Class kClass{
      "kClass",
      Method{"StringArray", jni::Return{}, Params{Array{jstring{}}}},
  };

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("StringArray"),
                                 StrEq("([Ljava/lang/String;)V")));
  LocalArray<jstring> arr{2};
  obj("StringArray", arr);
}
*/

////////////////////////////////////////////////////////////////////////////////
// Fields: Smoke Tests.
////////////////////////////////////////////////////////////////////////////////
static constexpr Class kFieldClass{
    "kFieldClass",
    Field{"BooleanArray", Array{jboolean{}}},
    Field{"ByteArray", Array{jbyte{}}},
    Field{"CharArray", Array{jchar{}}},
    Field{"ShortArray", Array{jshort{}}},
    Field{"IntArray", Array{jint{}}},
    Field{"FloatArray", Array{jfloat{}}},
    Field{"DoubleArray", Array{jdouble{}}},
    Field{"LongArray", Array{jlong{}}},
    Field{"ObjectArrayRank1", Array{kClass2}},
    Field{"ObjectArrayRank2", Array{kClass2, Rank<2>{}}},
    Field{"ObjectArrayRank3", Array{kClass2, Rank<3>{}}},
};

/*
TEST_F(JniTest, Array_FieldTests) {
  static constexpr Class kClass{
      "kClass",
      Field{"BoolArray", Array{jboolean{}}},
      Field{"ByteArray", Array{jbyte{}}},
      Field{"CharArray", Array{jchar{}}},
      Field{"ShortArray", Array{jshort{}}},
      Field{"IntArray", Array{jint{}}},
      Field{"FloatArray", Array{jfloat{}}},
      Field{"DoubleArray", Array{jdouble{}}},
      Field{"LongArray", Array{jlong{}}},
      Field{"ObjectArrayRank1", Array{kClass2}},
      Field{"ObjectArrayRank2", Array{kClass2, Rank<2>{}}},
      Field{"ObjectArrayRank3", Array{kClass2, Rank<3>{}}},
  };

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("BooleanArray"), StrEq("[Z")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ByteArray"), StrEq("[B")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("CharArray"), StrEq("[C")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ShortArray"), StrEq("[S")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("IntArray"), StrEq("[I")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("FloatArray"), StrEq("[F")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("DoubleArray"), StrEq("[D")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("LongArray"), StrEq("[J")));
  EXPECT_CALL(*env_,
              GetFieldID(_, StrEq("ObjectArrayRank1"), StrEq("[LkClass2;")));
  EXPECT_CALL(*env_,
              GetFieldID(_, StrEq("ObjectArrayRank2"), StrEq("[[LkClass2;")));
  EXPECT_CALL(*env_,
              GetFieldID(_, StrEq("ObjectArrayRank3"), StrEq("[[[LkClass2;")));

  obj["BooleanArray"].Get();
  obj["ByteArray"].Get();
  obj["CharArray"].Get();
  obj["ShortArray"].Get();
  obj["IntArray"].Get();
  obj["FloatArray"].Get();
  obj["LongArray"].Get();
  obj["DoubleArray"].Get();
  obj["ObjectArrayRank1"].Get();
  obj["ObjectArrayRank2"].Get();
  obj["ObjectArrayRank3"].Get();
}

////////////////////////////////////////////////////////////////////////////////
// Fields: String Smoke Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_CorrectFieldSignatureForStrings) {
  static constexpr Class kClass{
      "kClass",
      Field{"StringArrayRank1", Array{jstring{}}},
      Field{"StringArrayRank2", Array{jstring{}, Rank<2>{}}},
      Field{"StringArrayRank3", Array{jstring{}, Rank<3>{}}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("StringArrayRank1"),
                                StrEq("[Ljava/lang/String;")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("StringArrayRank2"),
                                StrEq("[[Ljava/lang/String;")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("StringArrayRank3"),
                                StrEq("[[[Ljava/lang/String;")));

  LocalArray<jstring> arr1 = obj["StringArrayRank1"].Get();
  LocalArray<jstring, 2> arr2 = obj["StringArrayRank2"].Get();
  LocalArray<jstring, 3> arr3 = obj["StringArrayRank3"].Get();
}

////////////////////////////////////////////////////////////////////////////////
// Fields: Aggressive E2E Tests.
////////////////////////////////////////////////////////////////////////////////

TEST_F(JniTest, Array_Field_Boolean_Test) {
  std::unique_ptr<jboolean> fake_storage_ptr(new jboolean[5]());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("BooleanArray"), StrEq("[Z")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_,
              SetObjectField(FakeJObject(), FakeJFieldId(),
                             reinterpret_cast<jbooleanArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetBooleanArrayElements)
      .WillOnce(::testing::Return(fake_storage_ptr.get()));

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jboolean> arr{obj["BooleanArray"].Get()};
  obj["BooleanArray"].Set(reinterpret_cast<jbooleanArray>(0XCCCCCCCC));
  EXPECT_EQ(obj["BooleanArray"].Get().Pin().ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Byte_Test) {
  std::unique_ptr<jbyte> fake_storage_ptr(new jbyte[5]());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ByteArray"), StrEq("[B")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_, SetObjectField(FakeJObject(), FakeJFieldId(),
                                    reinterpret_cast<jbyteArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetByteArrayElements)
      .WillOnce(::testing::Return(fake_storage_ptr.get()));

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jbyte> arr{obj["ByteArray"].Get()};
  obj["ByteArray"].Set(reinterpret_cast<jbyteArray>(0XCCCCCCCC));
  EXPECT_EQ(obj["ByteArray"].Get().Pin().ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Char_Test) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("CharArray"), StrEq("[C")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_,
              SetObjectField(FakeJObject(), FakeJFieldId(),
                             reinterpret_cast<jcharArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetCharArrayElements);

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jchar> arr{obj["CharArray"].Get()};
  obj["CharArray"].Set(reinterpret_cast<jcharArray>(0XCCCCCCCC));
  obj["CharArray"].Get().Pin();
}

TEST_F(JniTest, Array_Field_Short_Test) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ShortArray"), StrEq("[S")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_,
              SetObjectField(FakeJObject(), FakeJFieldId(),
                             reinterpret_cast<jshortArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetShortArrayElements);

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jshort> arr{obj["ShortArray"].Get()};
  obj["ShortArray"].Set(reinterpret_cast<jshortArray>(0XCCCCCCCC));
  obj["ShortArray"].Get().Pin();
}

TEST_F(JniTest, Array_Field_Int_Test) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("IntArray"), StrEq("[I")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_,
              SetObjectField(FakeJObject(), FakeJFieldId(),
                             reinterpret_cast<jintArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetIntArrayElements);

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jint> arr{obj["IntArray"].Get()};
  obj["IntArray"].Set(reinterpret_cast<jintArray>(0XCCCCCCCC));
  obj["IntArray"].Get().Pin();
}

TEST_F(JniTest, Array_Field_Float_Test) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("FloatArray"), StrEq("[F")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_,
              SetObjectField(FakeJObject(), FakeJFieldId(),
                             reinterpret_cast<jfloatArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetFloatArrayElements);

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jfloat> arr{obj["FloatArray"].Get()};
  obj["FloatArray"].Set(reinterpret_cast<jfloatArray>(0XCCCCCCCC));
  obj["FloatArray"].Get().Pin();
}
*/

TEST_F(JniTest, Array_Field_Double_Test) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("DoubleArray"), StrEq("[D")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_,
              SetObjectField(FakeJObject(), FakeJFieldId(),
                             reinterpret_cast<jdoubleArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetDoubleArrayElements);

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jdouble> arr{obj["DoubleArray"].Get()};
  // obj["DoubleArray"].Set(reinterpret_cast<jdoubleArray>(0XCCCCCCCC));
  obj["DoubleArray"].Set(std::move(arr));
  obj["DoubleArray"].Get().Pin();
}

/*
TEST_F(JniTest, Array_Field_Long_Test) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("LongArray"), StrEq("[J")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_,
              SetObjectField(FakeJObject(), FakeJFieldId(),
                             reinterpret_cast<jlongArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetLongArrayElements);

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jlong> arr{obj["LongArray"].Get()};
  obj["LongArray"].Set(reinterpret_cast<jlongArray>(0XCCCCCCCC));
  obj["LongArray"].Get().Pin();
}

TEST_F(JniTest, Array_Field_Object_Test) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ObjectArray"), StrEq("[LkClass;")))
      .WillOnce(::testing::Return(FakeJFieldId()));
  EXPECT_CALL(*env_,
              SetObjectField(FakeJObject(), FakeJFieldId(),
                             reinterpret_cast<jobjectArray>(0XCCCCCCCC)));
  EXPECT_CALL(*env_, GetObjectArrayElements);

  LocalObject<kFieldClass> obj{FakeJObject()};
  LocalArray<jobject> arr{obj["ObjectArray"].Get()};
  obj["ObjectArray"].Set(reinterpret_cast<jobjectArray>(0XCCCCCCCC));
  obj["ObjectArray"].Get().Pin();
}

////////////////////////////////////////////////////////////////////////////////
// Multi-Dimensional arrays.
////////////////////////////////////////////////////////////////////////////////
static constexpr Class kArrClass{
    "ArrClass",
    Method{"Foo", jni::Return{Array{jint{}}}, Params{Array<jint, 2>{}}},
    Method{"Baz", jni::Return{Array{kClass2}}, Params{Array<jint, 3>{}}},
    Method{"Bar", jni::Return{Array{Class{"kClass3"}, Rank<2>{}}},
           Params{Array<jint, 3>{}, Array{double{}}}},
};

TEST_F(JniTest, Array_HandlesMultipleMultiDimensionalValues_1) {
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([[I)[I")));
  LocalObject<kArrClass> obj{Fake<jobject>()};
  LocalArray<jint, 1> ret = obj("Foo", Fake<jobjectArray>());
}

TEST_F(JniTest, Array_HandlesMultipleMultiDimensionalValues_2) {
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

TEST_F(JniTest, Array_HandlesMultipleMultiDimensionalValues_3) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Bar"), StrEq("([[[I[D)[[LkClass3;")));

  LocalObject<kArrClass> obj{Fake<jobject>()};
  obj("Bar", Fake<jobjectArray>(), Fake<jdoubleArray>());
}
*/

}  // namespace
