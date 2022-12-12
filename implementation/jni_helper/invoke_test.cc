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
#include "jni_dep.h"
#include "jni_test.h"
#include "mock_jni_env.h"

namespace {

using jni::InvokeHelper;
using jni::test::JniTest;
using jni::test::MockJniEnv;
using testing::_;
using testing::Return;

TEST_F(JniTest, InvokeHelper_InvokesVoidMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallVoidMethodV(object, method, _)).Times(3);

  InvokeHelper<void, 0, false>::Invoke(object, nullptr, method, 1);
  InvokeHelper<void, 0, false>::Invoke(object, nullptr, method, 1, 2);
  InvokeHelper<void, 0, false>::Invoke(object, nullptr, method, 1, 2, 3);
}

TEST_F(JniTest, InvokeHelper_InvokesBooleanMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallBooleanMethodV(object, method, _))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true));

  EXPECT_EQ(
      (InvokeHelper<jboolean, 1, false>::Invoke(object, nullptr, method, 1)),
      true);
  EXPECT_EQ(
      (InvokeHelper<jboolean, 1, false>::Invoke(object, nullptr, method, 1, 2)),
      false);
  EXPECT_EQ((InvokeHelper<jboolean, 1, false>::Invoke(object, nullptr, method,
                                                      1, 2, 3)),
            true);
}

TEST_F(JniTest, InvokeHelper_InvokesIntMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallIntMethodV(object, method, _))
      .Times(3)
      .WillRepeatedly(Return(123));

  EXPECT_EQ((InvokeHelper<jint, 1, false>::Invoke(object, nullptr, method, 1)),
            123);
  EXPECT_EQ(
      (InvokeHelper<jint, 1, false>::Invoke(object, nullptr, method, 1, 2)),
      123);
  EXPECT_EQ(
      (InvokeHelper<jint, 1, false>::Invoke(object, nullptr, method, 1, 2, 3)),
      123);
}

TEST_F(JniTest, InvokeHelper_InvokesLongMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallLongMethodV(object, method, _))
      .Times(3)
      .WillRepeatedly(Return(123L));

  EXPECT_EQ((InvokeHelper<jlong, 1, false>::Invoke(object, nullptr, method, 1)),
            123L);
  EXPECT_EQ(
      (InvokeHelper<jlong, 1, false>::Invoke(object, nullptr, method, 1, 2)),
      123L);
  EXPECT_EQ(
      (InvokeHelper<jlong, 1, false>::Invoke(object, nullptr, method, 1, 2, 3)),
      123L);
}

TEST_F(JniTest, InvokeHelper_InvokesFloatMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallFloatMethodV(object, method, _))
      .Times(3)
      .WillRepeatedly(Return(123));

  EXPECT_EQ(
      (InvokeHelper<jfloat, 1, false>::Invoke(object, nullptr, method, 1)),
      123);
  EXPECT_EQ(
      (InvokeHelper<jfloat, 1, false>::Invoke(object, nullptr, method, 1, 2)),
      123);
  EXPECT_EQ((InvokeHelper<jfloat, 1, false>::Invoke(object, nullptr, method, 1,
                                                    2, 3)),
            123);
}

TEST_F(JniTest, InvokeHelper_InvokesObjectMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jobject result{reinterpret_cast<jobject>(0XBBBBBB)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XCCCCCC)};
  EXPECT_CALL(*env_, CallObjectMethodV(object, method, _))
      .Times(3)
      .WillRepeatedly(Return(result));

  EXPECT_EQ(
      (InvokeHelper<jobject, 1, false>::Invoke(object, nullptr, method, 1)),
      result);
  EXPECT_EQ(
      (InvokeHelper<jobject, 1, false>::Invoke(object, nullptr, method, 1, 2)),
      result);
  EXPECT_EQ((InvokeHelper<jobject, 1, false>::Invoke(object, nullptr, method, 1,
                                                     2, 3)),
            result);
}

}  // namespace
