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

#include "mock_jni_env.h"
#include <gtest/gtest.h>
#include "class.h"
#include "global_string.h"
#include "jni_dep.h"
#include "jni_test.h"
#include "local_object.h"
#include "local_string.h"
#include "method.h"
#include "params.h"

namespace {

using jni::test::JniTest;
using jni::test::MockJniEnv;
using testing::Return;
using testing::StrEq;

TEST_F(JniTest, String_NullWorks) {
  jni::LocalString local_jni_string{nullptr};
}

TEST_F(JniTest, String_ConstructsFromOutputOfMethod) {
  static constexpr jni::Class kClass{
      "Class", jni::Method{"Foo", jni::Return<jstring>{}, jni::Params{}}};

  jni::LocalObject<kClass> obj{};
  jni::LocalString local_jni_string{obj("Foo")};
}

TEST_F(JniTest, String_CreatesLocalStringFromCharPtr) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")));
  EXPECT_CALL(*env_, DeleteLocalRef);
  jni::LocalString jni_string{"TestString"};
}

TEST_F(JniTest, String_CreatesLocalStringFromCharPtrForGlobals) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")));
  EXPECT_CALL(*env_, DeleteLocalRef);
  EXPECT_CALL(*env_, DeleteGlobalRef);
  jni::GlobalString jni_string{"TestString"};
}

TEST_F(JniTest, String_PinsAndUnpinsMemoryForLocals) {
  jstring fake_jstring = reinterpret_cast<jstring>(0xAAAAAAA);
  const char* fake_chars = "Some fake characters.";
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestLocalString")))
      .WillOnce(Return(fake_jstring));
  EXPECT_CALL(*env_, GetStringUTFChars(fake_jstring, nullptr))
      .WillOnce(Return(fake_chars));
  EXPECT_CALL(*env_, ReleaseStringUTFChars(fake_jstring, fake_chars));

  jni::LocalString local_string{"TestLocalString"};
  jni::UtfStringView utf_string_view = local_string.Pin();
  EXPECT_EQ(utf_string_view.ToString().data(), fake_chars);
}

}  // namespace
