/*
 * Copyright 2022 Google LLC
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
#include <algorithm>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptLocal;
using ::jni::ArrayView;
using ::jni::CDecl_t;
using ::jni::Class;
using ::jni::Fake;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;
using ::jni::test::AsNewLocalReference;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Eq;

////////////////////////////////////////////////////////////////////////////////
// Pin Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ArrayView_CallsLengthProperly) {
  EXPECT_CALL(*env_, GetArrayLength).WillOnce(::testing::Return(3));

  LocalArray<jint> local_int_array{5};
  EXPECT_EQ(local_int_array.Length(), 3);
}

TEST_F(JniTest, ArrayView_GetsAndReleaseArrayBuffer) {
  EXPECT_CALL(*env_, GetBooleanArrayElements(Eq(Fake<jbooleanArray>()), _))
      .WillOnce(::testing::Return(Fake<jboolean*>()));
  EXPECT_CALL(*env_, ReleaseBooleanArrayElements(
                         Eq(Fake<jbooleanArray>()),
                         Eq(Fake<jboolean*>()), 0));

  EXPECT_CALL(*env_, GetByteArrayElements(Eq(Fake<jbyteArray>()), _))
      .WillOnce(::testing::Return(Fake<jbyte*>()));
  EXPECT_CALL(*env_,
              ReleaseByteArrayElements(Eq(Fake<jbyteArray>()),
                                       Eq(Fake<jbyte*>()), 0));

  EXPECT_CALL(*env_, GetCharArrayElements(Eq(Fake<jcharArray>()), _))
      .WillOnce(::testing::Return(Fake<jchar*>()));
  EXPECT_CALL(*env_,
              ReleaseCharArrayElements(Eq(Fake<jcharArray>()),
                                       Eq(Fake<jchar*>()), 0));

  EXPECT_CALL(*env_, GetShortArrayElements(Eq(Fake<jshortArray>()), _))
      .WillOnce(::testing::Return(Fake<jshort*>()));
  EXPECT_CALL(
      *env_, ReleaseShortArrayElements(Eq(Fake<jshortArray>()),
                                       Eq(Fake<jshort*>()), 0));

  EXPECT_CALL(*env_, GetIntArrayElements(Eq(Fake<jintArray>()), _))
      .WillOnce(::testing::Return(Fake<jint*>()));
  EXPECT_CALL(*env_,
              ReleaseIntArrayElements(Eq(Fake<jintArray>()),
                                      Eq(Fake<jint*>()), 0));

  EXPECT_CALL(*env_, GetLongArrayElements(Eq(Fake<jlongArray>()), _))
      .WillOnce(::testing::Return(Fake<jlong*>()));
  EXPECT_CALL(*env_,
              ReleaseLongArrayElements(Eq(Fake<jlongArray>()),
                                       Eq(Fake<jlong*>()), 0));

  EXPECT_CALL(*env_, GetFloatArrayElements(Eq(Fake<jfloatArray>()), _))
      .WillOnce(::testing::Return(Fake<jfloat*>()));
  EXPECT_CALL(
      *env_, ReleaseFloatArrayElements(Eq(Fake<jfloatArray>()),
                                       Eq(Fake<jfloat*>()), 0));

  EXPECT_CALL(*env_, GetDoubleArrayElements(Eq(Fake<jdoubleArray>()), _))
      .WillOnce(::testing::Return(Fake<jdouble*>()));
  EXPECT_CALL(*env_, ReleaseDoubleArrayElements(
                         Eq(Fake<jdoubleArray>()),
                         Eq(Fake<jdouble*>()), 0));

  LocalArray<jboolean> boolean_array{AdoptLocal{}, Fake<jbooleanArray>()};
  LocalArray<jbyte> byte_array{AdoptLocal{}, Fake<jbyteArray>()};
  LocalArray<jchar> char_array{AdoptLocal{}, Fake<jcharArray>()};
  LocalArray<jshort> short_array{AdoptLocal{}, Fake<jshortArray>()};
  LocalArray<jint> int_array{AdoptLocal{}, Fake<jintArray>()};
  LocalArray<jlong> long_array{AdoptLocal{}, Fake<jlongArray>()};
  LocalArray<jfloat> float_array{AdoptLocal{}, Fake<jfloatArray>()};
  LocalArray<jdouble> double_array{AdoptLocal{}, Fake<jdoubleArray>()};

  ArrayView<jboolean, 1> boolean_array_pin = {boolean_array.Pin()};
  ArrayView<jbyte, 1> byte_array_pin = {byte_array.Pin()};
  ArrayView<jint, 1> int_array_pin = {int_array.Pin()};
  ArrayView<jchar, 1> char_array_pin = {char_array.Pin()};
  ArrayView<jshort, 1> short_array_pin = {short_array.Pin()};
  ArrayView<jlong, 1> long_array_pin = {long_array.Pin()};
  ArrayView<jfloat, 1> float_array_pin = {float_array.Pin()};
  ArrayView<jdouble, 1> double_array_pin = {double_array.Pin()};
}

TEST_F(JniTest, LocalArrayView_AllowsCTAD) {
  EXPECT_CALL(*env_, GetBooleanArrayElements(Eq(Fake<jbooleanArray>()), _))
      .WillOnce(::testing::Return(Fake<jboolean*>()));
  EXPECT_CALL(*env_, ReleaseBooleanArrayElements(
                         Eq(Fake<jbooleanArray>()),
                         Eq(Fake<jboolean*>()), 0));

  LocalArray<jboolean> boolean_array{AdoptLocal{}, Fake<jbooleanArray>()};
  ArrayView ctad_array_view{boolean_array.Pin()};

  // Despite supporting construction from xvalue, move ctor is deleted (good).
  // ArrayView ctad_array_view_2 {std::move(ctad_array_view)};
}

TEST_F(JniTest, ArrayView_ConstructsFromAnObject) {
  static constexpr Class kClass{"kClass"};
  LocalArray<jobject, 1, kClass> local_obj_array{1, LocalObject<kClass>{}};
}

TEST_F(JniTest, ArrayView_ConstructsFromAnObjectRValueWithCTAD) {
  static constexpr Class kClass{"kClass"};
  LocalArray<jobject, 1, kClass> local_obj_array{1, LocalObject<kClass>{}};
}

TEST_F(JniTest, ArrayView_GetsAnObject) {
  static constexpr Class kClass{"kClass"};

  EXPECT_CALL(*env_, GetObjectArrayElement(_, _));
  LocalArray<jobject, 1, kClass> local_obj_array{1, LocalObject<kClass>{}};
  local_obj_array.Get(0);
}

TEST_F(JniTest, ArrayView_GetsAnObjectWithCTAD) {
  static constexpr Class kClass{"kClass"};

  EXPECT_CALL(*env_, GetObjectArrayElement(_, _));
  LocalArray<jobject, 1, kClass> local_obj_array{1, LocalObject<kClass>{}};
  local_obj_array.Get(0);
}

////////////////////////////////////////////////////////////////////////////////
// Iteration Tests: Primitives.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ArrayView_BooleanIsIterable) {
  std::array fake_vals{jboolean{true}, jboolean{false}, jboolean{true}};
  EXPECT_CALL(*env_, NewBooleanArray(3))
      .WillOnce(::testing::Return(Fake<jbooleanArray>()));
  EXPECT_CALL(*env_, GetArrayLength(Fake<jbooleanArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetBooleanArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jboolean> bool_arr{3};
  ArrayView<jboolean, 1> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_ByteIsIterable) {
  std::array fake_vals{jbyte{true}, jbyte{false}, jbyte{true}};
  EXPECT_CALL(*env_, NewByteArray(3))
      .WillOnce(::testing::Return(Fake<jbyteArray>()));
  EXPECT_CALL(*env_, GetArrayLength(Fake<jbyteArray>()))
      .WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetByteArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jbyte> bool_arr{3};
  ArrayView<jbyte, 1> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_CharIsIterable) {
  std::array fake_vals{jchar{true}, jchar{false}, jchar{true}};
  EXPECT_CALL(*env_, NewCharArray(3))
      .WillOnce(::testing::Return(Fake<jcharArray>()));
  EXPECT_CALL(*env_, GetArrayLength(Fake<jcharArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetCharArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jchar> bool_arr{3};
  ArrayView<jchar, 1> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_ShortIsIterable) {
  std::array fake_vals{jshort{true}, jshort{false}, jshort{true}};
  EXPECT_CALL(*env_, NewShortArray(3))
      .WillOnce(::testing::Return(Fake<jshortArray>()));
  EXPECT_CALL(*env_, GetArrayLength(Fake<jshortArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetShortArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jshort> bool_arr{3};
  ArrayView<jshort, 1> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_IntIsIterable) {
  std::array fake_vals{jint{1}, jint{2}, jint{3}};

  EXPECT_CALL(*env_, NewIntArray(3))
      .WillOnce(::testing::Return(Fake<jintArray>()));
  EXPECT_CALL(*env_, GetArrayLength(Fake<jintArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetIntArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jint> int_arr{3};
  ArrayView<jint, 1> int_view = int_arr.Pin();

  EXPECT_TRUE(std::equal(int_view.begin(), int_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_LongIsIterable) {
  std::array fake_vals{jlong{true}, jlong{false}, jlong{true}};
  EXPECT_CALL(*env_, NewLongArray(3))
      .WillOnce(::testing::Return(Fake<jlongArray>()));
  EXPECT_CALL(*env_, GetArrayLength(Fake<jlongArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetLongArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jlong> bool_arr{3};
  ArrayView<jlong, 1> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_FloatIsIterable) {
  std::array fake_vals{jfloat{true}, jfloat{false}, jfloat{true}};

  EXPECT_CALL(*env_, NewFloatArray(3))
      .WillOnce(::testing::Return(Fake<jfloatArray>()));
  EXPECT_CALL(*env_, GetArrayLength(Fake<jfloatArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetFloatArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jfloat> bool_arr{3};
  ArrayView<jfloat, 1> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_DoubleIsIterable) {
  std::array fake_vals{jdouble{true}, jdouble{false}, jdouble{true}};
  EXPECT_CALL(*env_, NewDoubleArray(3))
      .WillOnce(::testing::Return(Fake<jdoubleArray>()));
  EXPECT_CALL(*env_, GetArrayLength(Fake<jdoubleArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetDoubleArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jdouble> bool_arr{3};
  ArrayView<jdouble, 1> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

////////////////////////////////////////////////////////////////////////////////
// Iteration Tests: Objects.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ArrayView_ShallowObjectsAreIterable) {
  std::array fake_vals{Fake<jobject>(1), Fake<jobject>(2), Fake<jobject>(3)};

  EXPECT_CALL(*env_, GetArrayLength(Fake<jobjectArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(::testing::Return(Fake<jobject>(1)))
      .WillOnce(::testing::Return(Fake<jobject>(2)))
      .WillOnce(::testing::Return(Fake<jobject>(3)));

  LocalArray<jobject> obj_arr{AdoptLocal{}, Fake<jobjectArray>()};
  ArrayView<jobject, 1> obj_view = obj_arr.Pin();

  EXPECT_TRUE(std::equal(obj_view.begin(), obj_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_RichObjectsAreIterable) {
  static constexpr Class kClass{"kClass", Method{"Foo", Return<int>{}}};

  EXPECT_CALL(*env_, GetArrayLength(Fake<jobjectArray>()))
      .WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(::testing::Return(Fake<jobject>(1)))
      .WillOnce(::testing::Return(Fake<jobject>(2)))
      .WillOnce(::testing::Return(Fake<jobject>(3)));

  LocalArray<jobject, 1, kClass> obj_arr{AdoptLocal{}, Fake<jobjectArray>()};
  auto obj_view = obj_arr.Pin();

  // Note: GlobalObject will fail to compile here. This is good, the user
  // should be forced to explicitly promote the local.
  int fake_result = 123;
  for (LocalObject<kClass> obj : obj_view) {
    EXPECT_CALL(*env_, CallIntMethodV).WillOnce(::testing::Return(fake_result));
    EXPECT_EQ(obj("Foo"), fake_result);
    fake_result++;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Iteration Tests: Rank 2 Iterations.
//
// Note: Writing through every type would be tedious, however, if these tests
//   could be generalised across the universe of types it would be better.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ArrayView_Rank2IntArraysAreIterable) {
  std::array fake_vals{Fake<jintArray>(1), Fake<jintArray>(2),
                       Fake<jintArray>(3)};

  EXPECT_CALL(*env_, GetArrayLength(Fake<jobjectArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 0))
      .WillOnce(::testing::Return(Fake<jintArray>(1)));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 1))
      .WillOnce(::testing::Return(Fake<jintArray>(2)));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 2))
      .WillOnce(::testing::Return(Fake<jintArray>(3)));

  LocalArray<jint, 2> int_arr_rank_2{AdoptLocal{}, Fake<jobjectArray>()};
  ArrayView<jint, 2> int_rank2_view = int_arr_rank_2.Pin();

  EXPECT_TRUE(std::equal(int_rank2_view.begin(), int_rank2_view.end(),
                         fake_vals.begin(), fake_vals.end()));

  /*
  // Also viable to write this:
  // for (LocalArray<jint, 1> jint_array : int_rank2_view) { }
  */
}

TEST_F(JniTest, ArrayView_Rank2ObjectkArraysAreIterable) {
  std::array fake_vals{Fake<jobjectArray>(1), Fake<jobjectArray>(2),
                       Fake<jobjectArray>(3)};

  EXPECT_CALL(*env_, GetArrayLength(Fake<jobjectArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 0))
      .WillOnce(::testing::Return(Fake<jobjectArray>(1)));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 1))
      .WillOnce(::testing::Return(Fake<jobjectArray>(2)));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 2))
      .WillOnce(::testing::Return(Fake<jobjectArray>(3)));

  LocalArray<jobject, 2> int_arr_rank_2{AdoptLocal{}, Fake<jobjectArray>()};
  ArrayView<jobject, 2> int_rank2_view = int_arr_rank_2.Pin();

  EXPECT_TRUE(std::equal(int_rank2_view.begin(), int_rank2_view.end(),
                         fake_vals.begin(), fake_vals.end()));

  /*
  // Also viable to write this:
  // for (LocalArray<jint, 1> jint_array : int_rank2_view) { }
  */
}

////////////////////////////////////////////////////////////////////////////////
// Iteration Tests: Rank 3 Iterations.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ArrayView_Rank3IntArraysAreIterable) {
  std::array fake_vals{Fake<jobjectArray>(), Fake<jobjectArray>(),
                       Fake<jobjectArray>()};

  EXPECT_CALL(*env_, GetArrayLength(Fake<jobjectArray>()))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 0))
      .WillOnce(::testing::Return(Fake<jobjectArray>()));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 1))
      .WillOnce(::testing::Return(Fake<jobjectArray>()));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 2))
      .WillOnce(::testing::Return(Fake<jobjectArray>()));

  LocalArray<jint, 3> int_arr_rank_3{AdoptLocal{}, Fake<jobjectArray>()};
  ArrayView<jint, 3> int_rank_3_view = int_arr_rank_3.Pin();

  EXPECT_TRUE(std::equal(int_rank_3_view.begin(), int_rank_3_view.end(),
                         fake_vals.begin(), fake_vals.end()));

  // Also viable to write this:
  // for (LocalArray<jint, 1> jint_array : int_rank_3_view) {  }
}

TEST_F(JniTest, ArrayView_Rank3ObjectkArraysAreIterable) {
  std::array fake_vals{Fake<jobjectArray>(1), Fake<jobjectArray>(2),
                       Fake<jobjectArray>(3)};

  EXPECT_CALL(*env_, GetArrayLength(Fake<jobjectArray>(0)))
      .WillOnce(::testing::Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 0))
      .WillOnce(::testing::Return(Fake<jobjectArray>(1)));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 1))
      .WillOnce(::testing::Return(Fake<jobjectArray>(2)));
  EXPECT_CALL(*env_, GetObjectArrayElement(
                         AsNewLocalReference(Fake<jobjectArray>()), 2))
      .WillOnce(::testing::Return(Fake<jobjectArray>(3)));

  LocalArray<jobject, 3> object_arr_rank_3{AdoptLocal{}, Fake<jobjectArray>(0)};
  ArrayView<jobject, 3> object_rank_3_view = object_arr_rank_3.Pin();

  EXPECT_TRUE(std::equal(object_rank_3_view.begin(), object_rank_3_view.end(),
                         fake_vals.begin(), fake_vals.end()));

  // Also viable to write this:
  // for (LocalArray<jobject, 2> jobject_array : object_rank_3_view) { }
}

}  // namespace
