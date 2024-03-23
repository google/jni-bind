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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::Fake;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;
using ::jni::Self;
using ::jni::test::AsGlobal;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::StrEq;

static constexpr Class kClass{"Builder", Method{"build", Return{Self{}}},
                              Method{"takesBuilder", Return{}, Params{Self{}}}};

TEST_F(JniTest, SelfCanBeUsedAsAReturnValue) {
  EXPECT_CALL(*env_, FindClass(StrEq("Builder")))
      .WillOnce(::testing::Return(Fake<jclass>()));
  EXPECT_CALL(*env_, GetMethodID(AsGlobal(Fake<jclass>()), StrEq("build"),
                                 StrEq("()LBuilder;")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("build");
}

TEST_F(JniTest, SelfCanBeUsedAsAReturnValueAndMaintainsRichDecoration) {
  EXPECT_CALL(*env_, GetMethodID).Times(AnyNumber());
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("build"), StrEq("()LBuilder;")));

  LocalObject<kClass> obj{Fake<jobject>()};
  obj("build")("build")("build");
}

TEST_F(JniTest, SelfCanBeUsedAsParam) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("takesBuilder"), StrEq("(LBuilder;)V")));

  LocalObject<kClass> obj_1{Fake<jobject>(1)};
  LocalObject<kClass> obj_2{Fake<jobject>(2)};

  obj_1("takesBuilder", obj_2);
}

}  // namespace
