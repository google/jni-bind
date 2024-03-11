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
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptLocal;
using ::jni::Array;
using ::jni::Class;
using ::jni::Fake;
using ::jni::Field;
using ::jni::LocalObject;
using ::jni::Rank;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};

static constexpr Class kRank2{
    "kFieldClass",
    Field{"BooleanArray", Array{jboolean{}, Rank<2>{}}},
    Field{"ByteArray", Array{jbyte{}, Rank<2>{}}},
    Field{"CharArray", Array{jchar{}, Rank<2>{}}},
    Field{"ShortArray", Array{jshort{}, Rank<2>{}}},
    Field{"IntArray", Array{jint{}, Rank<2>{}}},
    Field{"FloatArray", Array{jfloat{}, Rank<2>{}}},
    Field{"DoubleArray", Array{jdouble{}, Rank<2>{}}},
    Field{"LongArray", Array{jlong{}, Rank<2>{}}},
    Field{"ObjectArray", Array{kClass, Rank<2>{}}},
};

TEST_F(JniTest, LocalArrayField_Rank2) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("BooleanArray"), StrEq("[[Z")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ByteArray"), StrEq("[[B")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("CharArray"), StrEq("[[C")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ShortArray"), StrEq("[[S")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("IntArray"), StrEq("[[I")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("FloatArray"), StrEq("[[F")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("DoubleArray"), StrEq("[[D")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("LongArray"), StrEq("[[J")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ObjectArray"), StrEq("[[LkClass;")));

  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillOnce(::testing::Return(Fake<jobjectArray>(1)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(2)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(3)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(4)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(5)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(6)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(7)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(8)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(9)));

  LocalObject<kRank2> obj{AdoptLocal{}, Fake<jobject>()};

  EXPECT_EQ(static_cast<jobjectArray>(obj["BooleanArray"].Get()),
            Fake<jobjectArray>(1));
  EXPECT_EQ(static_cast<jobjectArray>(obj["ByteArray"].Get()),
            Fake<jobjectArray>(2));
  EXPECT_EQ(static_cast<jobjectArray>(obj["CharArray"].Get()),
            Fake<jobjectArray>(3));
  EXPECT_EQ(static_cast<jobjectArray>(obj["ShortArray"].Get()),
            Fake<jobjectArray>(4));
  EXPECT_EQ(static_cast<jobjectArray>(obj["IntArray"].Get()),
            Fake<jobjectArray>(5));
  EXPECT_EQ(static_cast<jobjectArray>(obj["FloatArray"].Get()),
            Fake<jobjectArray>(6));
  EXPECT_EQ(static_cast<jobjectArray>(obj["DoubleArray"].Get()),
            Fake<jobjectArray>(7));
  EXPECT_EQ(static_cast<jobjectArray>(obj["LongArray"].Get()),
            Fake<jobjectArray>(8));
  EXPECT_EQ(static_cast<jobjectArray>(obj["ObjectArray"].Get()),
            Fake<jobjectArray>(9));
}

static constexpr Class kRank3{
    "kFieldClass",
    Field{"BooleanArray", Array{jboolean{}, Rank<3>{}}},
    Field{"ByteArray", Array{jbyte{}, Rank<3>{}}},
    Field{"CharArray", Array{jchar{}, Rank<3>{}}},
    Field{"ShortArray", Array{jshort{}, Rank<3>{}}},
    Field{"IntArray", Array{jint{}, Rank<3>{}}},
    Field{"FloatArray", Array{jfloat{}, Rank<3>{}}},
    Field{"DoubleArray", Array{jdouble{}, Rank<3>{}}},
    Field{"LongArray", Array{jlong{}, Rank<3>{}}},
    Field{"ObjectArray", Array{kClass, Rank<3>{}}},
};

TEST_F(JniTest, LocalArrayField_Rank3) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("BooleanArray"), StrEq("[[[Z")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ByteArray"), StrEq("[[[B")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("CharArray"), StrEq("[[[C")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ShortArray"), StrEq("[[[S")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("IntArray"), StrEq("[[[I")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("FloatArray"), StrEq("[[[F")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("DoubleArray"), StrEq("[[[D")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("LongArray"), StrEq("[[[J")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ObjectArray"), StrEq("[[[LkClass;")));

  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillOnce(::testing::Return(Fake<jobjectArray>(1)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(2)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(3)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(4)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(5)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(6)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(7)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(8)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(9)));

  LocalObject<kRank3> obj{AdoptLocal{}, Fake<jobject>()};

  EXPECT_EQ(static_cast<jobjectArray>(obj["BooleanArray"].Get()),
            Fake<jobjectArray>(1));
  EXPECT_EQ(static_cast<jobjectArray>(obj["ByteArray"].Get()),
            Fake<jobjectArray>(2));
  EXPECT_EQ(static_cast<jobjectArray>(obj["CharArray"].Get()),
            Fake<jobjectArray>(3));
  EXPECT_EQ(static_cast<jobjectArray>(obj["ShortArray"].Get()),
            Fake<jobjectArray>(4));
  EXPECT_EQ(static_cast<jobjectArray>(obj["IntArray"].Get()),
            Fake<jobjectArray>(5));
  EXPECT_EQ(static_cast<jobjectArray>(obj["FloatArray"].Get()),
            Fake<jobjectArray>(6));
  EXPECT_EQ(static_cast<jobjectArray>(obj["DoubleArray"].Get()),
            Fake<jobjectArray>(7));
  EXPECT_EQ(static_cast<jobjectArray>(obj["LongArray"].Get()),
            Fake<jobjectArray>(8));
  EXPECT_EQ(static_cast<jobjectArray>(obj["ObjectArray"].Get()),
            Fake<jobjectArray>(9));
}

}  // namespace
