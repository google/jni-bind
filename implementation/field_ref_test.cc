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

#include <optional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::Field;
using ::jni::LocalObject;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

// clang-format off
static constexpr Class java_class_under_test{
    "com/google/TestClass",
    Field{"booleanField", jboolean{}},
    Field{"byteField", jbyte{}},
    Field{"charField", jchar{}},
    Field{"shortField", jshort{}},
    Field{"intField", jint{}},
    Field{"longField", jlong{}},
    Field{"floatField", jfloat{}},
    Field{"doubleField", jdouble{}}
};
// clang-format on

TEST_F(JniTest, Field_SimpleGetAndSet) {
  static constexpr Class java_class_under_test{"com/google/TestClass",
                                               Field("SomeField", jint{})};

  jfieldID field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("SomeField"), StrEq("I")))
      .WillOnce(Return(field_id));
  EXPECT_CALL(*env_, GetIntField(_, field_id)).WillOnce(Return(999));
  EXPECT_CALL(*env_, SetIntField(_, field_id, 123));

  jni::GlobalObject<java_class_under_test> obj{};
  EXPECT_EQ(999, obj["SomeField"].Get());
  obj["SomeField"].Set(123);
}

TEST_F(JniTest, Field_BooleanField) {
  jni::LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetBooleanField);
  EXPECT_CALL(*env_, SetBooleanField);
  obj["booleanField"].Get();
  obj["booleanField"].Set(true);
}

TEST_F(JniTest, Field_ByteField) {
  jni::LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetByteField);
  EXPECT_CALL(*env_, SetByteField);
  obj["byteField"].Get();
  obj["byteField"].Set(jbyte{123});
}

TEST_F(JniTest, Field_CharField) {
  jni::LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetCharField);
  EXPECT_CALL(*env_, SetCharField);
  obj["charField"].Get();
  obj["charField"].Set(jchar{'a'});
}

TEST_F(JniTest, Field_ShortField) {
  jni::LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetShortField);
  EXPECT_CALL(*env_, SetShortField);
  obj["shortField"].Get();
  obj["shortField"].Set(jshort{123});
}

TEST_F(JniTest, Field_intField) {
  jni::LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetIntField);
  EXPECT_CALL(*env_, SetIntField);
  obj["intField"].Get();
  obj["intField"].Set(123);
}

TEST_F(JniTest, Field_longField) {
  jni::LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetLongField);
  EXPECT_CALL(*env_, SetLongField);
  obj["longField"].Get();
  obj["longField"].Set(123);
}

TEST_F(JniTest, Field_floatField) {
  jni::LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetFloatField);
  EXPECT_CALL(*env_, SetFloatField);
  obj["floatField"].Get();
  obj["floatField"].Set(123.f);
}

TEST_F(JniTest, Field_doubleField) {
  jni::LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetDoubleField);
  EXPECT_CALL(*env_, SetDoubleField);
  obj["doubleField"].Get();
  obj["doubleField"].Set(123.l);
}

TEST_F(JniTest, Field_ObjectGet) {
  static constexpr Class kClass{"com/google/TestClass",
                                Field{"classField", Class{"kClass2"}}};
  static constexpr Class kClass2{"kClass2"};

  jfieldID field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  jobject fake_obj{reinterpret_cast<jobject>(0XCDCDCD)};
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("classField"), StrEq("LkClass2;")))
      .WillOnce(Return(field_id));
  EXPECT_CALL(*env_, GetObjectField(_, field_id)).WillOnce(Return(fake_obj));

  jni::LocalObject<kClass> obj{};
  jni::LocalObject<kClass2> obj2 = obj["classField"].Get();
}

TEST_F(JniTest, Field_ObjectSet) {
  static constexpr Class kClass2{"kClass2"};
  static constexpr Class kClass{
      "com/google/TestClass",
      // Field{"classField", Class{"kClass2"}}}; // also works
      Field{"classField", kClass2}};

  jfieldID field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  jobject fake_obj{reinterpret_cast<jobject>(0XCDCDCD)};
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("classField"), StrEq("LkClass2;")))
      .WillOnce(Return(field_id));
  EXPECT_CALL(*env_, SetObjectField(_, field_id, fake_obj)).Times(4);

  jni::LocalObject<kClass> obj{};
  LocalObject<kClass2> some_obj{fake_obj};
  obj["classField"].Set(fake_obj);
  obj["classField"].Set(LocalObject<kClass2>{fake_obj});
  obj["classField"].Set(some_obj);
  obj["classField"].Set(std::move(some_obj));
}

}  // namespace
