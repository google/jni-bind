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
using ::jni::ArrayFromRank_t;
using ::jni::ArrayStrip_t;
using ::jni::CDecl_t;
using ::jni::Class;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Rankifier;
using ::jni::RegularToArrayTypeMap_t;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};

static constexpr Array arr1{jint{}};
static constexpr Array arr2{jfloat{}};
static constexpr Array arr3{jdouble{}};
static constexpr Array arr4{Class{"kClass"}};
static constexpr Array arr5{kClass};
static constexpr Array arr6{arr1};

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
static_assert(arr1 != arr6);
static_assert(arr6 != arr1);

// Classes.
static_assert(arr4 == arr4);
static_assert(arr4 == arr5);
static_assert(arr3 != arr4);

////////////////////////////////////////////////////////////////////////////////
// Primality Tests.
////////////////////////////////////////////////////////////////////////////////

static_assert(jni::kIsPrimitiveArrayType<jbooleanArray>);
static_assert(jni::kIsPrimitiveArrayType<jbyteArray>);
static_assert(jni::kIsPrimitiveArrayType<jcharArray>);
static_assert(jni::kIsPrimitiveArrayType<jshortArray>);
static_assert(jni::kIsPrimitiveArrayType<jintArray>);
static_assert(jni::kIsPrimitiveArrayType<jfloatArray>);
static_assert(jni::kIsPrimitiveArrayType<jlongArray>);

static_assert(!jni::kIsPrimitiveArrayType<jobjectArray>);

////////////////////////////////////////////////////////////////////////////////
// Rank Tests.
////////////////////////////////////////////////////////////////////////////////
static constexpr jint kArrRank0{0};
static constexpr Array kArrRank1{jint{}};
static constexpr Array kArrRank2{Array{jint{}}};
static constexpr Array kArrRank3{Array{Array{jint{}}}};
static constexpr Array kArrRank4{Array{Array{Array{jint{}}}}};

using ArrRank0T = std::decay_t<decltype(kArrRank0)>;
using ArrRank1T = std::decay_t<decltype(kArrRank1)>;
using ArrRank2T = std::decay_t<decltype(kArrRank2)>;
using ArrRank3T = std::decay_t<decltype(kArrRank3)>;
using ArrRank4T = std::decay_t<decltype(kArrRank4)>;

// Surprisingly these do actually require some care with the CTAD guides.
static_assert(std::is_same_v<ArrRank0T, jint>);
static_assert(std::is_same_v<ArrRank1T, Array<jint>>);
static_assert(std::is_same_v<ArrRank2T, Array<Array<jint>>>);
static_assert(std::is_same_v<ArrRank3T, Array<Array<Array<jint>>>>);
static_assert(std::is_same_v<ArrRank4T, Array<Array<Array<Array<jint>>>>>);

static_assert(Rankifier<ArrRank0T>::Rank(kArrRank0) == 0);
static_assert(Rankifier<ArrRank1T>::Rank(kArrRank1) == 1);
static_assert(Rankifier<ArrRank2T>::Rank(kArrRank2) == 2);
static_assert(Rankifier<ArrRank3T>::Rank(kArrRank3) == 3);
static_assert(Rankifier<ArrRank4T>::Rank(kArrRank4) == 4);

////////////////////////////////////////////////////////////////////////////////
// Strip Tests.
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same_v<ArrayStrip_t<ArrRank0T>, jint>);
static_assert(std::is_same_v<ArrayStrip_t<ArrRank1T>, jint>);
static_assert(std::is_same_v<ArrayStrip_t<ArrRank2T>, jint>);
static_assert(std::is_same_v<ArrayStrip_t<ArrRank3T>, jint>);

static_assert(jni::kIsArrayType<Array<int>>);
static_assert(jni::kIsArrayType<Array<Array<int>>>);
static_assert(
    std::is_same_v<int, std::decay_t<decltype(jni::FullArrayStripV(arr1))>>);
static_assert(
    std::is_same_v<int, std::decay_t<decltype(jni::FullArrayStripV(arr6))>>);

////////////////////////////////////////////////////////////////////////////////
// Build from Rank Tests.
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same_v<ArrayFromRank_t<jint, 0>, jint>);
static_assert(std::is_same_v<ArrayFromRank_t<jint, 1>, Array<jint>>);
static_assert(std::is_same_v<ArrayFromRank_t<jint, 2>, Array<Array<jint>>>);
static_assert(
    std::is_same_v<ArrayFromRank_t<jint, 3>, Array<Array<Array<jint>>>>);

////////////////////////////////////////////////////////////////////////////////
// Implementation Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_HandlesSingleIntArray) {
  static constexpr Class kClass{
      "ClassThatReturnsIntArrays",
      Method{"IntArray", jni::Return{Array{jint{}}}, Params{}}};

  LocalObject<kClass> obj{jobject{nullptr}};

  static_assert(
      !std::is_base_of_v<jni::RefBaseTag<jobjectArray>, decltype(obj)>);

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("()[I")));
  LocalArray<int> a = obj("IntArray");
}

TEST_F(JniTest, Array_ConstructsFromRValue) {
  LocalArray<jint> arr{jintArray{nullptr}};
  LocalArray<jint> arr2{std::move(arr)};
}

TEST_F(JniTest, Array_HandlesSingleObjectArrayAsReturn) {
  static constexpr Class kClass2{"kClass2"};
  static constexpr Class kClass{
      "ClassThatReturnsIntArrays",
      Method{"ObjectArray", jni::Return{Array{kClass2}}, Params{}}};

  LocalObject<kClass> obj{jobject{nullptr}};
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

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("takesFloatArray", jfloatArray{nullptr});
}

TEST_F(JniTest, Array_HandlesSingleObjectArray) {
  static constexpr Class kClass2{"kClass2"};
  static constexpr Class kClass{
      "kClass",
      Method{"takesObjectArray", jni::Return<void>{}, Params{Array{kClass2}}}};

  EXPECT_CALL(
      *env_, GetMethodID(_, StrEq("takesObjectArray"), StrEq("([LkClass2;)V")));

  LocalArray<jobject, 1, kClass2> input_arg{nullptr};
  LocalObject<kClass> obj{jobject{nullptr}};
  obj("takesObjectArray", input_arg);
}

TEST_F(JniTest, Array_HandlesSingleBoolAsParamWithRankfulReturnType) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"Foo", jni::Return<int>{}, Params{Array{jboolean{}}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([Z)I")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo", jbooleanArray{nullptr});
}

TEST_F(JniTest, Array_HandlesSingleBoolAsParamWithVoidReturnT) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"Foo", jni::Return{}, Params{Array{jboolean{}}}}

  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([Z)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  // obj("Foo", jarray{nullptr});     // doesn't compile (good).
  // obj("Foo", jintArray{nullptr});  // doesn't compile (good).
  obj("Foo", jbooleanArray{nullptr});
}

TEST_F(JniTest, Array_HandlesMultipleBoolAsParam) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"Z", jni::Return{}, Params{Array{jboolean{}}}},
      Method{"ZZ", jni::Return{}, Params{Array{jboolean{}}, Array{jboolean{}}}},
      Method{"ZZZ", jni::Return{},
             Params{Array{jboolean{}}, Array{jboolean{}}, Array{jboolean{}}}},

  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Z"), StrEq("([Z)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ZZ"), StrEq("([Z[Z)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ZZZ"), StrEq("([Z[Z[Z)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Z", jbooleanArray{nullptr});
  obj("ZZ", jbooleanArray{nullptr}, jbooleanArray{nullptr});
  obj("ZZZ", jbooleanArray{nullptr}, jbooleanArray{nullptr},
      jbooleanArray{nullptr});
}

TEST_F(JniTest, Array_HandlesComplexArrays) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"Foo", jni::Return{Array{kClass2}},
             Params{Array{jint{}}, Array{jboolean{}}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([I[Z)[LkClass2;")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo", jintArray{nullptr}, jbooleanArray{nullptr});
}

TEST_F(JniTest, Array_AllowsRValuesOfLocalArrays) {
  static constexpr Class kClass{
      "ClassThatTakesRValues",
      Method{"Foo", jni::Return{}, Params{Array{jint{}}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([I)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  LocalArray<jint> local_array{3};

  obj("Foo", std::move(local_array));
}

TEST_F(JniTest, Array_HandlesSingle2DIntAsReturnT) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"I", jni::Return{Array{Array{jint{}}}}, Params{}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("()[[I")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("I");
}

/*
// TODO(143908983): In order for the return to have higher dimensional rank all
// of the proxy must be overloadable.
TEST_F(JniTest, Array_HandlesSingle2DIntAsParamWithRankfulReturnT) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"I", jni::Return<int>{}, Params{Array{Array{jint{}}}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("([[I)I")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("I", jintArray{nullptr});
}
*/

TEST_F(JniTest, Array_HandlesSingle2DClassAsReturn) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"Foo", jni::Return{Array{Array{kClass2}}}, Params{}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("()[[LkClass2;")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo");
}

TEST_F(JniTest, Array_HandlesSinglePredefinedClassAsParam) {
  static constexpr Class kClass{"kClass"};
  static constexpr Class kClassThatAcceptsArrays{
      "ClassThatAcceptsArrays",
      Method{"Foo", jni::Return<void>{}, Params{Array{kClass}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([LkClass;)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  LocalObject<kClassThatAcceptsArrays> obj_to_call_on{jobject{nullptr}};
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

  LocalObject<kClass> obj{jobject{nullptr}};
  LocalObject<kClassThatAcceptsArrays> obj_to_call_on{jobject{nullptr}};
  LocalArray<jobject, 1, kClass> local_array{5, obj};
  obj_to_call_on("Foo", local_array);
}

////////////////////////////////////////////////////////////////////////////////
// Various Complicated Array Tests.
////////////////////////////////////////////////////////////////////////////////

TEST_F(JniTest, Array_LooksUpCorrectSignaturesForReturns) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"BoolArray", jni::Return{Array{jboolean{}}}, Params{}},
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
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("BoolArray"), StrEq("()[Z")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ByteArray"), StrEq("()[B")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("CharArray"), StrEq("()[C")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ShortArray"), StrEq("()[S")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("()[I")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("FloatArray"), StrEq("()[F")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("DoubleArray"), StrEq("()[D")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("LongArray"), StrEq("()[J")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("()[LkClass2;")));
  obj("BoolArray");
  obj("ByteArray");
  obj("CharArray");
  obj("ShortArray");
  obj("IntArray");
  obj("LongArray");
  obj("FloatArray");
  obj("DoubleArray");
  obj("ObjectArray");
}

TEST_F(JniTest, Array_LooksUpCorrectSignaturesForInputParams) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"BoolArray", jni::Return<void>{}, Params{Array{jboolean{}}}},
      Method{"ByteArray", jni::Return<void>{}, Params{Array{jbyte{}}}},
      Method{"CharArray", jni::Return<void>{}, Params{Array{jchar{}}}},
      Method{"ShortArray", jni::Return<void>{}, Params{Array{jshort{}}}},
      Method{"IntArray", jni::Return<void>{}, Params{Array{jint{}}}},
      Method{"FloatArray", jni::Return<void>{}, Params{Array{jfloat{}}}},
      Method{"DoubleArray", jni::Return<void>{}, Params{Array{jdouble{}}}},
      Method{"LongArray", jni::Return<void>{}, Params{Array{jlong{}}}},
      Method{"ObjectArray", jni::Return<void>{}, Params{Array{kClass2}}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("BoolArray"), StrEq("([Z)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ByteArray"), StrEq("([B)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("CharArray"), StrEq("([C)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ShortArray"), StrEq("([S)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("([I)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("FloatArray"), StrEq("([F)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("DoubleArray"), StrEq("([D)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("LongArray"), StrEq("([J)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("([LkClass2;)V")));

  obj("BoolArray", jbooleanArray{nullptr});
  obj("ByteArray", jbyteArray{nullptr});
  obj("CharArray", jcharArray{nullptr});
  obj("ShortArray", jshortArray{nullptr});
  obj("IntArray", jintArray{nullptr});
  obj("FloatArray", jfloatArray{nullptr});
  obj("LongArray", jlongArray{nullptr});
  obj("DoubleArray", jdoubleArray{nullptr});
  obj("ObjectArray", jobjectArray{nullptr});
}

// TODO(b/143908983): Handle multi dimensional values.
/*
TEST_F(JniTest, Array_HandlesMultipleMultiDimensionalValues) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ArrayMultiTest",
      Method{"Foo", jni::Return{Array{jint{}}}, Params{Array{Array{jint{}}}}},
      Method{"Baz", jni::Return{Array{kClass2}},
             Params{Array{Array{Array{jint{}}}}}},
      Method{"Bar", jni::Return{Array{Array{Class{"kClass3"}}}},
             Params{Array{Array{Array{jint{}}}}, Array{jint{}}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("([[I)[I")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("([[[I)[LkClass2;")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Bar"), StrEq("([[[I[I)[[LkClass3;")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Foo", jintArray{nullptr});
  obj("Baz", jintArray{nullptr});
  obj("Bar", jintArray{nullptr}, jintArray{nullptr});
}
*/

}  // namespace
