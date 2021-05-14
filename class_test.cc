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

#include "class.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "global_object.h"
#include "jni_dep.h"
#include "jni_test.h"
#include "local_object.h"
#include "mock_jni_env.h"

namespace {

using jni::Class;
using jni::GlobalObject;
using jni::LocalObject;
using jni::test::JniTest;

TEST_F(JniTest, Class_CallsNewObjectForConstructor) {
  static constexpr Class c{"jcg/ARCore"};

  EXPECT_CALL(*env_, NewObjectV).Times(2);
  LocalObject<c> local{};
  GlobalObject<c> global{};
}

TEST_F(JniTest, Class_DoesntCallNewObjectForWrap) {
  static constexpr Class c{"jcg/ARCore"};
  const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};

  EXPECT_CALL(*env_, NewObjectV).Times(0);
  EXPECT_CALL(*env_, NewObjectA).Times(0);
  EXPECT_CALL(*env_, NewObjectArray).Times(0);
  GlobalObject<c> global_object{object};
  LocalObject<c> local_object{object};
}

}  // namespace
