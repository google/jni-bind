// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <optional>

#include "jni_test.h"
#include "mock_jni_env.h"
#include "mock_jvm.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_dep.h"
#include "jni_test.h"

namespace {

using jni::Class;
using jni::Field;
using jni::Method;
using jni::Params;
using jni::test::JniTest;
using testing::_;
using testing::Eq;
using testing::Return;
using testing::StrEq;

TEST_F(JniTest, MultiTypeTest_SimpleSmokeTestForSingleObject) {
  static constexpr Class object{
      "ARCore",
      Method{"Foo", jni::Return<jint>{}, Params<jint, jfloat>{}},
      Method{"Bar", jni::Return{jint{}}, Params<>{}},
      Method{"Baz", jni::Return<void>{}, Params<jfloat>{}},
      Field{"SomeField", jint{}},
  };

  jclass local_jclass{reinterpret_cast<jclass>(0XAAAAA)};
  jclass global_jclass{reinterpret_cast<jclass>(0XBBBBB)};
  jobject local_jobject{reinterpret_cast<jobject>(0XCCCCCC)};
  jobject global_jobject{reinterpret_cast<jobject>(0XDDDDDD)};
  jmethodID jmethod{reinterpret_cast<jmethodID>(0XEEEEEE)};
  jfieldID field_id{reinterpret_cast<jfieldID>(0XFFFFFF)};

  EXPECT_CALL(*env_, FindClass(StrEq("ARCore"))).WillOnce(Return(local_jclass));
  EXPECT_CALL(*env_, NewGlobalRef(Eq(local_jclass)))
      .WillOnce(Return(global_jclass));
  EXPECT_CALL(*env_, GetMethodID(global_jclass, StrEq("<init>"), StrEq("()V")))
      .WillOnce(Return(jmethod));

  EXPECT_CALL(*env_, NewObjectV(global_jclass, jmethod, _))
      .WillOnce(Return(local_jobject));
  EXPECT_CALL(*env_, NewGlobalRef(local_jobject))
      .WillOnce(Return(global_jobject));
  EXPECT_CALL(*env_, DeleteLocalRef(local_jobject)).Times(1);

  EXPECT_CALL(*env_, GetMethodID(global_jclass, StrEq("Foo"), StrEq("(IF)I")))
      .WillOnce(Return(jmethod));
  EXPECT_CALL(*env_, GetMethodID(global_jclass, StrEq("Bar"), StrEq("()I")))
      .WillOnce(Return(jmethod));
  EXPECT_CALL(*env_, GetMethodID(global_jclass, StrEq("Baz"), StrEq("(F)V")))
      .WillOnce(Return(jmethod));
  EXPECT_CALL(*env_, GetFieldID(global_jclass, StrEq("SomeField"), StrEq("I")))
      .WillOnce(Return(field_id));

  EXPECT_CALL(*env_, DeleteGlobalRef(global_jobject)).Times(1);
  EXPECT_CALL(*env_, DeleteGlobalRef(global_jclass)).Times(1);

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
  static constexpr Class c1{
      "com.google.ARCore",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
  };
  static constexpr Class c2{
      "com.google.VRCore",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
  };
  const jobject object1{reinterpret_cast<jobject>(0XAAAAAA)};
  const jobject object2{reinterpret_cast<jobject>(0XBBBBBB)};
  const jobject object3{reinterpret_cast<jobject>(0XCCCCCC)};

  EXPECT_CALL(*env_, GetMethodID).Times(2);

  jni::LocalObject<c1> obj1{object1};
  jni::LocalObject<c2> obj2{object2};
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

  jni::LocalObject<c1> obj3{object3};
  obj3("Foo", 12345);
  obj3("Foo", 12345);
}

}  // namespace
