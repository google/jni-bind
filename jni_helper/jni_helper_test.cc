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

#include "jni_helper.h"

#include <gtest/gtest.h>
#include "field.h"
#include "jni_dep.h"
#include "jni_test.h"
#include "method.h"
#include "mock_jni_env.h"
#include "params.h"
#include "return.h"

namespace {

using jni::JniHelper;
using jni::Method;
using jni::Params;
using jni::Return;
using jni::test::JniTest;
using jni::test::MockJniEnv;
using testing::_;
using testing::Eq;
using testing::InSequence;
using testing::StrEq;

TEST_F(JniTest, JniHelper_CallsReleaseClass) {
  InSequence seq;
  static const jclass clazz{reinterpret_cast<jclass>(0XAAAAA)};
  EXPECT_CALL(*env_, FindClass(StrEq("Test2")))
      .Times(1)
      .WillOnce(testing::Return(clazz));

  EXPECT_EQ(JniHelper::FindClass("Test2"), clazz);
  JniHelper::ReleaseClass(clazz);
}

TEST_F(JniTest, JniHelper_CallsNewObjectV) {
  static const jclass clazz{reinterpret_cast<jclass>(0XAAAAA)};
  static const jmethodID method{reinterpret_cast<jmethodID>(0XBBBBB)};
  EXPECT_CALL(*env_, NewObjectV(Eq(clazz), Eq(method), _));
  JniHelper::NewLocalObject(clazz, method);
}

TEST_F(JniTest, JniHelper_CallsDeleteLocalObject) {
  static const jobject object{reinterpret_cast<jobject>(0XAAAAA)};
  EXPECT_CALL(*env_, DeleteLocalRef(object));
  JniHelper::DeleteLocalObject(object);
}

TEST_F(JniTest, JniHelper_CallsNewLocalString) {
  const char* fake_str{reinterpret_cast<const char*>(0xAAAAA)};
  EXPECT_CALL(*env_, NewStringUTF(fake_str));
  JniHelper::NewLocalString(fake_str);
}

TEST_F(JniTest, JniHelper_CallsGlobalRefOnProvidedLocalString) {
  InSequence seq;
  jstring fake_str{reinterpret_cast<jstring>(0xAAAAA)};
  EXPECT_CALL(*env_, NewGlobalRef(fake_str));
  EXPECT_CALL(*env_, DeleteLocalRef(fake_str));
  JniHelper::PromoteLocalToGlobalString(fake_str);
}

TEST_F(JniTest, JniHelper_CallsDeleteGlobalRef) {
  jstring fake_str{reinterpret_cast<jstring>(0xAAAAA)};
  EXPECT_CALL(*env_, DeleteGlobalRef(fake_str));
  JniHelper::DeleteGlobalString(fake_str);
}

TEST_F(JniTest, JniHelper_CallsNewStringUTF) {
  jstring fake_str{reinterpret_cast<jstring>(0xAAAAA)};
  EXPECT_CALL(*env_, GetStringUTFChars(fake_str, nullptr));
  JniHelper::GetStringUTFChars(fake_str);
}

TEST_F(JniTest, JniHelper_CallsReleaseStringUTFChars) {
  jstring fake_jstr{reinterpret_cast<jstring>(0xAAAAA)};
  const char* fake_pinned_chars{reinterpret_cast<const char*>(0xBBBBB)};
  EXPECT_CALL(*env_, ReleaseStringUTFChars(fake_jstr, fake_pinned_chars));
  JniHelper::ReleaseStringUTFChars(fake_jstr, fake_pinned_chars);
}

}  // namespace
