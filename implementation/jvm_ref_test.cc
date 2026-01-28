/*
 * Copyright 2024 Google LLC
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

#include "implementation/jvm_ref.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

using ::jni::Class;
using ::jni::Fake;
using ::jni::JvmRef;
using ::jni::LocalObject;
using ::jni::test::AsGlobal;
using ::jni::test::JniTest;
using ::jni::test::JniTestWithNoDefaultJvmRef;
using ::jni::test::kDefaultConfiguration;
using ::testing::AnyNumber;
using ::testing::Return;

namespace {

TEST_F(JniTest, JvmRefTearsDownClassesLoadedfromDefaultLoader) {
  // Note, this expectation means FindClass is called *exactly* once.
  // Using offset to avoid Fake<jclass>() usage in jni_test.h.
  EXPECT_CALL(*env_, FindClass(testing::StrEq("android/app/ActivityThread")))
      .Times(AnyNumber())
      .WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*env_, ExceptionClear()).Times(AnyNumber());
  EXPECT_CALL(*env_, FindClass(testing::StrEq("com/google/Class1")))
      .WillOnce(Return(Fake<jclass>(1)));
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(AnyNumber());
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(1))));

  static constexpr Class kClass1{"com/google/Class1"};
  LocalObject<kClass1> local_object1{};
}

TEST_F(JniTest, NoStaticCrossTalkWithPriorTest) {
  // Note, this expectation means FindClass is called *exactly* once.
  // Using 1 offset to avoid Fake<jclass>() usage in jni_test.h.
  EXPECT_CALL(*env_, FindClass(testing::StrEq("android/app/ActivityThread")))
      .Times(AnyNumber())
      .WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*env_, ExceptionClear()).Times(AnyNumber());
  EXPECT_CALL(*env_, FindClass(testing::StrEq("com/google/Class1")))
      .WillOnce(Return(Fake<jclass>(1)));
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(AnyNumber());
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(1))));

  static constexpr Class kClass1{"com/google/Class1"};
  LocalObject<kClass1> local_object1{};
}

TEST_F(JniTest, NoStaticCrossTalkWithUnrelatedTest) {
  // Note, this expectation means FindClass is called *exactly* once.
  // Using 1 offset to avoid Fake<jclass>() usage in jni_test.h.
  EXPECT_CALL(*env_, FindClass(testing::StrEq("android/app/ActivityThread")))
      .Times(AnyNumber())
      .WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*env_, ExceptionClear()).Times(AnyNumber());
  EXPECT_CALL(*env_, FindClass(testing::StrEq("com/google/Class2")))
      .WillOnce(Return(Fake<jclass>(1)));
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(AnyNumber());
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(1))));

  static constexpr Class kClass1{"com/google/Class2"};
  LocalObject<kClass1> local_object1{};
}

TEST_F(JniTestWithNoDefaultJvmRef,
       JvmsNeitherQueryNorReleaseIfNoObjectsCreated) {
  EXPECT_CALL(*env_, FindClass(testing::StrEq("android/app/ActivityThread")))
      .Times(AnyNumber())
      .WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*env_, ExceptionClear()).Times(AnyNumber());
  EXPECT_CALL(*env_, FindClass(testing::Ne("android/app/ActivityThread")))
      .Times(0);

  JvmRef<jni::kDefaultJvm> jvm_ref{jvm_.get(), kDefaultConfiguration};
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(0);
}

TEST_F(JniTestWithNoDefaultJvmRef, JvmRefsDontReuuseStaleFindClassValues) {
  EXPECT_CALL(*env_, FindClass(testing::StrEq("android/app/ActivityThread")))
      .Times(AnyNumber())
      .WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*env_, ExceptionClear()).Times(AnyNumber());
  EXPECT_CALL(*env_,
              FindClass(testing::StrEq("com/google/ADifferentClassForVariety")))
      .WillOnce(Return(Fake<jclass>(1)))
      .WillOnce(Return(Fake<jclass>(2)));
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(AnyNumber());
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(1))));
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(2))));

  static constexpr Class kClass1{"com/google/ADifferentClassForVariety"};
  {
    JvmRef<jni::kDefaultJvm> jvm_ref{jvm_.get(), kDefaultConfiguration};
    LocalObject<kClass1> local_object1{};
  }

  {
    JvmRef<jni::kDefaultJvm> jvm_ref{jvm_.get(), kDefaultConfiguration};
    LocalObject<kClass1> local_object1{};
  }
}

TEST_F(JniTest, DefaultLoaderReleasesMultipleClasses) {
  EXPECT_CALL(*env_, FindClass(testing::StrEq("android/app/ActivityThread")))
      .Times(AnyNumber())
      .WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*env_, ExceptionClear()).Times(AnyNumber());
  EXPECT_CALL(*env_, FindClass(testing::StrEq("com/google/Class1")))
      .WillOnce(Return(Fake<jclass>(1)));
  EXPECT_CALL(*env_, FindClass(testing::StrEq("com/google/Class2")))
      .WillOnce(Return(Fake<jclass>(2)));
  EXPECT_CALL(*env_, FindClass(testing::StrEq("com/google/Class3")))
      .WillOnce(Return(Fake<jclass>(3)));
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(AnyNumber());
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(1))));
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(2))));
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(3))));

  static constexpr Class kClass1{"com/google/Class1"};
  static constexpr Class kClass2{"com/google/Class2"};
  static constexpr Class kClass3{"com/google/Class3"};

  LocalObject<kClass1> local_object1{};
  LocalObject<kClass2> local_object2{};
  LocalObject<kClass3> local_object3{};
}

TEST_F(JniTestWithNoDefaultJvmRef, JvmRefsDontTeardownIfConfigurationIsFalse) {
  EXPECT_CALL(*env_, FindClass(testing::StrEq("android/app/ActivityThread")))
      .Times(AnyNumber())
      .WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*env_, ExceptionClear()).Times(AnyNumber());
  EXPECT_CALL(*env_, FindClass(testing::StrEq("com/google/UniqueClass1")))
      .WillOnce(Return(Fake<jclass>(1)));

  // This is the core of the test: DeleteGlobalRef should NOT be called.
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(0);

  static constexpr jni::Configuration kNoTeardownConfig{
      .release_class_ids_on_teardown_ = false,
      .release_method_ids_on_teardown_ = false,
      .release_field_ids_on_teardown_ = false,
  };

  {
    JvmRef<jni::kDefaultJvm> jvm_ref{jvm_.get(), kNoTeardownConfig};
    static constexpr Class kClass1{"com/google/UniqueClass1"};
    LocalObject<kClass1> local_object1{};
  }
}

TEST_F(JniTestWithNoDefaultJvmRef, JvmRefsTeardownIfConfigurationIsTrue) {
  EXPECT_CALL(*env_, FindClass(testing::StrEq("android/app/ActivityThread")))
      .Times(AnyNumber())
      .WillRepeatedly(Return(nullptr));
  EXPECT_CALL(*env_, ExceptionClear()).Times(AnyNumber());
  EXPECT_CALL(*env_, FindClass(testing::StrEq("com/google/UniqueClass2")))
      .WillOnce(Return(Fake<jclass>(1)));

  // This is the core of the test: DeleteGlobalRef SHOULD be called.
  EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>(1))));

  static constexpr jni::Configuration kTeardownConfig{
      .release_class_ids_on_teardown_ = true,
      .release_method_ids_on_teardown_ = true,
      .release_field_ids_on_teardown_ = true,
  };

  {
    JvmRef<jni::kDefaultJvm> jvm_ref{jvm_.get(), kTeardownConfig};
    static constexpr Class kClass1{"com/google/UniqueClass2"};
    LocalObject<kClass1> local_object1{};
  }
}

}  // namespace
