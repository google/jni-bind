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

#include "field_ref.h"

#include <optional>

#include "mock_jni_env.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_dep.h"
#include "jni_test.h"

namespace {

using jni::Class;
using jni::Field;

using jni::test::JniTest;
using testing::_;
using testing::Eq;
using testing::InSequence;
using testing::Return;
using testing::StrEq;

TEST_F(JniTest, Field_SimpleGetAndSet) {
  static constexpr Class java_class_under_test{"com/google/TestClass",
                                               Field("SomeField", jint{})};

  jfieldID field_id{reinterpret_cast<jfieldID>(0XDEDEDE)};
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("SomeField"), StrEq("I")))
      .WillOnce(Return(field_id));
  EXPECT_CALL(*env_, GetIntField(_, field_id)).WillOnce(Return(999));
  EXPECT_CALL(*env_, SetIntField(_, field_id, 123));

  jni::GlobalObject<java_class_under_test> obj{};
  EXPECT_EQ(999, obj["SomeField"].Get());
  obj["SomeField"].Set(123);
}

}  // namespace
