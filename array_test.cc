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

using jni::Array;
using jni::ArrayStrip_t;
using jni::Class;
using jni::LocalArray;
using jni::LocalArrayTag;
using jni::LocalObject;
using jni::Method;
using jni::Params;
using jni::Rankifier;
using jni::test::JniTest;
using testing::_;
using testing::StrEq;

static constexpr Class kClass{"kClass"};

static constexpr Array arr1{jint{}};
static constexpr Array arr2{jfloat{}};
static constexpr Array arr3{jdouble{}};
static constexpr Array arr4{Class{"kClass"}};
static constexpr Array arr5{kClass};

static_assert(arr1 == arr1);
static_assert(arr1 == arr1);
static_assert(arr1 != arr2);
static_assert(arr1 != arr2);
static_assert(arr1 != arr3);
static_assert(arr2 != arr3);

// Classes.
static_assert(arr4 == arr4);
static_assert(arr4 == arr5);
static_assert(arr3 != arr4);

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

////////////////////////////////////////////////////////////////////////////////
// Implementation Tests.
////////////////////////////////////////////////////////////////////////////////

TEST_F(JniTest, Array_HandlesSingleIntArray) {
  static constexpr Class kClass{
      "ClassThatReturnsIntArrays",
      Method{"IntArray", jni::Return{Array{jint{}}}, Params{}}};

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("()[I")));

  static_assert(
      std::is_base_of_v<LocalArrayTag<jint>, decltype(obj("IntArray"))>);
  obj("IntArray");
}

TEST_F(JniTest, Array_HandlesSingleObjectArray) {
  static constexpr Class kClass2{"kClass2"};
  static constexpr Class kClass{
      "ClassThatReturnsIntArrays",
      Method{"ObjectArray", jni::Return{Array{kClass2}}, Params{}}};

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("()[LkClass2;")));

  obj("ObjectArray");
}

TEST_F(JniTest, Array_HandlesSingleBoolAsParamWithRankfulReturnT) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"Z", jni::Return<int>{}, Params{Array{jboolean{}}}}

  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Z"), StrEq("([Z)I")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Z", jbooleanArray{nullptr});
}

TEST_F(JniTest, Array_HandlesSingleBoolAsParamWithVoidReturnT) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"Z", jni::Return{}, Params{Array{jboolean{}}}}

  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Z"), StrEq("([Z)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("Z", jintArray{nullptr});
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
  obj("Z", jintArray{nullptr});
  obj("ZZ", jintArray{nullptr}, jintArray{nullptr});
  obj("ZZZ", jintArray{nullptr}, jintArray{nullptr}, jintArray{nullptr});
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
      Method{"I", jni::Return{}, Params{Array{jint{}}}},
  };

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("([I)V")));

  LocalObject<kClass> obj{jobject{nullptr}};
  LocalArray<jint> local_array{3};
  obj("I", std::move(local_array));
}

TEST_F(JniTest, Array_HandlesSingle2DIntAsReturnT) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"I", jni::Return{Array{Array{jint{}}}}, Params{}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("()[[I")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("I");
}

TEST_F(JniTest, Array_HandlesSingle2DIntAsParamWithRankfulReturnT) {
  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"I", jni::Return<int>{}, Params{Array{Array{jint{}}}}}};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("I"), StrEq("([[I)I")));

  LocalObject<kClass> obj{jobject{nullptr}};
  obj("I", jintArray{nullptr});
}

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
  LocalArray<jobject, kClass> local_array{5, obj};
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
  LocalArray<jobject, kClass> local_array{5, obj};
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
      Method{"IntArray", jni::Return{Array{jint{}}}, Params{}},
      Method{"FloatArray", jni::Return{Array{jfloat{}}}, Params{}},
      Method{"LongArray", jni::Return{Array{jlong{}}}, Params{}},
      Method{"ObjectArray", jni::Return{Array{kClass2}}, Params{}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("BoolArray"), StrEq("()[Z")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ByteArray"), StrEq("()[B")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("CharArray"), StrEq("()[C")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("()[I")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("LongArray"), StrEq("()[J")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("FloatArray"), StrEq("()[F")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("()[LkClass2;")));
  obj("BoolArray");
  obj("ByteArray");
  obj("CharArray");
  obj("IntArray");
  obj("LongArray");
  obj("FloatArray");
  obj("ObjectArray");
}

TEST_F(JniTest, Array_LooksUpCorrectSignaturesForInputParams) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"BoolArray", jni::Return<void>{}, Params{Array{jboolean{}}}},
      Method{"ByteArray", jni::Return<void>{}, Params{Array{jbyte{}}}},
      Method{"CharArray", jni::Return<void>{}, Params{Array{jchar{}}}},
      Method{"IntArray", jni::Return<void>{}, Params{Array{jint{}}}},
      Method{"FloatArray", jni::Return<void>{}, Params{Array{jfloat{}}}},
      Method{"LongArray", jni::Return<void>{}, Params{Array{jlong{}}}},
      Method{"ObjectArray", jni::Return<void>{}, Params{Array{kClass2}}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("BoolArray"), StrEq("([Z)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ByteArray"), StrEq("([B)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("CharArray"), StrEq("([C)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("([I)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("LongArray"), StrEq("([J)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("FloatArray"), StrEq("([F)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("([LkClass2;)V")));

  obj("BoolArray", jbooleanArray{nullptr});
  obj("ByteArray", jbyteArray{nullptr});
  obj("CharArray", jcharArray{nullptr});
  obj("IntArray", jintArray{nullptr});
  obj("LongArray", jlongArray{nullptr});
  obj("FloatArray", jfloatArray{nullptr});
  obj("ObjectArray", jobjectArray{nullptr});
}

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

}  // namespace
