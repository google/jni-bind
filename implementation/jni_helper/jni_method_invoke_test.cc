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

#include "jni_method_invoke.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_dep.h"
#include "jni_test.h"
#include "mock_jni_env.h"

namespace {

using jni::JniMethodInvoke;
using jni::test::JniTest;
using jni::test::MockJniEnv;
using testing::_;
using testing::Return;

TEST_F(JniTest, JniMethodInvoke_InvokesVoidMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallVoidMethodV(object, method, _)).Times(3);

  JniMethodInvoke<void>::Invoke(object, method, 1);
  JniMethodInvoke<void>::Invoke(object, method, 1, 2);
  JniMethodInvoke<void>::Invoke(object, method, 1, 2, 3);
}

TEST_F(JniTest, JniMethodInvoke_InvokesBooleanMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallBooleanMethodV(object, method, _))
      .WillOnce(Return(true))
      .WillOnce(Return(false))
      .WillOnce(Return(true));

  EXPECT_EQ(JniMethodInvoke<jboolean>::Invoke(object, method, 1), true);
  EXPECT_EQ(JniMethodInvoke<jboolean>::Invoke(object, method, 1, 2), false);
  EXPECT_EQ(JniMethodInvoke<jboolean>::Invoke(object, method, 1, 2, 3), true);
}

TEST_F(JniTest, JniMethodInvoke_InvokesIntMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallIntMethodV(object, method, _))
      .Times(3)
      .WillRepeatedly(Return(123));

  EXPECT_EQ(JniMethodInvoke<jint>::Invoke(object, method, 1), 123);
  EXPECT_EQ(JniMethodInvoke<jint>::Invoke(object, method, 1, 2), 123);
  EXPECT_EQ(JniMethodInvoke<jint>::Invoke(object, method, 1, 2, 3), 123);
}

TEST_F(JniTest, JniMethodInvoke_InvokesLongMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallLongMethodV(object, method, _))
      .Times(3)
      .WillRepeatedly(Return(123L));

  EXPECT_EQ(JniMethodInvoke<jlong>::Invoke(object, method, 1), 123L);
  EXPECT_EQ(JniMethodInvoke<jlong>::Invoke(object, method, 1, 2), 123L);
  EXPECT_EQ(JniMethodInvoke<jlong>::Invoke(object, method, 1, 2, 3), 123L);
}

TEST_F(JniTest, JniMethodInvoke_InvokesFloatMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, CallFloatMethodV(object, method, _))
      .Times(3)
      .WillRepeatedly(Return(123));

  EXPECT_EQ(JniMethodInvoke<jfloat>::Invoke(object, method, 1), 123);
  EXPECT_EQ(JniMethodInvoke<jfloat>::Invoke(object, method, 1, 2), 123);
  EXPECT_EQ(JniMethodInvoke<jfloat>::Invoke(object, method, 1, 2, 3), 123);
}

TEST_F(JniTest, JniMethodInvoke_InvokesObjectMethod) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};
  static const jobject result{reinterpret_cast<jobject>(0XBBBBBB)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XCCCCCC)};
  EXPECT_CALL(*env_, CallObjectMethodV(object, method, _))
      .Times(3)
      .WillRepeatedly(Return(result));

  EXPECT_EQ(JniMethodInvoke<jobject>::Invoke(object, method, 1), result);
  EXPECT_EQ(JniMethodInvoke<jobject>::Invoke(object, method, 1, 2), result);
  EXPECT_EQ(JniMethodInvoke<jobject>::Invoke(object, method, 1, 2, 3), result);
}

}  // namespace
