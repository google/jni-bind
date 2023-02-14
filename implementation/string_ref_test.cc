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
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptGlobal;
using ::jni::GlobalString;
using ::jni::kJavaLangString;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::UtfStringView;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

const char* char_ptr = "TestString";
jstring FakeJString() { return reinterpret_cast<jstring>(0xFAFAFAFAFA); }

static constexpr jni::Class kClass{
    "Class", jni::Method{"Foo", jni::Return<jstring>{}, jni::Params{}}};

////////////////////////////////////////////////////////////////////////////////
// Local String Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, LocalString_NullPtrT) { LocalString str{nullptr}; }

TEST_F(JniTest, LocalString_NullWorks) {
  EXPECT_CALL(*env_, DeleteLocalRef).Times(0);
  LocalString str{jstring{nullptr}};
}

TEST_F(JniTest, LocalString_ConstructsFromObject) {
  EXPECT_CALL(*env_, DeleteLocalRef).Times(1);
  LocalObject<kJavaLangString> undecorated_object{
      reinterpret_cast<jobject>(0xabcde)};
  LocalString decorated_object{std::move(undecorated_object)};
}

TEST_F(JniTest, LocalString_ConstructsFromOutputOfMethod) {
  LocalObject<kClass> obj{};
  LocalString str{obj("Foo")};
}

TEST_F(JniTest, LocalString_ConstructsFromByteArray) {
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), StrEq("([B)V")));
  LocalString str{reinterpret_cast<jbyteArray>(0xabcde)};
}

TEST_F(JniTest, LocalString_CreatesFromCharPtr) {
  LocalString str{"TestString"};
}

TEST_F(JniTest, LocalString_CreatesFromStringView) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")))
      .WillOnce(Return(FakeJString()));
  EXPECT_CALL(*env_, DeleteLocalRef);
  LocalString str{std::string_view{char_ptr}};
}

TEST_F(JniTest, LocalString_CreatesFromString) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")))
      .WillOnce(Return(FakeJString()));
  EXPECT_CALL(*env_, DeleteLocalRef);
  LocalString str{std::string{"TestString"}};
}

TEST_F(JniTest, LocalString_CreatesFromCharPtrForGlobals) {
  GlobalString str{"TestString"};
}

TEST_F(JniTest, LocalString_PinsAndUnpinsMemoryForLocals) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("<init>"), StrEq("(Ljava/lang/String;)V")));
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestLocalString")));
  EXPECT_CALL(*env_, GetStringUTFChars(_, nullptr)).WillOnce(Return(char_ptr));
  EXPECT_CALL(*env_, ReleaseStringUTFChars(_, char_ptr));

  LocalString str{"TestLocalString"};
  UtfStringView utf_string_view = str.Pin();
  EXPECT_EQ(utf_string_view.ToString().data(), char_ptr);
}

////////////////////////////////////////////////////////////////////////////////
// Global String Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, GlobalString_NullPtrT) { GlobalString str{nullptr}; }

TEST_F(JniTest, GlobalString_NullWorks) { GlobalString str{jstring{nullptr}}; }

TEST_F(JniTest, GlobalString_ConstructsFromObject) {
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(1);
  jni::GlobalObject<kJavaLangString> undecorated_object{
      AdoptGlobal{}, reinterpret_cast<jobject>(0xabcde)};
  GlobalString decorated_object{std::move(undecorated_object)};
}

TEST_F(JniTest, GlobalString_GlobalsReleaseWithGlobalMechanism) {
  EXPECT_CALL(*env_, DeleteGlobalRef);

  GlobalString str{FakeJString()};
}

TEST_F(JniTest, GlobalString_ConstructsFromOutputOfMethod) {
  LocalObject<kClass> obj{};
  GlobalString str{obj("Foo")};
}

TEST_F(JniTest, GlobalString_ConstructsFromByteArray) {
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), StrEq("([B)V")));
  GlobalString str{reinterpret_cast<jbyteArray>(0xabcde)};
}

TEST_F(JniTest, GlobalString_CreatesFromCharPtr) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("<init>"), StrEq("(Ljava/lang/String;)V")));
  GlobalString str{"TestString"};
}

TEST_F(JniTest, GlobalString_CreatesFromStringView) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")))
      .WillOnce(Return(FakeJString()));
  GlobalString str{std::string_view{char_ptr}};
}

TEST_F(JniTest, GlobalString_CreatesFromString) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")))
      .WillOnce(Return(FakeJString()));
  GlobalString str{std::string{"TestString"}};
}

TEST_F(JniTest, GlobalString_CreatesFromCharPtrForGlobals) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("<init>"), StrEq("(Ljava/lang/String;)V")));
  GlobalString str{"TestString"};
}

TEST_F(JniTest, GlobalString_PinsAndUnpinsMemoryForLocals) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("<init>"), StrEq("(Ljava/lang/String;)V")));
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestGlobalString")));
  EXPECT_CALL(*env_, GetStringUTFChars(_, nullptr)).WillOnce(Return(char_ptr));
  EXPECT_CALL(*env_, ReleaseStringUTFChars(_, char_ptr));

  GlobalString str{"TestGlobalString"};
  UtfStringView utf_string_view = str.Pin();
  EXPECT_EQ(utf_string_view.ToString().data(), char_ptr);
}

}  // namespace
