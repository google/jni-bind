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
#include "mock_jni_env.h"

namespace {

using jni::ArrayView;
using jni::LocalArray;
using jni::test::JniTest;
using testing::_;
using testing::Eq;
using testing::Return;

////////////////////////////////////////////////////////////////////////////////
// Construction Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, LocalArray_ConstructsIntArrayWithCorrectSize) {
  EXPECT_CALL(*env_, NewIntArray(1));
  EXPECT_CALL(*env_, NewIntArray(2));
  EXPECT_CALL(*env_, NewIntArray(3));
  EXPECT_CALL(*env_, NewIntArray(4));

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

/*
// TODO(b/174273621):  Add support for objects.
TEST_F(JniTest, LocalArray_ConstructsTheRightTypeForObjects) {
  static constexpr Class kClass { "kClass" };

  // Unlike POD, objects are constructed with a size, a jclass, and an init
  // object.  This makes for a slightly different API then other objects.
  EXPECT_CALL(*env_, NewObjectArray(5, _, _));
  LocalArray<jarray, kClass> local_object_array{ 5 };
}
*/

////////////////////////////////////////////////////////////////////////////////
// Accessor Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, LocalArray_CallsLengthProperly) {
  EXPECT_CALL(*env_, GetArrayLength).WillOnce(testing::Return(3));

  LocalArray<jint> local_int_array{5};
  EXPECT_EQ(local_int_array.Length(), 3);
}

TEST_F(JniTest, LocalArray_GetsAndReleaseArrayBuffer) {
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

////////////////////////////////////////////////////////////////////////////////
// Pin Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, LocalArray_PinsAnIntBufferRegion) {
  LocalArray<jint> local_int_array_1{1};
  local_int_array_1.Pin();
}

}  // namespace
