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

#include "jni_env.h"

#include <gtest/gtest.h>
#include "jni_dep.h"

namespace {

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
  JniEnvForTest::SetEnv_ForTest(reinterpret_cast<JNIEnv*>(0xAAAAAA));
  EXPECT_EQ(JniEnvForTest::GetEnv(), reinterpret_cast<JNIEnv*>(0xAAAAAA));
  JniEnvForTest::SetEnv_ForTest(reinterpret_cast<JNIEnv*>(0xBBBBBB));
  EXPECT_EQ(JniEnvForTest::GetEnv(), reinterpret_cast<JNIEnv*>(0xBBBBBB));
  JniEnvForTest::SetEnv_ForTest(nullptr);
  EXPECT_EQ(JniEnvForTest::GetEnv(), nullptr);
}

// TODO:  Write a test that spawns multiple threads and validates they all
// separately cache their JNIENV*.

}  // namespace
