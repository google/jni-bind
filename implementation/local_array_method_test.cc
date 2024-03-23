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
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Array;
using ::jni::Class;
using ::jni::Fake;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

static constexpr Class kClass2{"kClass2"};

////////////////////////////////////////////////////////////////////////////////
// As Return.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ReturnSmokeTest) {
  static constexpr Class kClass{
      "kClass",
      Method{"BooleanArray", Return{Array{jboolean{}}}},
      Method{"ByteArray", Return{Array{jbyte{}}}},
      Method{"CharArray", Return{Array{jchar{}}}},
      Method{"ShortArray", Return{Array{jshort{}}}},
      Method{"IntArray", Return{Array{jint{}}}},
      Method{"FloatArray", Return{Array{jfloat{}}}},
      Method{"DoubleArray", Return{Array{jdouble{}}}},
      Method{"LongArray", Return{Array{jlong{}}}},
      Method{"ObjectArray", Return{Array{kClass2}}},
  };

  EXPECT_CALL(*env_, CallObjectMethodV)
      .WillOnce(testing::Return(Fake<jbooleanArray>()))
      .WillOnce(testing::Return(Fake<jbyteArray>()))
      .WillOnce(testing::Return(Fake<jcharArray>()))
      .WillOnce(testing::Return(Fake<jshortArray>()))
      .WillOnce(testing::Return(Fake<jintArray>()))
      .WillOnce(testing::Return(Fake<jlongArray>()))
      .WillOnce(testing::Return(Fake<jfloatArray>()))
      .WillOnce(testing::Return(Fake<jdoubleArray>()))
      .WillOnce(testing::Return(Fake<jobjectArray>()));

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("BooleanArray"), StrEq("()[Z")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ByteArray"), StrEq("()[B")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("CharArray"), StrEq("()[C")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ShortArray"), StrEq("()[S")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("()[I")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("LongArray"), StrEq("()[J")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("FloatArray"), StrEq("()[F")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("DoubleArray"), StrEq("()[D")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("()[LkClass2;")));

  LocalArray<jboolean> bool_array{obj("BooleanArray")};
  EXPECT_EQ((static_cast<jbooleanArray>(bool_array)), (Fake<jbooleanArray>()));

  LocalArray<jbyte> byte_array{obj("ByteArray")};
  EXPECT_EQ((static_cast<jbyteArray>(byte_array)), (Fake<jbyteArray>()));

  LocalArray<jchar> char_array{obj("CharArray")};
  EXPECT_EQ((static_cast<jcharArray>(char_array)), (Fake<jcharArray>()));

  LocalArray<jshort> short_array{obj("ShortArray")};
  EXPECT_EQ((static_cast<jshortArray>(short_array)), (Fake<jshortArray>()));

  LocalArray<jint> int_array{obj("IntArray")};
  EXPECT_EQ((static_cast<jintArray>(int_array)), (Fake<jintArray>()));

  LocalArray<jlong> long_array{obj("LongArray")};
  EXPECT_EQ((static_cast<jlongArray>(long_array)), (Fake<jlongArray>()));

  LocalArray<jfloat> float_array{obj("FloatArray")};
  EXPECT_EQ((static_cast<jfloatArray>(float_array)), (Fake<jfloatArray>()));

  LocalArray<jdouble> double_array{obj("DoubleArray")};
  EXPECT_EQ((static_cast<jdoubleArray>(double_array)), (Fake<jdoubleArray>()));

  LocalArray<jobject, 1, kClass2> object_array{obj("ObjectArray")};
  EXPECT_EQ((static_cast<jobjectArray>(object_array)), (Fake<jobjectArray>()));
}

////////////////////////////////////////////////////////////////////////////////
// As Params.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ParamsSmokeTest) {
  static constexpr Class kClass{
      "kClass",
      Method{"BooleanArray", Return{}, Params{Array{jboolean{}}}},
      Method{"ByteArray", Return{}, Params{Array{jbyte{}}}},
      Method{"CharArray", Return{}, Params{Array{jchar{}}}},
      Method{"ShortArray", Return{}, Params{Array{jshort{}}}},
      Method{"IntArray", Return{}, Params{Array{jint{}}}},
      Method{"FloatArray", Return{}, Params{Array{jfloat{}}}},
      Method{"DoubleArray", Return{}, Params{Array{jdouble{}}}},
      Method{"LongArray", Return{}, Params{Array{jlong{}}}},
      Method{"ObjectArray", Return{}, Params{Array{kClass2}}},
  };

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("BooleanArray"), StrEq("([Z)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ByteArray"), StrEq("([B)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("CharArray"), StrEq("([C)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ShortArray"), StrEq("([S)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("([I)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("FloatArray"), StrEq("([F)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("DoubleArray"), StrEq("([D)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("LongArray"), StrEq("([J)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("ObjectArray"), StrEq("([LkClass2;)V")));

  obj("BooleanArray", LocalArray<jboolean>{Fake<jbooleanArray>()});
  obj("ByteArray", LocalArray<jbyte>{Fake<jbyteArray>()});
  obj("CharArray", LocalArray<jchar>{Fake<jcharArray>()});
  obj("ShortArray", LocalArray<jshort>{Fake<jshortArray>()});
  obj("IntArray", LocalArray<jint>{Fake<jintArray>()});
  obj("FloatArray", LocalArray<jfloat>{Fake<jfloatArray>()});
  obj("DoubleArray", LocalArray<jdouble>{Fake<jdoubleArray>()});
  obj("LongArray", LocalArray<jlong>{Fake<jlongArray>()});
  obj("ObjectArray", LocalArray<jobject, 1, kClass2>{Fake<jobjectArray>()});
}

////////////////////////////////////////////////////////////////////////////////
// As Complex.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, ComplexSmokeTest) {
  static constexpr Class kClass{
      "kClass",
      Method{"BooleanArray", Return{Array{jboolean{}}},
             Params{Array{jboolean{}}}},
      Method{"ByteArray", Return{Array{jbyte{}}}, Params{Array{jbyte{}}}},
      Method{"CharArray", Return{Array{jchar{}}}, Params{Array{jchar{}}}},
      Method{"ShortArray", Return{Array{jshort{}}}, Params{Array{jshort{}}}},
      Method{"IntArray", Return{Array{jint{}}}, Params{Array{jint{}}}},
      Method{"FloatArray", Return{Array{jfloat{}}}, Params{Array{jfloat{}}}},
      Method{"DoubleArray", Return{Array{jdouble{}}}, Params{Array{jdouble{}}}},
      Method{"LongArray", Return{Array{jlong{}}}, Params{Array{jlong{}}}},
      Method{"ObjectArray", Return{Array{kClass2}}, Params{Array{kClass2}}},
  };

  EXPECT_CALL(*env_, CallObjectMethodV)
      .WillOnce(testing::Return(Fake<jbooleanArray>()))
      .WillOnce(testing::Return(Fake<jbyteArray>()))
      .WillOnce(testing::Return(Fake<jcharArray>()))
      .WillOnce(testing::Return(Fake<jshortArray>()))
      .WillOnce(testing::Return(Fake<jintArray>()))
      .WillOnce(testing::Return(Fake<jfloatArray>()))
      .WillOnce(testing::Return(Fake<jdoubleArray>()))
      .WillOnce(testing::Return(Fake<jlongArray>()))
      .WillOnce(testing::Return(Fake<jobjectArray>()));

  LocalObject<kClass> obj{Fake<jobject>()};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("BooleanArray"), StrEq("([Z)[Z")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ByteArray"), StrEq("([B)[B")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("CharArray"), StrEq("([C)[C")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ShortArray"), StrEq("([S)[S")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("IntArray"), StrEq("([I)[I")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("FloatArray"), StrEq("([F)[F")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("DoubleArray"), StrEq("([D)[D")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("LongArray"), StrEq("([J)[J")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("ObjectArray"),
                                 StrEq("([LkClass2;)[LkClass2;")));

  LocalArray<jboolean> bool_array{
      obj("BooleanArray", LocalArray<jboolean>{Fake<jbooleanArray>()})};
  EXPECT_EQ((static_cast<jbooleanArray>(bool_array)), (Fake<jbooleanArray>()));

  LocalArray<jbyte> byte_array{
      obj("ByteArray", LocalArray<jbyte>{Fake<jbyteArray>()})};
  EXPECT_EQ((static_cast<jbyteArray>(byte_array)), (Fake<jbyteArray>()));

  LocalArray<jchar> char_array{
      obj("CharArray", LocalArray<jchar>{Fake<jcharArray>()})};
  EXPECT_EQ((static_cast<jcharArray>(char_array)), (Fake<jcharArray>()));

  LocalArray<jshort> short_array{
      obj("ShortArray", LocalArray<jshort>{Fake<jshortArray>()})};
  EXPECT_EQ((static_cast<jshortArray>(short_array)), (Fake<jshortArray>()));

  LocalArray<jint> int_array{
      obj("IntArray", LocalArray<jint>{Fake<jintArray>()})};
  EXPECT_EQ((static_cast<jintArray>(int_array)), (Fake<jintArray>()));

  LocalArray<jfloat> float_array{
      obj("FloatArray", LocalArray<jfloat>{Fake<jfloatArray>()})};
  EXPECT_EQ((static_cast<jfloatArray>(float_array)), (Fake<jfloatArray>()));

  LocalArray<jdouble> double_array{
      obj("DoubleArray", LocalArray<jdouble>{Fake<jdoubleArray>()})};
  EXPECT_EQ((static_cast<jdoubleArray>(double_array)), (Fake<jdoubleArray>()));

  LocalArray<jlong> long_array{
      obj("LongArray", LocalArray<jlong>{Fake<jlongArray>()})};
  EXPECT_EQ((static_cast<jlongArray>(long_array)), (Fake<jlongArray>()));

  LocalArray<jobject, 1, kClass2> object_array{obj(
      "ObjectArray", LocalArray<jobject, 1, kClass2>{Fake<jobjectArray>()})};
  EXPECT_EQ((static_cast<jobjectArray>(object_array)), (Fake<jobjectArray>()));
}

}  // namespace
