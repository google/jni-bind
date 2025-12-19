/*
 * Copyright 2025 Google LLC
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

#include "implementation/local_exception.h"

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::ClassRef;
using ::jni::Fake;
using ::jni::JniT;
using ::jni::LocalException;
using ::jni::StaticRef;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

static constexpr jni::Class kClassWithStaticMethod{
    "kClassWithStaticMethod",
    jni::Static{
        jni::Method{"build", jni::Return{jni::kJavaLangException},
                    jni::Params<>{}},
    },
};

static constexpr jni::Class kCustomException{
    "com/jnibind/test/CustomException",
    jni::Constructor<jstring>{},
    jni::Method{"getMessage", jni::Return<jstring>{}, jni::Params<>{}},
};

TEST_F(JniTest, LocalException_ThrowCallsThrowNewWithCorrectMessage) {
  // Allow NewLocalRef/DeleteLocalRef for object creation/destruction
  EXPECT_CALL(*env_, NewLocalRef).Times(testing::AnyNumber());
  EXPECT_CALL(*env_, DeleteLocalRef).Times(testing::AnyNumber());

  // Setup mock for getMessage
  EXPECT_CALL(
      *env_, GetMethodID(_, StrEq("getMessage"), StrEq("()Ljava/lang/String;")))
      .WillOnce(testing::Return(Fake<jmethodID>(1)));

  jstring fake_message = Fake<jstring>(123);
  EXPECT_CALL(*env_, CallObjectMethodV(_, Fake<jmethodID>(1), _))
      .WillOnce(testing::Return(fake_message));

  // Setup mock for GetStringUTFChars (Pin)
  const char* kErrorMessage = "The Error Message";
  EXPECT_CALL(*env_, GetStringUTFChars(fake_message, _))
      .WillOnce(testing::Return(kErrorMessage));

  EXPECT_CALL(*env_, ReleaseStringUTFChars(fake_message, kErrorMessage));

  // Setup mock for ThrowNew
  EXPECT_CALL(*env_, ThrowNew(_, StrEq(kErrorMessage)))
      .WillOnce(testing::Return(0));

  LocalException exception{Fake<jobject>()};
  exception.Throw();
}

TEST_F(JniTest, LocalCustomException_ThrowCallsThrowNewWithCorrectMessage) {
  // Allow NewLocalRef/DeleteLocalRef for object creation/destruction
  EXPECT_CALL(*env_, NewLocalRef).Times(testing::AnyNumber());
  EXPECT_CALL(*env_, DeleteLocalRef).Times(testing::AnyNumber());

  // Setup mock for getMessage
  EXPECT_CALL(
      *env_, GetMethodID(_, StrEq("getMessage"), StrEq("()Ljava/lang/String;")))
      .WillOnce(testing::Return(Fake<jmethodID>(1)));

  jstring fake_message = Fake<jstring>(123);
  EXPECT_CALL(*env_, CallObjectMethodV(_, Fake<jmethodID>(1), _))
      .WillOnce(testing::Return(fake_message));

  // Setup mock for GetStringUTFChars (Pin)
  const char* kErrorMessage = "Custom Error Message";
  EXPECT_CALL(*env_, GetStringUTFChars(fake_message, _))
      .WillOnce(testing::Return(kErrorMessage));

  EXPECT_CALL(*env_, ReleaseStringUTFChars(fake_message, kErrorMessage));

  // Setup mock for ThrowNew
  EXPECT_CALL(
      *env_,
      ThrowNew(
          ClassRef<JniT<jobject, kCustomException>>::GetAndMaybeLoadClassRef(
              nullptr),
          StrEq(kErrorMessage)))
      .WillOnce(testing::Return(0));

  LocalException<kCustomException> exception{Fake<jobject>()};
  exception.Throw();
}

TEST_F(JniTest, StaticMethodReturningLocalObjectAssignedToLocalException) {
  // Allow lifecycle calls for objects.
  EXPECT_CALL(*env_, NewLocalRef).Times(testing::AnyNumber());
  EXPECT_CALL(*env_, DeleteLocalRef).Times(testing::AnyNumber());

  // Mock GetStaticMethodID for the 'build' method.
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("build"),
                                       StrEq("()Ljava/lang/Exception;")))
      .WillOnce(testing::Return(Fake<jmethodID>(2)));

  // Mock CallStaticObjectMethodV to return a fake jobject.
  EXPECT_CALL(*env_, CallStaticObjectMethodV(_, Fake<jmethodID>(2), _))
      .WillOnce(testing::Return(Fake<jobject>(100)));

  // Call the static build method and assign its result to a LocalException.
  LocalException returned_exception =
      StaticRef<kClassWithStaticMethod>{}.Call<"build">();
}

TEST_F(JniTest, LocalException_ConstructsFromStringView) {
  EXPECT_CALL(*env_, NewLocalRef).Times(testing::AnyNumber());
  EXPECT_CALL(*env_, DeleteLocalRef).Times(testing::AnyNumber());

  // String construction.
  EXPECT_CALL(*env_, NewStringUTF(StrEq("Test String View")))
      .WillOnce(testing::Return(Fake<jstring>(1)));

  // Constructor call.
  EXPECT_CALL(*env_, NewObjectV(_, _, _))
      .WillOnce(testing::Return(Fake<jobject>(2)));

  LocalException<> exception{"Test String View"};
}

TEST_F(JniTest, LocalException_ConstructsFromConstChar) {
  EXPECT_CALL(*env_, NewLocalRef).Times(testing::AnyNumber());
  EXPECT_CALL(*env_, DeleteLocalRef).Times(testing::AnyNumber());

  EXPECT_CALL(*env_, NewStringUTF(StrEq("Test Const Char")))
      .WillOnce(testing::Return(Fake<jstring>(1)));
  EXPECT_CALL(*env_, NewObjectV(_, _, _))
      .WillOnce(testing::Return(Fake<jobject>(2)));

  LocalException exception{"Test Const Char"};
}

TEST_F(JniTest, LocalException_ConstructsFromStdString) {
  EXPECT_CALL(*env_, NewLocalRef).Times(testing::AnyNumber());
  EXPECT_CALL(*env_, DeleteLocalRef).Times(testing::AnyNumber());

  EXPECT_CALL(*env_, NewStringUTF(StrEq("Test Std String")))
      .WillOnce(testing::Return(Fake<jstring>(1)));
  EXPECT_CALL(*env_, NewObjectV(_, _, _))
      .WillOnce(testing::Return(Fake<jobject>(2)));

  LocalException exception{std::string{"Test Std String"}};
}

}  // namespace
