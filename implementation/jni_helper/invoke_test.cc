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
#include "invoke.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/fake_test_constants.h"
#include "jni_dep.h"
#include "jni_test.h"
#include "mock_jni_env.h"

namespace {

using jni::InvokeHelper;
using jni::test::Fake;
using jni::test::JniTest;
using jni::test::MockJniEnv;
using testing::_;
using testing::Return;

TEST_F(JniTest, InvokeHelper_InvokesVoidMethod) {
  EXPECT_CALL(*env_, CallVoidMethodV(Fake<jobject>(), Fake<jmethodID>(), _))
      .Times(3);

  InvokeHelper<void, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                       Fake<jmethodID>(), 1);
  InvokeHelper<void, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                       Fake<jmethodID>(), 1, 2);
  InvokeHelper<void, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                       Fake<jmethodID>(), 1, 2, 3);
}

TEST_F(JniTest, InvokeHelper_InvokesBooleanMethod) {
  EXPECT_CALL(*env_, CallBooleanMethodV(Fake<jobject>(), Fake<jmethodID>(), _))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true));

  EXPECT_EQ((InvokeHelper<jboolean, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                      Fake<jmethodID>(), 1)),
            true);
  EXPECT_EQ((InvokeHelper<jboolean, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                      Fake<jmethodID>(), 1, 2)),
            false);
  EXPECT_EQ((InvokeHelper<jboolean, 0, false>::Invoke(
                Fake<jobject>(), nullptr, Fake<jmethodID>(), 1, 2, 3)),
            true);
}

TEST_F(JniTest, InvokeHelper_InvokesIntMethod) {
  EXPECT_CALL(*env_, CallIntMethodV(Fake<jobject>(), Fake<jmethodID>(), _))
      .Times(3)
      .WillRepeatedly(Return(123));

  EXPECT_EQ((InvokeHelper<jint, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                  Fake<jmethodID>(), 1)),
            123);
  EXPECT_EQ((InvokeHelper<jint, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                  Fake<jmethodID>(), 1, 2)),
            123);
  EXPECT_EQ((InvokeHelper<jint, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                  Fake<jmethodID>(), 1, 2, 3)),
            123);
}

TEST_F(JniTest, InvokeHelper_InvokesLongMethod) {
  EXPECT_CALL(*env_, CallLongMethodV(Fake<jobject>(), Fake<jmethodID>(), _))
      .Times(3)
      .WillRepeatedly(Return(123L));

  EXPECT_EQ((InvokeHelper<jlong, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                   Fake<jmethodID>(), 1)),
            123L);
  EXPECT_EQ((InvokeHelper<jlong, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                   Fake<jmethodID>(), 1, 2)),
            123L);
  EXPECT_EQ((InvokeHelper<jlong, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                   Fake<jmethodID>(), 1, 2, 3)),
            123L);
}

TEST_F(JniTest, InvokeHelper_InvokesFloatMethod) {
  EXPECT_CALL(*env_, CallFloatMethodV(Fake<jobject>(), Fake<jmethodID>(), _))
      .Times(3)
      .WillRepeatedly(Return(123));

  EXPECT_EQ((InvokeHelper<jfloat, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                    Fake<jmethodID>(), 1)),
            123);
  EXPECT_EQ((InvokeHelper<jfloat, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                    Fake<jmethodID>(), 1, 2)),
            123);
  EXPECT_EQ((InvokeHelper<jfloat, 0, false>::Invoke(
                Fake<jobject>(), nullptr, Fake<jmethodID>(), 1, 2, 3)),
            123);
}

TEST_F(JniTest, InvokeHelper_InvokesObjectMethod) {
  EXPECT_CALL(*env_, CallObjectMethodV(Fake<jobject>(), Fake<jmethodID>(), _))
      .Times(3)
      .WillRepeatedly(Return(Fake<jobject>()));

  EXPECT_EQ((InvokeHelper<jobject, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                     Fake<jmethodID>(), 1)),
            Fake<jobject>());
  EXPECT_EQ((InvokeHelper<jobject, 0, false>::Invoke(Fake<jobject>(), nullptr,
                                                     Fake<jmethodID>(), 1, 2)),
            Fake<jobject>());
  EXPECT_EQ((InvokeHelper<jobject, 0, false>::Invoke(
                Fake<jobject>(), nullptr, Fake<jmethodID>(), 1, 2, 3)),
            Fake<jobject>());
}

}  // namespace
