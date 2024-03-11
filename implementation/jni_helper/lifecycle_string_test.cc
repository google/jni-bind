/*
 * Copyright 2023 Google LLC
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
#include "implementation/jni_helper/lifecycle_string.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

using ::jni::Fake;
using ::jni::LifecycleHelper;
using ::jni::LifecycleType;
using ::jni::test::JniTest;
using ::testing::Eq;

namespace {

////////////////////////////////////////////////////////////////////////////////
// Local jstring.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Lifecycle_jstring_Local_CallsNewLocalRef) {
  EXPECT_CALL(*env_, NewLocalRef(Eq(Fake<jstring>())));
  LifecycleHelper<jstring, LifecycleType::LOCAL>::NewReference(Fake<jstring>());
}

TEST_F(JniTest, Lifecycle_jstring_Local_CallsDeleteLocalstring) {
  EXPECT_CALL(*env_, DeleteLocalRef(Eq(Fake<jstring>())));
  LifecycleHelper<jstring, LifecycleType::LOCAL>::Delete(Fake<jstring>());
}

TEST_F(JniTest, Lifecycle_jstring_Local_CallsNewstringV) {
  const char* fake_str = "foo";
  EXPECT_CALL(*env_, NewStringUTF(fake_str));
  LifecycleHelper<jstring, LifecycleType::LOCAL>::Construct(fake_str);
}

////////////////////////////////////////////////////////////////////////////////
// Global jstring.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Lifecycle_jstring_Global_CallsNewGlobalRef) {
  EXPECT_CALL(*env_, NewGlobalRef(Eq(Fake<jstring>())));
  LifecycleHelper<jstring, LifecycleType::GLOBAL>::NewReference(
      Fake<jstring>());
}

TEST_F(JniTest, Lifecycle_jstring_Global_CallsDeleteGlobalstring) {
  EXPECT_CALL(*env_, DeleteGlobalRef(Eq(Fake<jstring>())));
  LifecycleHelper<jstring, LifecycleType::GLOBAL>::Delete(Fake<jstring>());
}

TEST_F(JniTest, Lifecycle_jstring_Global_CallsNewstringV) {
  const char* fake_str = "foo";
  EXPECT_CALL(*env_, NewStringUTF(fake_str));
  EXPECT_CALL(*env_, NewGlobalRef);

  LifecycleHelper<jstring, LifecycleType::GLOBAL>::Construct(fake_str);
}

TEST_F(JniTest, Lifecycle_jstring_Global_Promotes) {
  EXPECT_CALL(*env_, NewGlobalRef(Eq(Fake<jstring>())));
  LifecycleHelper<jstring, LifecycleType::GLOBAL>::Promote(Fake<jstring>());
}

}  // namespace
