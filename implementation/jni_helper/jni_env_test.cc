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

#include "jni_env.h"

#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_dep.h"

namespace {

using ::jni::Fake;

class JniEnvForTest : public jni::JniEnv {
 public:
  static JNIEnv* GetEnv_ForTest() { return GetEnv(); }
  static void SetEnv_ForTest(JNIEnv* env) { return SetEnv(env); }
};

// Why not split this test apart?
//
// JniEnv is by design based on a process level static.  Separate tests here
// will actually have crosstalk with variables and simply re-ordering them could
// cause tests to fail (which would be totally unobvious).
//
// Instead, they are crammed into this single test and this obnoxious comment
// has been added.
TEST(JniEnv, StartsNullAndSetsAndGetsTheSameValue) {
  EXPECT_EQ(JniEnvForTest::GetEnv(), nullptr);
  JniEnvForTest::SetEnv_ForTest(Fake<JNIEnv*>(1));
  EXPECT_EQ(JniEnvForTest::GetEnv(), Fake<JNIEnv*>(1));
  JniEnvForTest::SetEnv_ForTest(Fake<JNIEnv*>(2));
  EXPECT_EQ(JniEnvForTest::GetEnv(), Fake<JNIEnv*>(2));
  JniEnvForTest::SetEnv_ForTest(nullptr);
  EXPECT_EQ(JniEnvForTest::GetEnv(), nullptr);
}

// TODO:  Write a test that spawns multiple threads and validates they all
// separately cache their JNIENV*.

}  // namespace
