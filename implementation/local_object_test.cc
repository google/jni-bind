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
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using jni::Class;
using jni::Field;
using jni::kDefaultClassLoader;
using jni::kDefaultJvm;
using jni::LocalObject;
using jni::Method;
using jni::Params;
using jni::test::JniTest;
using testing::_;
using testing::InSequence;
using testing::StrEq;

TEST_F(JniTest, LocalObject_CallsNewAndDeleteOnNewObject) {
  static constexpr Class kClass{"com/google/CallsNewAndDeleteOnNewObject"};
  const jobject local_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  // Note, class refs are not released, so Times() != 2.
  EXPECT_CALL(*env_, NewObjectV).WillOnce(testing::Return(local_jobject));
  EXPECT_CALL(*env_, DeleteLocalRef(local_jobject)).Times(1);

  LocalObject<kClass> local_object{};
  EXPECT_EQ(jobject{local_object}, local_jobject);
}

TEST_F(JniTest, LocalObject_CallsOnlyDeleteOnWrapCtor) {
  static constexpr Class kClass{"com/google/CallsOnlyDeleteOnWrapCtor"};
  const jobject local_jobject_from_user{reinterpret_cast<jobject>(0XAAAAAA)};

  EXPECT_CALL(*env_, NewLocalRef).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef(local_jobject_from_user)).Times(1);

  LocalObject<kClass> local_object{local_jobject_from_user};
  EXPECT_NE(jobject{local_object}, nullptr);
}

TEST_F(JniTest, LocalObject_ObjectReturnsInstanceMethods) {
  // This test doesn't use the default JniTest helpers to be a little more
  // explicit about exactly what calls must be made in what order.
  static constexpr Class java_class_under_test{
      "com/google/AnotherTestClass",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
      Method{"Baz", jni::Return<void>{}, Params<jfloat>{}},
      Method{"AMethodWithAReallyLongNameThatWouldPossiblyBeHardForTemplates"
             "ToHandle",
             jni::Return<jdouble>{},
             Params<jint, jfloat, jint, jfloat, jdouble>{}}};

  const jobject local_jobject{reinterpret_cast<jobject>(0XAAAAAA)};
  const jmethodID ctor_method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  const jmethodID jmethod{reinterpret_cast<jmethodID>(0XCCCCCC)};

  InSequence seq;
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), StrEq("()V")))
      .WillOnce(testing::Return(ctor_method));
  EXPECT_CALL(*env_, NewObjectV(_, ctor_method, _))
      .WillOnce(testing::Return(local_jobject));

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("(I)I")))
      .WillOnce(testing::Return(jmethod));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("(F)V")))
      .WillOnce(testing::Return(jmethod));
  EXPECT_CALL(*env_,
              GetMethodID(_,
                          StrEq("AMethodWithAReallyLongNameThatWouldPossiblyBeH"
                                "ardForTemplatesToHandle"),
                          StrEq("(IFIFD)D")))
      .WillOnce(testing::Return(jmethod));
  EXPECT_CALL(*env_, DeleteLocalRef(local_jobject)).Times(1);

  LocalObject<java_class_under_test> dynamic_object{};
  dynamic_object("Foo", 12345);
  dynamic_object("Baz", 12345.f);
  dynamic_object(
      "AMethodWithAReallyLongNameThatWouldPossiblyBeHardForTemplatesToHandle",
      12345, 12345.f, 12345, 12345.f, jdouble{12345});
}

TEST_F(JniTest, LocalObject_CallsDeleteOnceAfterAMoveConstruction) {
  static constexpr Class kClass{
      "com/google/CallsDeleteOnceAfterAMoveConstruction"};
  const jobject local_jobject_from_user{reinterpret_cast<jobject>(0XAAAAAA)};

  EXPECT_CALL(*env_, NewLocalRef).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef(local_jobject_from_user)).Times(1);

  LocalObject<kClass, kDefaultClassLoader, kDefaultJvm> local_object_1{
      local_jobject_from_user};

  EXPECT_NE(jobject{local_object_1}, nullptr);

  LocalObject<kClass, kDefaultClassLoader, kDefaultJvm> local_object_2{
      std::move(local_object_1)};

  EXPECT_NE(jobject{local_object_2}, nullptr);
}

TEST_F(JniTest, LocalObject_FunctionsProperlyInSTLContainer) {
  static constexpr Class kClass{
      "com/google/CallsDeleteOnceAfterAMoveConstruction"};
  const jobject local_jobject_from_user{reinterpret_cast<jobject>(0XAAAAAA)};

  EXPECT_CALL(*env_, NewLocalRef).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef(local_jobject_from_user)).Times(2);

  LocalObject<kClass, kDefaultClassLoader, kDefaultJvm> local_object_1{
      local_jobject_from_user};
  LocalObject<kClass, kDefaultClassLoader, kDefaultJvm> local_object_2{
      local_jobject_from_user};
  std::tuple t{std::move(local_object_1), std::move(local_object_2)};
}

TEST_F(JniTest, LocalObject_ValuesWorkAfterMoveConstructor) {
  static constexpr Class kClass{
      "com/google/ValuesWorkAfterMoveConstructor",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
      Field{"BarField", jint{}}};
  const jobject local_jobject_from_user{reinterpret_cast<jobject>(0XAAAAAA)};

  EXPECT_CALL(*env_, CallIntMethodV).Times(3);
  EXPECT_CALL(*env_, SetIntField).Times(4);

  LocalObject<kClass, kDefaultClassLoader, kDefaultJvm> local_object_1{
      local_jobject_from_user};
  local_object_1("Foo", 1);
  local_object_1("Foo", 2);
  local_object_1["BarField"].Set(1);

  LocalObject<kClass, kDefaultClassLoader, kDefaultJvm> local_object_2{
      std::move(local_object_1)};
  local_object_2("Foo", 3);
  local_object_2["BarField"].Set(2);
  local_object_2["BarField"].Set(3);
  local_object_2["BarField"].Set(4);
}

TEST_F(JniTest, LocalObject_ReleasesLocalsForAlternateConstructors) {
  static constexpr Class java_class_under_test{
      "ReleasesLocalsForAlternateConstructors", jni::Constructor<int>{}};
  LocalObject<java_class_under_test> g1{1};
  LocalObject<java_class_under_test> g2{2};
  LocalObject<java_class_under_test> g3{3};
  EXPECT_CALL(*env_, DeleteLocalRef(_)).Times(3);
}

TEST_F(JniTest, LocalObject_SupportsPassingAnObjectAsAnLvalue) {
  static constexpr Class kTestClass1{"TestClass1"};

  static constexpr Class kTestClass2{
      "TestClass2", Method{"Foo", jni::Return{}, jni::Params{kTestClass1}}};

  LocalObject<kTestClass1> a{};
  LocalObject<kTestClass2> b{};
  b("Foo", a);
}

TEST_F(JniTest, LocalObject_SupportsReturningAClass) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kTestClass1{
      "TestClass1", Method{"Foo", jni::Return{kClass2}, jni::Params{}}};

  LocalObject<kTestClass1> a{};
  a("Foo");
}

TEST_F(JniTest, LocalObject_SupportsReturningAString) {
  static constexpr Class kTestClass1{
      "TestClass1", Method{"Foo", jni::Return<jstring>{}, jni::Params{}}};

  LocalObject<kTestClass1> a{};
  a("Foo");
}

jobject ReturnOutputOfMethod() {
  static constexpr Class kClass1{"Class1"};
  static constexpr Class kClass2{
      "Class2", Method{"Foo", jni::Return{kClass1}, Params<>{}}};

  return LocalObject<kClass2>{}("Foo").Release();
}

TEST_F(JniTest, LocalObject_CompilesWhenReturnReleasing) {
  ReturnOutputOfMethod();
}

TEST_F(JniTest, LocalObject_SupportsPassingAnObjectAsAnPrvalue) {
  static constexpr Class kTestClass1{"TestClass1"};

  static constexpr Class kTestClass2{
      "TestClass2", Method{"Foo", jni::Return{}, jni::Params{kTestClass1}}};

  LocalObject<kTestClass1> a{};
  LocalObject<kTestClass2> b{};
  b("Foo", std::move(a));
}

TEST_F(JniTest, LocalObject_SupportsPassingAnObjectAsAnXvalue) {
  static constexpr Class kTestClass1{"TestClass1"};

  static constexpr Class kTestClass2{
      "TestClass2", Method{"Foo", jni::Return{}, jni::Params{kTestClass1}}};

  LocalObject<kTestClass2> b{};
  b("Foo", LocalObject<kTestClass1>{});
}

}  // namespace
