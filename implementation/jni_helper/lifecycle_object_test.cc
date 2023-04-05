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
#include "implementation/jni_helper/lifecycle_object.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

using ::jni::LifecycleHelper;
using ::jni::LifecycleType;
using ::jni::test::Fake;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Eq;

namespace {

////////////////////////////////////////////////////////////////////////////////
// Local jobject.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Lifecycle_jobject_Local_CallsNewLocalRef) {
  EXPECT_CALL(*env_, NewLocalRef(Eq(Fake<jobject>())));
  LifecycleHelper<jobject, LifecycleType::LOCAL>::NewReference(Fake<jobject>());
}

TEST_F(JniTest, Lifecycle_jobject_Local_CallsDeleteLocalObject) {
  EXPECT_CALL(*env_, DeleteLocalRef(Eq(Fake<jobject>())));
  LifecycleHelper<jobject, LifecycleType::LOCAL>::Delete(Fake<jobject>());
}

TEST_F(JniTest, Lifecycle_jobject_Local_CallsNewObjectV) {
  EXPECT_CALL(*env_, NewObjectV(Eq(Fake<jclass>()), Eq(Fake<jmethodID>()), _));
  LifecycleHelper<jobject, LifecycleType::LOCAL>::Construct(
      Fake<jclass>(), Fake<jmethodID>(), 1, 2, 3);
}

////////////////////////////////////////////////////////////////////////////////
// Global jobject.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Lifecycle_jobject_Global_CallsNewGlobalRef) {
  EXPECT_CALL(*env_, NewGlobalRef(Eq(Fake<jobject>())));
  LifecycleHelper<jobject, LifecycleType::GLOBAL>::NewReference(
      Fake<jobject>());
}

TEST_F(JniTest, Lifecycle_jobject_Global_CallsDeleteGlobalObject) {
  EXPECT_CALL(*env_, DeleteGlobalRef(Eq(Fake<jobject>())));
  LifecycleHelper<jobject, LifecycleType::GLOBAL>::Delete(Fake<jobject>());
}

TEST_F(JniTest, Lifecycle_jobject_Global_CallsNewObjectV) {
  EXPECT_CALL(*env_, NewObjectV(Eq(Fake<jclass>()), Eq(Fake<jmethodID>()), _));
  LifecycleHelper<jobject, LifecycleType::GLOBAL>::Construct(
      Fake<jclass>(), Fake<jmethodID>(), 1, 2, 3);
}

TEST_F(JniTest, Lifecycle_jobject_Global_Promotes) {
  EXPECT_CALL(*env_, NewGlobalRef(Eq(Fake<jobject>())));
  EXPECT_CALL(*env_, DeleteLocalRef(Eq(Fake<jobject>())));
  LifecycleHelper<jobject, LifecycleType::GLOBAL>::Promote(Fake<jobject>());
}

}  // namespace
