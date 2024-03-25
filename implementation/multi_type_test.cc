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
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::Fake;
using ::jni::Field;
using ::jni::Method;
using ::jni::Params;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::StrEq;

TEST_F(JniTest, MultiTypeTest_SimpleSmokeTestForSingleObject) {
  static constexpr Class object{
      "ARCore",
      Method{"Foo", jni::Return<jint>{}, Params<jint, jfloat>{}},
      Method{"Bar", jni::Return{jint{}}},
      Method{"Baz", jni::Return<void>{}, Params<jfloat>{}},
      Field{"SomeField", jint{}},
  };

  EXPECT_CALL(*env_, FindClass(StrEq("ARCore")))
      .WillOnce(Return(Fake<jclass>(1)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>(1)));
  EXPECT_CALL(*env_, NewGlobalRef(Eq(Fake<jclass>(1))))
      .WillOnce(Return(Fake<jclass>(2)));
  EXPECT_CALL(*env_,
              GetMethodID(Fake<jclass>(2), StrEq("<init>"), StrEq("()V")))
      .WillOnce(Return(Fake<jmethodID>()));

  EXPECT_CALL(*env_, NewObjectV(Fake<jclass>(2), Fake<jmethodID>(), _))
      .WillOnce(Return(Fake<jobject>(1)));
  EXPECT_CALL(*env_, NewGlobalRef(Fake<jobject>(1)))
      .WillOnce(Return(Fake<jobject>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>(1))).Times(1);

  EXPECT_CALL(*env_, GetMethodID(Fake<jclass>(2), StrEq("Foo"), StrEq("(IF)I")))
      .WillOnce(Return(Fake<jmethodID>()));
  EXPECT_CALL(*env_, GetMethodID(Fake<jclass>(2), StrEq("Bar"), StrEq("()I")))
      .WillOnce(Return(Fake<jmethodID>()));
  EXPECT_CALL(*env_, GetMethodID(Fake<jclass>(2), StrEq("Baz"), StrEq("(F)V")))
      .WillOnce(Return(Fake<jmethodID>()));
  EXPECT_CALL(*env_,
              GetFieldID(Fake<jclass>(2), StrEq("SomeField"), StrEq("I")))
      .WillOnce(Return(Fake<jfieldID>()));

  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jobject>(2))).Times(1);
  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jclass>(2))).Times(1);

  jni::GlobalObject<object> obj{};
  obj("Foo", 1, 2.f);
  obj("Baz", 1.f);
  obj("Baz", 1.f);
  obj("Baz", 2.f);
  obj("Baz", 3.f);
  obj("Bar");
  obj["SomeField"].Get();
}

TEST_F(JniTest, MultiTypeTest_MethodsOfSameNameButDifferentClassAreUnique) {
  EXPECT_CALL(*env_, GetMethodID).Times(2);

  static constexpr Class c1{
      "com/google/ARCore",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
  };
  static constexpr Class c2{
      "com/google/VRCore",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
  };

  jni::LocalObject<c1> obj1{Fake<jobject>(1)};
  jni::LocalObject<c2> obj2{Fake<jobject>(2)};
  obj1("Foo", 12345);
  obj2("Foo", 12345);

  // All of these calls ought not query for a method ID again.
  obj1("Foo", 12345);
  obj1("Foo", 12345);
  obj1("Foo", 12345);
  obj1("Foo", 12345);
  obj2("Foo", 12345);
  obj2("Foo", 12345);
  obj2("Foo", 12345);

  jni::LocalObject<c1> obj3{Fake<jobject>(3)};
  obj3("Foo", 12345);
  obj3("Foo", 12345);
}

}  // namespace
