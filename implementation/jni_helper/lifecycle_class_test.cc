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
#include "implementation/jni_helper/lifecycle_class.h"

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
// Local jclass.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Lifecycle_jclass_Local_CallsNewLocalRef) {
  EXPECT_CALL(*env_, NewLocalRef(Eq(Fake<jclass>())));
  LifecycleHelper<jclass, LifecycleType::LOCAL>::NewReference(Fake<jclass>());
}

TEST_F(JniTest, Lifecycle_jclass_Local_CallsDeleteLocalObject) {
  EXPECT_CALL(*env_, DeleteLocalRef(Eq(Fake<jclass>())));
  LifecycleHelper<jclass, LifecycleType::LOCAL>::Delete(Fake<jclass>());
}

TEST_F(JniTest, Lifecycle_jclass_Local_CallsNewObjectV) {
  EXPECT_CALL(*env_, NewObjectV(Eq(Fake<jclass>()), Eq(Fake<jmethodID>()), _));
  LifecycleHelper<jclass, LifecycleType::LOCAL>::Construct(
      Fake<jclass>(), Fake<jmethodID>(), 1, 2, 3);
}

////////////////////////////////////////////////////////////////////////////////
// Global jclass.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Lifecycle_jclass_Global_CallsNewGlobalRef) {
  EXPECT_CALL(*env_, NewGlobalRef(Eq(Fake<jclass>())));
  LifecycleHelper<jclass, LifecycleType::GLOBAL>::NewReference(Fake<jclass>());
}

TEST_F(JniTest, Lifecycle_jclass_Global_CallsDeleteGlobalObject) {
  EXPECT_CALL(*env_, DeleteGlobalRef(Eq(Fake<jclass>())));
  LifecycleHelper<jclass, LifecycleType::GLOBAL>::Delete(Fake<jclass>());
}

TEST_F(JniTest, Lifecycle_jclass_Global_CallsNewObjectV) {
  EXPECT_CALL(*env_, NewObjectV(Eq(Fake<jclass>()), Eq(Fake<jmethodID>()), _));
  LifecycleHelper<jclass, LifecycleType::GLOBAL>::Construct(
      Fake<jclass>(), Fake<jmethodID>(), 1, 2, 3);
}

TEST_F(JniTest, Lifecycle_jclass_Global_Promotes) {
  EXPECT_CALL(*env_, NewGlobalRef(Eq(Fake<jclass>())));
  EXPECT_CALL(*env_, DeleteLocalRef(Eq(Fake<jclass>())));
  LifecycleHelper<jclass, LifecycleType::GLOBAL>::Promote(Fake<jclass>());
}

}  // namespace
