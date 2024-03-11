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
#include "implementation/jni_helper/fake_test_constants.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "jni_bind.h"
#include "jni_dep.h"
#include "jni_test.h"
#include "mock_jni_env.h"

namespace {

using ::jni::Fake;
using ::jni::JniHelper;
using ::jni::LifecycleHelper;
using ::jni::LifecycleType;
using ::jni::test::JniTest;
using ::testing::InSequence;
using ::testing::StrEq;

TEST_F(JniTest, JniHelper_CallsReleaseClass) {
  InSequence seq;
  EXPECT_CALL(*env_, FindClass(StrEq("Test2")))
      .Times(1)
      .WillOnce(testing::Return(Fake<jclass>()));

  EXPECT_EQ(JniHelper::FindClass("Test2"), Fake<jclass>());
  LifecycleHelper<jclass, LifecycleType::GLOBAL>::Delete(Fake<jclass>());
}

TEST_F(JniTest, JniHelper_CallsNewStringUTF) {
  EXPECT_CALL(*env_, GetStringUTFChars(Fake<jstring>(), nullptr));
  JniHelper::GetStringUTFChars(Fake<jstring>());
}

TEST_F(JniTest, JniHelper_CallsReleaseStringUTFChars) {
  const char* fake_pinned_chars = "foo";
  EXPECT_CALL(*env_, ReleaseStringUTFChars(Fake<jstring>(), fake_pinned_chars));
  JniHelper::ReleaseStringUTFChars(Fake<jstring>(), fake_pinned_chars);
}

}  // namespace
