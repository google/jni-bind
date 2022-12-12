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

using ::jni::Array;
using ::jni::Class;
using ::jni::Field;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
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

////////////////////////////////////////////////////////////////////////////////
// Static Methods.
////////////////////////////////////////////////////////////////////////////////

// clang-format off
static constexpr Class kMethodClass{
  "kMethodClass",
      Static {
        Method{"booleanMethod", ::jni::Return{jboolean{}}, Params<>{}},
        Method{"byteMethod", ::jni::Return{jbyte{}}, Params<>{}},
        Method{"charMethod", ::jni::Return{jchar{}}, Params<>{}},
        Method{"shortMethod", ::jni::Return{jshort{}}, Params<>{}},
        Method{"intMethod", ::jni::Return{jint{}}, Params<>{}},
        Method{"longMethod", ::jni::Return{jlong{}}, Params<>{}},
        Method{"floatMethod", ::jni::Return{jfloat{}}, Params<>{}},
        Method{"doubleMethod", ::jni::Return{jdouble{}}, Params<>{}},
        Method{"objectMethod", ::jni::Return{Class{"kClass2"}}, Params<>{}},
        Method{"rank1ArrayMethod", ::jni::Return{Array{Class{"kClass2"}}}, Params<>{}},
        Method{"rank2ArrayMethod", ::jni::Return{Array{Array{Class{"kClass2"}}}}, Params<>{}},

        Method{"simpleFunc", ::jni::Return{int{}}, Params{jfloat{}}},
        Method{"complexFunc", ::jni::Return{float{}},
          Params{
            Array{Array{Class{"kClass2"}}}, int{}, float{}, Class{"kClass3"},
          }
        }
      },
};
// clang-format on

TEST_F(JniTest, StaticExerciseAllReturns) {
  EXPECT_CALL(*env_,
              GetStaticMethodID(_, StrEq("booleanMethod"), StrEq("()Z")));
  EXPECT_CALL(*env_, CallStaticBooleanMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("byteMethod"), StrEq("()B")));
  EXPECT_CALL(*env_, CallStaticByteMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("charMethod"), StrEq("()C")));
  EXPECT_CALL(*env_, CallStaticCharMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("shortMethod"), StrEq("()S")));
  EXPECT_CALL(*env_, CallStaticShortMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("intMethod"), StrEq("()I")));
  EXPECT_CALL(*env_, CallStaticIntMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("longMethod"), StrEq("()J")));
  EXPECT_CALL(*env_, CallStaticLongMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("floatMethod"), StrEq("()F")));
  EXPECT_CALL(*env_, CallStaticFloatMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("doubleMethod"), StrEq("()D")));
  EXPECT_CALL(*env_, CallStaticDoubleMethodV);

  EXPECT_CALL(
      *env_, GetStaticMethodID(_, StrEq("objectMethod"), StrEq("()LkClass2;")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("rank1ArrayMethod"),
                                       StrEq("()[LkClass2;")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("rank2ArrayMethod"),
                                       StrEq("()[[LkClass2;")));

  EXPECT_CALL(*env_, CallStaticObjectMethodV).Times(3);

  StaticRef<kMethodClass>{}("booleanMethod");
  StaticRef<kMethodClass>{}("byteMethod");
  StaticRef<kMethodClass>{}("charMethod");
  StaticRef<kMethodClass>{}("shortMethod");
  StaticRef<kMethodClass>{}("intMethod");
  StaticRef<kMethodClass>{}("longMethod");
  StaticRef<kMethodClass>{}("floatMethod");
  StaticRef<kMethodClass>{}("doubleMethod");

  // It would be more complete to exercise all types here.
  StaticRef<kMethodClass>{}("objectMethod");
  StaticRef<kMethodClass>{}("rank1ArrayMethod");
  StaticRef<kMethodClass>{}("rank2ArrayMethod");
}

TEST_F(JniTest, StaticExerciseComplexSetOfParams) {
  // The primary difference for statics are how they handle their returns.
  // Coverage is already fairly extensive for params.
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("simpleFunc"), StrEq("(F)I")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("complexFunc"),
                                       StrEq("([[LkClass2;IFLkClass3;)F")));

  StaticRef<kMethodClass>{}("simpleFunc", 123.f);
  StaticRef<kMethodClass>{}("complexFunc", jobjectArray{nullptr}, 123, 456.f,
                            jobject{nullptr});
}

}  // namespace
