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
#include "implementation/array_view.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using jni::ArrayView;
using jni::CDecl_t;
using jni::Class;
using jni::LocalArray;
using jni::LocalObject;
using jni::test::JniTest;
using testing::_;
using testing::Eq;
using testing::Return;

////////////////////////////////////////////////////////////////////////////////
// Pin Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ArrayView_CallsLengthProperly) {
  EXPECT_CALL(*env_, GetArrayLength).WillOnce(testing::Return(3));

  LocalArray<jint> local_int_array{5};
  EXPECT_EQ(local_int_array.Length(), 3);
}

TEST_F(JniTest, ArrayView_GetsAndReleaseArrayBuffer) {
  jbooleanArray fake_boolean_jobject{reinterpret_cast<jbooleanArray>(0xaaaaaa)};
  jbyteArray fake_byte_jobject{reinterpret_cast<jbyteArray>(0xaaaaab)};
  jcharArray fake_char_jobject{reinterpret_cast<jcharArray>(0xaaaaac)};
  jshortArray fake_short_jobject{reinterpret_cast<jshortArray>(0xaaaaad)};
  jintArray fake_int_jobject{reinterpret_cast<jintArray>(0xaaaaae)};
  jlongArray fake_long_jobject{reinterpret_cast<jlongArray>(0xaaaaaf)};
  jfloatArray fake_float_jobject{reinterpret_cast<jfloatArray>(0xaaaab0)};
  jdoubleArray fake_double_jobject{reinterpret_cast<jdoubleArray>(0xaaaab1)};

  jboolean* fake_raw_boolean_ptr{reinterpret_cast<jboolean*>(0xbaaaaaaa)};
  jbyte* fake_raw_byte_ptr{reinterpret_cast<jbyte*>(0xbaaaaaab)};
  jchar* fake_raw_char_ptr{reinterpret_cast<jchar*>(0xbaaaaaac)};
  jshort* fake_raw_short_ptr{reinterpret_cast<jshort*>(0xbaaaaaad)};
  jint* fake_raw_int_ptr{reinterpret_cast<jint*>(0xbaaaaaae)};
  jlong* fake_raw_long_ptr{reinterpret_cast<jlong*>(0xbaaaaaaf)};
  jfloat* fake_raw_float_ptr{reinterpret_cast<jfloat*>(0xbaaaaab0)};
  jdouble* fake_raw_double_ptr{reinterpret_cast<jdouble*>(0xbaaaaab1)};

  EXPECT_CALL(*env_, GetBooleanArrayElements(Eq(fake_boolean_jobject), _))
      .WillOnce(Return(fake_raw_boolean_ptr));
  EXPECT_CALL(*env_, ReleaseBooleanArrayElements(Eq(fake_boolean_jobject),
                                                 Eq(fake_raw_boolean_ptr), 0));

  EXPECT_CALL(*env_, GetByteArrayElements(Eq(fake_byte_jobject), _))
      .WillOnce(Return(fake_raw_byte_ptr));
  EXPECT_CALL(*env_, ReleaseByteArrayElements(Eq(fake_byte_jobject),
                                              Eq(fake_raw_byte_ptr), 0));

  EXPECT_CALL(*env_, GetCharArrayElements(Eq(fake_char_jobject), _))
      .WillOnce(Return(fake_raw_char_ptr));
  EXPECT_CALL(*env_, ReleaseCharArrayElements(Eq(fake_char_jobject),
                                              Eq(fake_raw_char_ptr), 0));

  EXPECT_CALL(*env_, GetShortArrayElements(Eq(fake_short_jobject), _))
      .WillOnce(Return(fake_raw_short_ptr));
  EXPECT_CALL(*env_, ReleaseShortArrayElements(Eq(fake_short_jobject),
                                               Eq(fake_raw_short_ptr), 0));

  EXPECT_CALL(*env_, GetIntArrayElements(Eq(fake_int_jobject), _))
      .WillOnce(Return(fake_raw_int_ptr));
  EXPECT_CALL(*env_, ReleaseIntArrayElements(Eq(fake_int_jobject),
                                             Eq(fake_raw_int_ptr), 0));

  EXPECT_CALL(*env_, GetLongArrayElements(Eq(fake_long_jobject), _))
      .WillOnce(Return(fake_raw_long_ptr));
  EXPECT_CALL(*env_, ReleaseLongArrayElements(Eq(fake_long_jobject),
                                              Eq(fake_raw_long_ptr), 0));

  EXPECT_CALL(*env_, GetFloatArrayElements(Eq(fake_float_jobject), _))
      .WillOnce(Return(fake_raw_float_ptr));
  EXPECT_CALL(*env_, ReleaseFloatArrayElements(Eq(fake_float_jobject),
                                               Eq(fake_raw_float_ptr), 0));

  EXPECT_CALL(*env_, GetDoubleArrayElements(Eq(fake_double_jobject), _))
      .WillOnce(Return(fake_raw_double_ptr));
  EXPECT_CALL(*env_, ReleaseDoubleArrayElements(Eq(fake_double_jobject),
                                                Eq(fake_raw_double_ptr), 0));

  LocalArray<jboolean> boolean_array{fake_boolean_jobject};
  LocalArray<jbyte> byte_array{fake_byte_jobject};
  LocalArray<jchar> char_array{fake_char_jobject};
  LocalArray<jshort> short_array{fake_short_jobject};
  LocalArray<jint> int_array{fake_int_jobject};
  LocalArray<jlong> long_array{fake_long_jobject};
  LocalArray<jfloat> float_array{fake_float_jobject};
  LocalArray<jdouble> double_array{fake_double_jobject};

  ArrayView<jboolean> boolean_array_pin = {boolean_array.Pin()};
  ArrayView<jbyte> byte_array_pin = {byte_array.Pin()};
  ArrayView<jint> int_array_pin = {int_array.Pin()};
  ArrayView<jchar> char_array_pin = {char_array.Pin()};
  ArrayView<jshort> short_array_pin = {short_array.Pin()};
  ArrayView<jlong> long_array_pin = {long_array.Pin()};
  ArrayView<jfloat> float_array_pin = {float_array.Pin()};
  ArrayView<jdouble> double_array_pin = {double_array.Pin()};
}

TEST_F(JniTest, LocalArrayView_AllowsCTAD) {
  jbooleanArray fake_boolean_jobject{reinterpret_cast<jbooleanArray>(0xaaaaaa)};
  jboolean* fake_raw_boolean_ptr{reinterpret_cast<jboolean*>(0xbaaaaaaa)};

  EXPECT_CALL(*env_, GetBooleanArrayElements(Eq(fake_boolean_jobject), _))
      .WillOnce(Return(fake_raw_boolean_ptr));
  EXPECT_CALL(*env_, ReleaseBooleanArrayElements(Eq(fake_boolean_jobject),
                                                 Eq(fake_raw_boolean_ptr), 0));

  LocalArray<jboolean> boolean_array{fake_boolean_jobject};
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
  const jbooleanArray fake_jarr = reinterpret_cast<jbooleanArray>(0xDADADADADA);

  EXPECT_CALL(*env_, NewBooleanArray(3)).WillOnce(::testing::Return(fake_jarr));
  EXPECT_CALL(*env_, GetArrayLength(fake_jarr)).WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetBooleanArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jboolean> bool_arr{3};
  ArrayView<jboolean> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_ByteIsIterable) {
  std::array fake_vals{jbyte{true}, jbyte{false}, jbyte{true}};
  const jbyteArray fake_jarr = reinterpret_cast<jbyteArray>(0xDADADADADA);

  EXPECT_CALL(*env_, NewByteArray(3)).WillOnce(::testing::Return(fake_jarr));
  EXPECT_CALL(*env_, GetArrayLength(fake_jarr)).WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetByteArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jbyte> bool_arr{3};
  ArrayView<jbyte> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_CharIsIterable) {
  std::array fake_vals{jchar{true}, jchar{false}, jchar{true}};
  const jcharArray fake_jarr = reinterpret_cast<jcharArray>(0xDADADADADA);

  EXPECT_CALL(*env_, NewCharArray(3)).WillOnce(::testing::Return(fake_jarr));
  EXPECT_CALL(*env_, GetArrayLength(fake_jarr)).WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetCharArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jchar> bool_arr{3};
  ArrayView<jchar> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_ShortIsIterable) {
  std::array fake_vals{jshort{true}, jshort{false}, jshort{true}};
  const jshortArray fake_jarr = reinterpret_cast<jshortArray>(0xDADADADADA);

  EXPECT_CALL(*env_, NewShortArray(3)).WillOnce(::testing::Return(fake_jarr));
  EXPECT_CALL(*env_, GetArrayLength(fake_jarr)).WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetShortArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jshort> bool_arr{3};
  ArrayView<jshort> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_IntIsIterable) {
  std::array fake_vals{jint{1}, jint{2}, jint{3}};
  const jintArray fake_jarr = reinterpret_cast<jintArray>(0xDADADADADA);

  EXPECT_CALL(*env_, NewIntArray(3)).WillOnce(Return(fake_jarr));
  EXPECT_CALL(*env_, GetArrayLength(fake_jarr)).WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetIntArrayElements)
      .WillOnce(testing::Return(fake_vals.data()));

  LocalArray<jint> int_arr{3};
  ArrayView<jint> int_view = int_arr.Pin();

  EXPECT_TRUE(std::equal(int_view.begin(), int_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_LongIsIterable) {
  std::array fake_vals{jlong{true}, jlong{false}, jlong{true}};
  const jlongArray fake_jarr = reinterpret_cast<jlongArray>(0xDADADADADA);

  EXPECT_CALL(*env_, NewLongArray(3)).WillOnce(::testing::Return(fake_jarr));
  EXPECT_CALL(*env_, GetArrayLength(fake_jarr)).WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetLongArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jlong> bool_arr{3};
  ArrayView<jlong> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_FloatIsIterable) {
  std::array fake_vals{jfloat{true}, jfloat{false}, jfloat{true}};
  const jfloatArray fake_jarr = reinterpret_cast<jfloatArray>(0xDADADADADA);

  EXPECT_CALL(*env_, NewFloatArray(3)).WillOnce(::testing::Return(fake_jarr));
  EXPECT_CALL(*env_, GetArrayLength(fake_jarr)).WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetFloatArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jfloat> bool_arr{3};
  ArrayView<jfloat> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

TEST_F(JniTest, ArrayView_DoubleIsIterable) {
  std::array fake_vals{jdouble{true}, jdouble{false}, jdouble{true}};
  const jdoubleArray fake_jarr = reinterpret_cast<jdoubleArray>(0xDADADADADA);

  EXPECT_CALL(*env_, NewDoubleArray(3)).WillOnce(::testing::Return(fake_jarr));
  EXPECT_CALL(*env_, GetArrayLength(fake_jarr)).WillOnce(testing::Return(3));
  EXPECT_CALL(*env_, GetDoubleArrayElements)
      .WillOnce(::testing::Return(fake_vals.data()));

  LocalArray<jdouble> bool_arr{3};
  ArrayView<jdouble> bool_view = bool_arr.Pin();

  EXPECT_TRUE(std::equal(bool_view.begin(), bool_view.end(), fake_vals.begin(),
                         fake_vals.end()));
}

}  // namespace
