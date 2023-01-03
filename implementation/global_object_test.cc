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

using ::jni::AdoptGlobal;
using ::jni::Class;
using ::jni::Constructor;
using ::jni::Field;
using ::jni::GlobalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::PromoteToGlobal;
using ::jni::test::AsGlobal;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrEq;

TEST_F(JniTest, GlobalObject_CallsNewAndDeleteOnNewObject) {
  static constexpr Class kClass{"kClass"};
  const jobject local_jobject{reinterpret_cast<jobject>(0XBBBBBB)};

  EXPECT_CALL(*env_, NewObjectV).WillOnce(Return(local_jobject));
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(local_jobject)));

  GlobalObject<kClass> global_object{};

  EXPECT_NE(jobject{global_object}, nullptr);
}

TEST_F(JniTest, GlobalObject_ConstructsFromNonStandardConstructor) {
  static constexpr Class kClass{
      "kClass",
      Constructor{jfloat{}, jfloat{}},
  };
  const jobject local_jobject{reinterpret_cast<jobject>(0XBBBBBB)};

  EXPECT_CALL(*env_, NewObjectV).WillOnce(Return(local_jobject));
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(local_jobject)));

  GlobalObject<kClass> global_object{1.f, 2.f};

  EXPECT_NE(jobject{global_object}, nullptr);
}

TEST_F(JniTest, GlobalObject_DoesNotDeleteAnyLocalsForAdoptedGlobalJobject) {
  static constexpr Class kClass{"kClass"};
  const jobject global_jobject{reinterpret_cast<jobject>(0XBBBBBB)};

  EXPECT_CALL(*env_, NewObjectV).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef).Times(0);
  EXPECT_CALL(*env_, DeleteGlobalRef(global_jobject));

  GlobalObject<kClass> global_object{AdoptGlobal{}, global_jobject};

  EXPECT_NE(jobject{global_object}, nullptr);
}

TEST_F(JniTest, GlobalObject_PromotesJobjectsOnConstruction) {
  static constexpr Class kClass{"kClass"};
  const jobject local_jobject{reinterpret_cast<jobject>(0XBBBBBB)};

  EXPECT_CALL(*env_, NewObjectV).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef).Times(1);
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(local_jobject)));

  GlobalObject<kClass> global_object{PromoteToGlobal{}, local_jobject};

  EXPECT_NE(jobject{global_object}, nullptr);
}

TEST_F(JniTest, GlobalObject_CallsOnlyDeleteOnWrapCtor) {
  static constexpr Class kClass{"com/google/CallsOnlyDeleteOnWrapCtor"};
  const jobject global_jobject_from_user{reinterpret_cast<jobject>(0XAAAAAA)};

  EXPECT_CALL(*env_, DeleteGlobalRef(global_jobject_from_user));

  GlobalObject<kClass> global_object{AdoptGlobal{}, global_jobject_from_user};

  EXPECT_NE(jobject{global_object}, nullptr);
}

TEST_F(JniTest, GlobalObject_CallsDeleteOnceAfterAMoveConstruction) {
  static constexpr Class kClass{
      "com/google/CallsDeleteOnceAfterAMoveConstruction"};
  const jobject global_jobject_from_user{reinterpret_cast<jobject>(0XAAAAAA)};

  EXPECT_CALL(*env_, DeleteGlobalRef(global_jobject_from_user));

  GlobalObject<kClass> global_object_1{AdoptGlobal{}, global_jobject_from_user};
  EXPECT_NE(jobject{global_object_1}, nullptr);
  GlobalObject<kClass> global_object_2{std::move(global_object_1)};
  EXPECT_NE(jobject{global_object_2}, nullptr);
}

TEST_F(JniTest, GlobalObject_FunctionsProperlyInSTLContainer) {
  static constexpr Class kClass{
      "com/google/CallsDeleteOnceAfterAMoveConstruction"};
  const jobject global_jobject_from_user1{reinterpret_cast<jobject>(0XAAAAAA)};
  const jobject global_jobject_from_user2{reinterpret_cast<jobject>(0XBBBBBB)};

  EXPECT_CALL(*env_, DeleteGlobalRef(global_jobject_from_user1));
  EXPECT_CALL(*env_, DeleteGlobalRef(global_jobject_from_user2));
  GlobalObject<kClass> global_object_1{AdoptGlobal{},
                                       global_jobject_from_user1};
  GlobalObject<kClass> global_object_2{AdoptGlobal{},
                                       global_jobject_from_user2};
  std::tuple t{std::move(global_object_1), std::move(global_object_2)};
}

TEST_F(JniTest, GlobalObject_ValuesWorkAfterMoveConstructor) {
  static constexpr Class kClass{
      "com/google/ValuesWorkAfterMoveConstructor",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
      Field{"BarField", jint{}}};
  const jobject global_jobject_from_user{reinterpret_cast<jobject>(0XAAAAAA)};

  EXPECT_CALL(*env_, CallIntMethodV).Times(3);
  EXPECT_CALL(*env_, SetIntField).Times(4);

  GlobalObject<kClass> global_object_1{AdoptGlobal{}, global_jobject_from_user};
  global_object_1("Foo", 1);
  global_object_1("Foo", 2);
  global_object_1["BarField"].Set(1);

  GlobalObject<kClass> global_object_2{std::move(global_object_1)};
  global_object_2("Foo", 3);
  global_object_2["BarField"].Set(2);
  global_object_2["BarField"].Set(3);
  global_object_2["BarField"].Set(4);

  GlobalObject<kClass> global_object_3{AdoptGlobal{}, global_jobject_from_user};
}

TEST_F(JniTest, GlobalObject_ObjectReturnsInstanceMethods) {
  static constexpr Class java_class_under_test{
      "com/google/ObjectReturnsInstanceMethods",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
      Method{"Baz", jni::Return<void>{}, Params<jfloat>{}},
      Method{"AMethodWithAReallyLongNameThatWouldPossiblyBeHardForTemplates"
             "ToHandle",
             jni::Return<jdouble>{},
             Params<jint, jfloat, jint, jfloat, jdouble>{}}};

  const jmethodID ctor_method{reinterpret_cast<jmethodID>(0XCCCCCC)};
  const jmethodID jmethod{reinterpret_cast<jmethodID>(0XDDDDDD)};
  const jobject local_jobject{reinterpret_cast<jobject>(0XEEEEEE)};

  InSequence seq;
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), StrEq("()V")))
      .WillOnce(Return(ctor_method));
  EXPECT_CALL(*env_, NewObjectV(_, ctor_method, _))
      .WillOnce(Return(local_jobject));

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("(I)I")))
      .WillOnce(Return(jmethod));
  EXPECT_CALL(*env_, CallIntMethodV(_, _, _));

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("(F)V")))
      .WillOnce(Return(jmethod));
  EXPECT_CALL(*env_, CallVoidMethodV(_, _, _));

  EXPECT_CALL(*env_,
              GetMethodID(_,
                          StrEq("AMethodWithAReallyLongNameThatWouldPossiblyBeH"
                                "ardForTemplatesToHandle"),
                          StrEq("(IFIFD)D")));
  EXPECT_CALL(*env_, CallDoubleMethodV(_, _, _));

  auto dynamic_object = GlobalObject<java_class_under_test>();
  dynamic_object("Foo", 1);
  dynamic_object("Baz", 2.f);
  dynamic_object(
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

}  // namespace
