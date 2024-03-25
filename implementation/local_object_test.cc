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

#include <array>
#include <optional>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::Fake;
using ::jni::Field;
using ::jni::kDefaultClassLoader;
using ::jni::kDefaultJvm;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::NewRef;
using ::jni::Params;
using ::jni::test::AsNewLocalReference;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::InSequence;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};
static constexpr Class kClass2{"kClass2"};

TEST_F(JniTest, LocalObject_AllowsNullPtrT) {
  EXPECT_CALL(*env_, NewLocalRef).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef).Times(0);

  LocalObject<kClass> obj{nullptr};
  EXPECT_EQ(jobject{obj}, nullptr);
}

TEST_F(JniTest, LocalObject_DoesntTryToDeleteNull) {
  EXPECT_CALL(*env_, NewLocalRef).Times(0);
  EXPECT_CALL(*env_, DeleteLocalRef).Times(0);

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_EQ(jobject{obj}, nullptr);
}

TEST_F(JniTest, LocalObject_CallsNewAndDeleteOnNewObject) {
  EXPECT_CALL(*env_, NewObjectV).WillOnce(testing::Return(Fake<jobject>()));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>())).Times(1);
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>())).Times(1);

  LocalObject<kClass> obj{};
  EXPECT_EQ(jobject{obj}, Fake<jobject>());
}

TEST_F(JniTest, LocalObject_CallsOnlyDeleteOnWrapCtor) {
  EXPECT_CALL(*env_, NewLocalRef).Times(1);
  EXPECT_CALL(*env_, DeleteLocalRef(AsNewLocalReference(Fake<jobject>())))
      .Times(1);

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_NE(jobject{obj}, nullptr);
}

TEST_F(JniTest, LocalObject_CallsNewLocalRefByDefault) {
  EXPECT_CALL(*env_, NewLocalRef).WillOnce(::testing::Return(Fake<jobject>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>(2)));

  LocalObject<kClass> obj{Fake<jobject>(1)};
  EXPECT_EQ(jobject{obj}, Fake<jobject>(2));
}

TEST_F(JniTest, LocalObject_CallsNewLocalRefOnCopy) {
  EXPECT_CALL(*env_, NewLocalRef).WillOnce(::testing::Return(Fake<jobject>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>(2)));

  LocalObject<kClass> obj{NewRef{}, Fake<jobject>(1)};
  EXPECT_EQ(jobject{obj}, Fake<jobject>(2));
}

TEST_F(JniTest, LocalObject_ObjectReturnsInstanceMethods) {
  // This test doesn't use the default JniTest helpers to be a little more
  // explicit about exactly what calls must be made in what order.
  static constexpr Class kClass{
      "com/google/AnotherClass",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
      Method{"Baz", jni::Return<void>{}, Params<jfloat>{}},
      Method{"AMethodWithAReallyLongNameThatWouldPossiblyBeHardForTemplates"
             "ToHandle",
             jni::Return<jdouble>{},
             Params<jint, jfloat, jint, jfloat, jdouble>{}}};

  InSequence seq;
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>())).Times(1);
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), StrEq("()V")))
      .WillOnce(testing::Return(Fake<jmethodID>(1)));
  EXPECT_CALL(*env_, NewObjectV(_, Fake<jmethodID>(1), _))
      .WillOnce(testing::Return(Fake<jobject>()));

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("(I)I")))
      .WillOnce(testing::Return(Fake<jmethodID>(2)));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("(F)V")))
      .WillOnce(testing::Return(Fake<jmethodID>(2)));
  EXPECT_CALL(*env_,
              GetMethodID(_,
                          StrEq("AMethodWithAReallyLongNameThatWouldPossiblyBeH"
                                "ardForTemplatesToHandle"),
                          StrEq("(IFIFD)D")))
      .WillOnce(testing::Return(Fake<jmethodID>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>())).Times(1);

  LocalObject<kClass> obj{};
  obj("Foo", 12345);
  obj("Baz", 12345.f);
  obj("AMethodWithAReallyLongNameThatWouldPossiblyBeHardForTemplatesToHandle",
      12345, 12345.f, 12345, 12345.f, jdouble{12345});
}

TEST_F(JniTest, LocalObject_CallsDeleteOnceAfterAMoveConstruction) {
  EXPECT_CALL(*env_, NewLocalRef).Times(1);
  EXPECT_CALL(*env_, DeleteLocalRef(AsNewLocalReference(Fake<jobject>())))
      .Times(1);

  LocalObject<kClass> obj_1{Fake<jobject>()};

  EXPECT_NE(jobject{obj_1}, nullptr);

  LocalObject<kClass> obj_2{std::move(obj_1)};

  EXPECT_NE(jobject{obj_2}, nullptr);
}

TEST_F(JniTest, LocalObject_FunctionsProperlyInSTLContainer) {
  EXPECT_CALL(*env_, NewLocalRef).Times(2);
  EXPECT_CALL(*env_, DeleteLocalRef(AsNewLocalReference(Fake<jobject>(1))));
  EXPECT_CALL(*env_, DeleteLocalRef(AsNewLocalReference(Fake<jobject>(2))));

  LocalObject<kClass> obj_1{Fake<jobject>(1)};
  LocalObject<kClass> obj_2{Fake<jobject>(2)};
  std::tuple t{std::move(obj_1), std::move(obj_2)};
}

TEST_F(JniTest, LocalObject_ValuesWorkAfterMoveConstructor) {
  static constexpr Class kClass{
      "com/google/ValuesWorkAfterMoveConstructor",
      Method{"Foo", jni::Return<jint>{}, Params<jint>{}},
      Field{"BarField", jint{}}};

  EXPECT_CALL(*env_, CallIntMethodV).Times(3);
  EXPECT_CALL(*env_, SetIntField).Times(4);

  LocalObject<kClass> obj_1{Fake<jobject>()};
  obj_1("Foo", 1);
  obj_1("Foo", 2);
  obj_1["BarField"].Set(1);

  LocalObject<kClass> obj_2{std::move(obj_1)};
  obj_2("Foo", 3);
  obj_2["BarField"].Set(2);
  obj_2["BarField"].Set(3);
  obj_2["BarField"].Set(4);
}

TEST_F(JniTest, LocalObject_ReleasesLocalsForAlternateConstructors) {
  static constexpr Class kClass{"ReleasesLocalsForAlternateConstructors",
                                jni::Constructor<int>{}};
  LocalObject<kClass> g1{1};
  LocalObject<kClass> g2{2};
  LocalObject<kClass> g3{3};
  EXPECT_CALL(*env_, DeleteLocalRef(_)).Times(3);
}

TEST_F(JniTest, LocalObject_ComparesAgainstOtherLocalObjects) {
  LocalObject<kClass> val_1{Fake<jobject>(1)};
  LocalObject<kClass2> val_2{Fake<jobject>(2)};

  EXPECT_TRUE(val_1 == val_1);
  EXPECT_FALSE(val_1 == val_2);
  EXPECT_TRUE(val_1 != val_2);
  EXPECT_TRUE(val_2 == val_2);
  EXPECT_TRUE(val_2 != val_1);
  EXPECT_FALSE(val_1 == val_2);
}

TEST_F(JniTest, LocalObject_ComparesAgainstjobjects) {
  static constexpr Class kClass{"kClass1"};
  LocalObject<kClass> val_1{Fake<jobject>()};

  EXPECT_TRUE(val_1 == AsNewLocalReference(Fake<jobject>()));
  EXPECT_TRUE(AsNewLocalReference(Fake<jobject>()) == val_1);

  EXPECT_FALSE(val_1 != AsNewLocalReference(Fake<jobject>()));
  EXPECT_FALSE(AsNewLocalReference(Fake<jobject>()) != val_1);
}

TEST_F(JniTest, LocalObject_ComparesAgainstOtherLocalObjects_InContainers) {
  struct A {
    LocalObject<kClass> val_1;
    LocalObject<kClass2> val_2;

    A(LocalObject<kClass>&& val_1, LocalObject<kClass2>&& val_2)
        : val_1(std::move(val_1)), val_2(std::move(val_2)) {}

    A(A&& rhs) : val_1(std::move(rhs.val_1)), val_2(std::move(rhs.val_2)) {}

    bool operator==(const A& rhs) const {
      return val_1 == rhs.val_1 && val_2 == rhs.val_2;
    }

    bool operator!=(const A& rhs) const {
      return val_1 != rhs.val_1 || val_2 != rhs.val_2;
    }
  };

  A val_1{LocalObject<kClass>{Fake<jobject>(1)}, {Fake<jobject>(2)}};
  A val_2{LocalObject<kClass2>{Fake<jobject>(1)}, {Fake<jobject>(3)}};

  EXPECT_FALSE(val_1 == val_2);
  EXPECT_TRUE(val_1 != val_2);

  EXPECT_EQ((std::array{A{LocalObject<kClass>(Fake<jobject>(1)),
                          LocalObject<kClass2>(Fake<jobject>(2))}}),
            (std::array{A{LocalObject<kClass>(Fake<jobject>(1)),
                          LocalObject<kClass2>(Fake<jobject>(2))}}));

  EXPECT_TRUE((std::array{A{LocalObject<kClass>(Fake<jobject>(1)),
                            LocalObject<kClass2>(Fake<jobject>(2))}} !=
               (std::array{A{LocalObject<kClass>(Fake<jobject>(1)),
                             LocalObject<kClass2>(Fake<jobject>(3))}})));
}

TEST_F(JniTest, LocalObject_SupportsPassingAnObjectAsAnLvalue) {
  static constexpr Class kClass2{
      "Class2", Method{"Foo", jni::Return{}, jni::Params{kClass}}};

  LocalObject<kClass> a{};
  LocalObject<kClass2> b{};
  b("Foo", a);
}

TEST_F(JniTest, LocalObject_SupportsReturningAClass) {
  static constexpr Class kClass{
      "Class1", Method{"Foo", jni::Return{kClass2}, jni::Params{}}};

  LocalObject<kClass> a{};
  a("Foo");
}

TEST_F(JniTest, LocalObject_SupportsReturningAString) {
  static constexpr Class kClass{
      "Class1", Method{"Foo", jni::Return<jstring>{}, jni::Params{}}};

  LocalObject<kClass> a{};
  a("Foo");
}

jobject ReturnOutputOfMethod() {
  static constexpr Class kClass2{"Class2", Method{"Foo", jni::Return{kClass}}};

  return LocalObject<kClass2>{}("Foo").Release();
}

TEST_F(JniTest, LocalObject_CompilesWhenReturnReleasing) {
  ReturnOutputOfMethod();
}

TEST_F(JniTest, LocalObject_SupportsPassingAnObjectAsAnPrvalue) {
  static constexpr Class kClass2{
      "Class2", Method{"Foo", jni::Return{}, jni::Params{kClass}}};

  LocalObject<kClass> a{};
  LocalObject<kClass2> b{};
  b("Foo", std::move(a));
}

TEST_F(JniTest, LocalObject_SupportsPassingAnObjectAsAnXvalue) {
  static constexpr Class kClass2{
      "Class2", Method{"Foo", jni::Return{}, jni::Params{kClass}}};

  LocalObject<kClass2> b{};
  b("Foo", LocalObject<kClass>{});
}

TEST_F(JniTest, LocalObject_MovesInContainerStruct) {
  struct A {
    const LocalObject<kClass> val;

    A(LocalObject<kClass>&& in) : val(std::move(in)) {}
  };

  EXPECT_CALL(*env_, DeleteLocalRef(AsNewLocalReference(Fake<jobject>())));

  A a{LocalObject<kClass>{Fake<jobject>()}};
}

TEST_F(JniTest, LocalObject_DoesntCrossTalkOverClassMethodIds) {
  static constexpr Class kClass{
      "kClass", Method{"Foo", jni::Return{}, jni::Params<int>{}}};

  static constexpr Class kClass2{
      "kClass2", Method{"Foo", jni::Return{}, jni::Params<int>{}}};

  EXPECT_CALL(*env_, GetMethodID(_, _, StrEq("(I)V"))).Times(2);

  LocalObject<kClass> obj_1{Fake<jobject>(1)};
  LocalObject<kClass2> obj_2{Fake<jobject>(2)};

  // These are different method IDs (they are different classes).
  obj_1("Foo", 1);
  obj_2("Foo", 1);
}

}  // namespace
