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

#include "jni_helper.h"

#include <gtest/gtest.h>
#include "implementation/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_dep.h"
#include "jni_test.h"
#include "mock_jni_env.h"

namespace {

using jni::JniHelper;
using jni::Method;
using jni::Params;
using jni::Return;
using jni::test::Fake;
using jni::test::JniTest;
using jni::test::MockJniEnv;
using testing::_;
using testing::Eq;
using testing::InSequence;
using testing::StrEq;

TEST_F(JniTest, JniHelper_CallsReleaseClass) {
  InSequence seq;
  EXPECT_CALL(*env_, FindClass(StrEq("Test2")))
      .Times(1)
      .WillOnce(testing::Return(Fake<jclass>()));

  EXPECT_EQ(JniHelper::FindClass("Test2"), Fake<jclass>());
  JniHelper::ReleaseClass(Fake<jclass>());
}

TEST_F(JniTest, JniHelper_CallsNewObjectV) {
  EXPECT_CALL(*env_, NewObjectV(Eq(Fake<jclass>()), Eq(Fake<jmethodID>()), _));
  JniHelper::NewLocalObject(Fake<jclass>(), Fake<jmethodID>());
}

TEST_F(JniTest, JniHelper_CallsDeleteLocalObject) {
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>()));
  JniHelper::DeleteLocalObject(Fake<jobject>());
}

TEST_F(JniTest, JniHelper_CallsNewLocalString) {
  const char* fake_str{reinterpret_cast<const char*>(0xAAAAA)};
  EXPECT_CALL(*env_, NewStringUTF(fake_str));
  JniHelper::NewLocalString(fake_str);
}

TEST_F(JniTest, JniHelper_CallsGlobalRefOnProvidedLocalString) {
  InSequence seq;
  EXPECT_CALL(*env_, NewGlobalRef(Fake<jstring>()));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jstring>()));
  JniHelper::PromoteLocalToGlobalString(Fake<jstring>());
}

TEST_F(JniTest, JniHelper_CallsDeleteGlobalRef) {
  EXPECT_CALL(*env_, DeleteGlobalRef(Fake<jstring>()));
  JniHelper::DeleteGlobalString(Fake<jstring>());
}

TEST_F(JniTest, JniHelper_CallsNewStringUTF) {
  EXPECT_CALL(*env_, GetStringUTFChars(Fake<jstring>(), nullptr));
  JniHelper::GetStringUTFChars(Fake<jstring>());
}

TEST_F(JniTest, JniHelper_CallsReleaseStringUTFChars) {
  const char* fake_pinned_chars{reinterpret_cast<const char*>(0xBBBBB)};
  EXPECT_CALL(*env_, ReleaseStringUTFChars(Fake<jstring>(), fake_pinned_chars));
  JniHelper::ReleaseStringUTFChars(Fake<jstring>(), fake_pinned_chars);
}

}  // namespace
