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

#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptLocal;
using ::jni::Class;
using ::jni::Fake;
using ::jni::Field;
using ::jni::GlobalObject;
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
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("SomeField"), StrEq("I")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetIntField(_, Fake<jfieldID>())).WillOnce(Return(999));
  EXPECT_CALL(*env_, SetIntField(_, Fake<jfieldID>(), 123));

  static constexpr Class java_class_under_test{"com/google/TestClass",
                                               Field("SomeField", jint{})};

  GlobalObject<java_class_under_test> obj{};
  EXPECT_EQ(999, obj.Access<"SomeField">().Get());
  obj.Access<"SomeField">().Set(123);
}

TEST_F(JniTest, Field_BooleanField) {
  EXPECT_CALL(*env_, GetBooleanField);
  EXPECT_CALL(*env_, SetBooleanField);

  LocalObject<java_class_under_test> obj{};
  obj.Access<"booleanField">().Get();
  obj.Access<"booleanField">().Set(true);
}

TEST_F(JniTest, Field_ByteField) {
  EXPECT_CALL(*env_, GetByteField);
  EXPECT_CALL(*env_, SetByteField);

  LocalObject<java_class_under_test> obj{};
  obj.Access<"byteField">().Get();
  obj.Access<"byteField">().Set(jbyte{123});
}

TEST_F(JniTest, Field_CharField) {
  EXPECT_CALL(*env_, GetCharField);
  EXPECT_CALL(*env_, SetCharField);

  LocalObject<java_class_under_test> obj{};
  obj.Access<"charField">().Get();
  obj.Access<"charField">().Set(jchar{'a'});
}

TEST_F(JniTest, Field_ShortField) {
  EXPECT_CALL(*env_, GetShortField);
  EXPECT_CALL(*env_, SetShortField);

  LocalObject<java_class_under_test> obj{};
  obj.Access<"shortField">().Get();
  obj.Access<"shortField">().Set(jshort{123});
}

TEST_F(JniTest, Field_intField) {
  EXPECT_CALL(*env_, GetIntField);
  EXPECT_CALL(*env_, SetIntField);

  LocalObject<java_class_under_test> obj{};
  obj.Access<"intField">().Get();
  obj.Access<"intField">().Set(123);
}

TEST_F(JniTest, Field_longField) {
  EXPECT_CALL(*env_, GetLongField);
  EXPECT_CALL(*env_, SetLongField);

  LocalObject<java_class_under_test> obj{};
  obj.Access<"longField">().Get();
  obj.Access<"longField">().Set(123);
}

TEST_F(JniTest, Field_floatField) {
  LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetFloatField);
  EXPECT_CALL(*env_, SetFloatField);
  obj.Access<"floatField">().Get();
  obj.Access<"floatField">().Set(123.f);
}

TEST_F(JniTest, Field_doubleField) {
  LocalObject<java_class_under_test> obj{};
  EXPECT_CALL(*env_, GetDoubleField);
  EXPECT_CALL(*env_, SetDoubleField);
  obj.Access<"doubleField">().Get();
  obj.Access<"doubleField">().Set(123.);
}

TEST_F(JniTest, Field_ObjectGet) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("classField"), StrEq("LkClass2;")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(_, Fake<jfieldID>()))
      .WillOnce(Return(Fake<jobject>()));

  static constexpr Class kClass{"com/google/TestClass",
                                Field{"classField", Class{"kClass2"}}};
  static constexpr Class kClass2{"kClass2"};

  LocalObject<kClass> obj{};
  LocalObject<kClass2> obj2 = obj.Access<"classField">().Get();
}

TEST_F(JniTest, Field_ObjectSet) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("classField"), StrEq("LkClass2;")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, SetObjectField(_, Fake<jfieldID>(), Fake<jobject>()))
      .Times(4);

  static constexpr Class kClass2{"kClass2"};
  static constexpr Class kClass{
      "com/google/TestClass",
      // Field{"classField", Class{"kClass2"}}}; // also works
      Field{"classField", kClass2}};

  LocalObject<kClass> obj{};
  LocalObject<kClass2> some_obj{AdoptLocal{}, Fake<jobject>()};
  obj.Access<"classField">().Set(Fake<jobject>());
  obj.Access<"classField">().Set(
      LocalObject<kClass2>{AdoptLocal{}, Fake<jobject>()});
  obj.Access<"classField">().Set(some_obj);
  obj.Access<"classField">().Set(std::move(some_obj));
}

}  // namespace
