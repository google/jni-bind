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
#include "implementation/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptGlobal;
using ::jni::AdoptLocal;
using ::jni::Class;
using ::jni::Constructor;
using ::jni::CreateCopy;
using ::jni::Field;
using ::jni::GlobalObject;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::PromoteToGlobal;
using ::jni::test::AsGlobal;
using ::jni::test::Fake;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrEq;

TEST_F(JniTest, GlobalObject_CallsNewAndDeleteOnNewObject) {
  static constexpr Class kClass{"kClass"};

  EXPECT_CALL(*env_, NewObjectV).WillOnce(Return(Fake<jobject>()));
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jobject>())));

  GlobalObject<kClass> global_object{};

  EXPECT_EQ(jobject{global_object}, AsGlobal(Fake<jobject>()));
}

TEST_F(JniTest, GlobalObject_ConstructsFromNonStandardConstructor) {
  static constexpr Class kClass{
      "kClass",
      Constructor{jfloat{}, jfloat{}},
  };

  EXPECT_CALL(*env_, NewObjectV).WillOnce(Return(Fake<jobject>()));
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jobject>())));

  GlobalObject<kClass> global_object{1.f, 2.f};

  EXPECT_NE(jobject{global_object}, nullptr);
}

TEST_F(JniTest, GlobalObject_DoesNotDeleteAnyLocalsForAdoptedGlobalJobject) {
  static constexpr Class kClass{"kClass"};

  EXPECT_CALL(*env_, NewObjectV).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef).Times(0);
  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jobject>()));

  GlobalObject<kClass> global_object{AdoptGlobal{}, Fake<jobject>()};

  EXPECT_EQ(jobject{global_object}, Fake<jobject>());
}

TEST_F(JniTest, GlobalObject_PromotesJobjectsOnConstruction) {
  EXPECT_CALL(*env_, NewObjectV).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef).Times(1);
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jobject>())));

  static constexpr Class kClass{"kClass"};
  GlobalObject<kClass> global_object{PromoteToGlobal{}, Fake<jobject>()};
  EXPECT_EQ(jobject{global_object}, AsGlobal(Fake<jobject>()));
}

TEST_F(JniTest, GlobalObject_PromotesDecoratedLocals) {
  EXPECT_CALL(*env_, NewObjectV).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef).Times(1);
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jobject>())));

  static constexpr Class kClass{"kClass"};
  LocalObject<kClass> local_obj{AdoptLocal{}, Fake<jobject>()};
  // GlobalObject<kClass> global_object{local_obj}; // doesn't compile (good).
  GlobalObject<kClass> global_object{std::move(local_obj)};

  EXPECT_EQ(jobject{global_object}, AsGlobal(Fake<jobject>()));
}

// Identical to above but Local constructed in place.
TEST_F(JniTest, GlobalObject_PromotesDecoratedLocalsFromXValue) {
  EXPECT_CALL(*env_, NewObjectV).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef).Times(1);
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jobject>())));

  static constexpr Class kClass{"kClass"};
  GlobalObject<kClass> global_object{
      LocalObject<kClass>{AdoptLocal{}, Fake<jobject>()}};

  EXPECT_EQ(jobject{global_object}, AsGlobal(Fake<jobject>()));
}

TEST_F(JniTest, GlobalObject_CallsOnlyDeleteOnWrapCtor) {
  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jobject>()));

  static constexpr Class kClass{"com/google/CallsOnlyDeleteOnWrapCtor"};
  GlobalObject<kClass> global_object{AdoptGlobal{}, Fake<jobject>()};

  EXPECT_NE(jobject{global_object}, nullptr);
}

TEST_F(JniTest, GlobalObject_CallsNewGlobalRefOnCopy) {
  static constexpr Class kClass{"kClass"};

  EXPECT_CALL(*env_, NewGlobalRef(Fake<jobject>(1)))
      .WillOnce(::testing::Return(Fake<jobject>(2)));
  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jobject>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>(1))).Times(0);

  GlobalObject<kClass> global_object{CreateCopy{}, Fake<jobject>(1)};
  EXPECT_EQ(jobject{global_object}, Fake<jobject>(2));
}

TEST_F(JniTest, GlobalObject_CallsDeleteOnceAfterAMoveConstruction) {
  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jobject>()));

  static constexpr Class kClass{
      "com/google/CallsDeleteOnceAfterAMoveConstruction"};
  GlobalObject<kClass> global_object_1{AdoptGlobal{}, Fake<jobject>()};
  EXPECT_EQ(jobject{global_object_1}, Fake<jobject>());
  GlobalObject<kClass> global_object_2{std::move(global_object_1)};

  EXPECT_EQ(jobject{global_object_1}, nullptr);  // NOLINT
  EXPECT_EQ(jobject{global_object_2}, Fake<jobject>());
}

TEST_F(JniTest, GlobalObject_FunctionsProperlyInSTLContainer) {
  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jobject>(1)));
  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jobject>(2)));

  static constexpr Class kClass{
      "com/google/CallsDeleteOnceAfterAMoveConstruction"};
  GlobalObject<kClass> global_object_1{AdoptGlobal{}, Fake<jobject>(1)};
  GlobalObject<kClass> global_object_2{AdoptGlobal{}, Fake<jobject>(2)};
  std::tuple t{std::move(global_object_1), std::move(global_object_2)};
}

TEST_F(JniTest, GlobalObject_ValuesWorkAfterMoveConstructor) {
  EXPECT_CALL(*env_, CallIntMethodV).Times(3);
  EXPECT_CALL(*env_, SetIntField).Times(4);

  static constexpr Class kClass{
      "com/google/ValuesWorkAfterMoveConstructor",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
      Field{"BarField", jint{}}};
  GlobalObject<kClass> global_object_1{AdoptGlobal{}, Fake<jobject>(1)};
  global_object_1("Foo", 1);
  global_object_1("Foo", 2);
  global_object_1["BarField"].Set(1);

  GlobalObject<kClass> global_object_2{std::move(global_object_1)};
  global_object_2("Foo", 3);
  global_object_2["BarField"].Set(2);
  global_object_2["BarField"].Set(3);
  global_object_2["BarField"].Set(4);

  GlobalObject<kClass> global_object_3{AdoptGlobal{}, Fake<jobject>(1)};
}

TEST_F(JniTest, GlobalObject_ObjectReturnsInstanceMethods) {
  InSequence seq;
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), StrEq("()V")))
      .WillOnce(Return(Fake<jmethodID>(1)));
  EXPECT_CALL(*env_, NewObjectV(_, Fake<jmethodID>(1), _))
      .WillOnce(Return(Fake<jobject>()));

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("(I)I")))
      .WillOnce(Return(Fake<jmethodID>(2)));
  EXPECT_CALL(*env_, CallIntMethodV(_, _, _));

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("(F)V")))
      .WillOnce(Return(Fake<jmethodID>(2)));
  EXPECT_CALL(*env_, CallVoidMethodV(_, _, _));

  EXPECT_CALL(*env_,
              GetMethodID(_,
                          StrEq("AMethodWithAReallyLongNameThatWouldPossiblyBeH"
                                "ardForTemplatesToHandle"),
                          StrEq("(IFIFD)D")));
  EXPECT_CALL(*env_, CallDoubleMethodV(_, _, _));

  static constexpr Class java_class_under_test{
      "com/google/ObjectReturnsInstanceMethods",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
      Method{"Baz", jni::Return<void>{}, Params<jfloat>{}},
      Method{"AMethodWithAReallyLongNameThatWouldPossiblyBeHardForTemplates"
             "ToHandle",
             jni::Return<jdouble>{},
             Params<jint, jfloat, jint, jfloat, jdouble>{}}};

  GlobalObject<java_class_under_test> global_object{};
  global_object("Foo", 1);
  global_object("Baz", 2.f);
  global_object(
      "AMethodWithAReallyLongNameThatWouldPossiblyBeHardForTemplatesToHandle",
      int{}, float{}, int{}, float{}, double{});
}

TEST_F(JniTest, GlobalObject_ReleasesGlobalsForAlternateConstructors) {
  static constexpr Class java_class_under_test{
      "ReleasesGlobalsForAlternateConstructors", jni::Constructor<int>{}};
  GlobalObject<java_class_under_test> g1{1};
  GlobalObject<java_class_under_test> g2{2};
  GlobalObject<java_class_under_test> g3{3};
  EXPECT_CALL(*env_, DeleteGlobalRef(_)).Times(3);
}

TEST_F(JniTest, GlobalObject_SupportsPassingAPrvalue) {
  static constexpr Class kTestClass1{"TestClass1"};
  static constexpr Class kTestClass2{
      "TestClass2", Method{"Foo", jni::Return{}, jni::Params{kTestClass1}}};

  GlobalObject<kTestClass1> a{};
  GlobalObject<kTestClass2> b{};
  b("Foo", std::move(a));
}

TEST_F(JniTest, GlobalObjects_PromoteRValuesFromEmittedLValues) {
  static constexpr Class kClass1{"TestClass1"};
  static constexpr Class kClass2{
      "TestClass2", Method{"Foo", jni::Return{kClass1}, jni::Params{}}};

  LocalObject<kClass2> b{};
  GlobalObject<kClass1> a{b("Foo")};

  a = b("Foo");
}

}  // namespace
