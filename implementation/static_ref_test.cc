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
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::Field;
using ::jni::LocalObject;
using ::jni::Static;
using ::jni::StaticRef;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

////////////////////////////////////////////////////////////////////////////////
// Fields.
////////////////////////////////////////////////////////////////////////////////

static constexpr Class kClass2{"kClass2"};

// clang-format off
static constexpr Class kClass{
  "kClass",
      Static {
        Field{"booleanField", jboolean{}},
        Field{"byteField", jbyte{}},
        Field{"charField", jchar{}},
        Field{"shortField", jshort{}},
        Field{"intField", jint{}},
        Field{"longField", jlong{}},
        Field{"floatField", jfloat{}},
        Field{"doubleField", jdouble{}},
        Field{"classField", Class{"kClass2"}}
      },
};
// clang-format on

TEST_F(JniTest, StaticBooleanField) {
  jfieldID static_field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("booleanField"), StrEq("Z")))
      .WillOnce(Return(static_field_id));
  EXPECT_CALL(*env_, GetStaticBooleanField(_, static_field_id))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticBooleanField(_, static_field_id, true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}["booleanField"].Get());
  StaticRef<kClass>{}["booleanField"].Set(true);
}

TEST_F(JniTest, StaticByteField) {
  jfieldID static_field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("byteField"), StrEq("B")))
      .WillOnce(Return(static_field_id));
  EXPECT_CALL(*env_, GetStaticByteField(_, static_field_id))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticByteField(_, static_field_id, true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}["byteField"].Get());
  StaticRef<kClass>{}["byteField"].Set(true);
}

TEST_F(JniTest, StaticCharField) {
  jfieldID static_field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("charField"), StrEq("C")))
      .WillOnce(Return(static_field_id));
  EXPECT_CALL(*env_, GetStaticCharField(_, static_field_id))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticCharField(_, static_field_id, true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}["charField"].Get());
  StaticRef<kClass>{}["charField"].Set(true);
}

TEST_F(JniTest, StaticShortField) {
  jfieldID static_field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("shortField"), StrEq("S")))
      .WillOnce(Return(static_field_id));
  EXPECT_CALL(*env_, GetStaticShortField(_, static_field_id))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticShortField(_, static_field_id, true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}["shortField"].Get());
  StaticRef<kClass>{}["shortField"].Set(true);
}

TEST_F(JniTest, StaticIntField) {
  jfieldID static_field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("intField"), StrEq("I")))
      .WillOnce(Return(static_field_id));
  EXPECT_CALL(*env_, GetStaticIntField(_, static_field_id))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticIntField(_, static_field_id, true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}["intField"].Get());
  StaticRef<kClass>{}["intField"].Set(true);
}

TEST_F(JniTest, StaticLongField) {
  jfieldID static_field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("longField"), StrEq("J")))
      .WillOnce(Return(static_field_id));
  EXPECT_CALL(*env_, GetStaticLongField(_, static_field_id))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticLongField(_, static_field_id, true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}["longField"].Get());
  StaticRef<kClass>{}["longField"].Set(true);
}

TEST_F(JniTest, StaticFloatField) {
  jfieldID static_field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("floatField"), StrEq("F")))
      .WillOnce(Return(static_field_id));
  EXPECT_CALL(*env_, GetStaticFloatField(_, static_field_id))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticFloatField(_, static_field_id, true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}["floatField"].Get());
  StaticRef<kClass>{}["floatField"].Set(true);
}

TEST_F(JniTest, StaticDoubleField) {
  jfieldID static_field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("doubleField"), StrEq("D")))
      .WillOnce(Return(static_field_id));
  EXPECT_CALL(*env_, GetStaticDoubleField(_, static_field_id))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticDoubleField(_, static_field_id, true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}["doubleField"].Get());
  StaticRef<kClass>{}["doubleField"].Set(true);
}

TEST_F(JniTest, StaticField_ObjectGet) {
  jfieldID field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  jobject fake_obj{reinterpret_cast<jobject>(0XCDCDCD)};
  EXPECT_CALL(*env_,
              GetStaticFieldID(_, StrEq("classField"), StrEq("LkClass2;")))
      .WillOnce(Return(field_id));
  EXPECT_CALL(*env_, GetStaticObjectField(_, field_id))
      .WillOnce(Return(fake_obj));

  jni::LocalObject<kClass2> obj = StaticRef<kClass>{}["classField"].Get();
}

TEST_F(JniTest, StaticField_ObjectSet) {
  jfieldID field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  jobject fake_obj{reinterpret_cast<jobject>(0XCDCDCD)};
  EXPECT_CALL(*env_,
              GetStaticFieldID(_, StrEq("classField"), StrEq("LkClass2;")))
      .WillOnce(Return(field_id));
  EXPECT_CALL(*env_, SetStaticObjectField(_, field_id, fake_obj));

  StaticRef<kClass>{}["classField"].Set(LocalObject<kClass2>{fake_obj});
}

}  // namespace
