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

#include <string>
#include <string_view>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptGlobal;
using ::jni::Fake;
using ::jni::GlobalObject;
using ::jni::GlobalString;
using ::jni::kJavaLangString;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::NewRef;
using ::jni::UtfStringView;
using ::jni::test::AsNewLocalReference;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

const char* char_ptr = "TestString";

static constexpr jni::Class kClass{
    "Class",
    jni::Method{"Foo", jni::Return<jstring>{}, jni::Params{}},
    jni::Method{"TakesStrParam", jni::Return<void>{}, jni::Params<jstring>{}},
};

  /*
////////////////////////////////////////////////////////////////////////////////
// Local String Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, LocalString_NullPtrT) { LocalString str{nullptr}; }

TEST_F(JniTest, LocalString_IsImplicitlyConvertible) {
  LocalString str{Fake<jstring>()};
  EXPECT_EQ(static_cast<jstring>(str), AsNewLocalReference(Fake<jstring>()));
}

TEST_F(JniTest, LocalString_NullWorks) {
  EXPECT_CALL(*env_, DeleteLocalRef).Times(0);
  LocalString str{jstring{nullptr}};
}

TEST_F(JniTest, LocalString_ConstructsFromObject) {
  EXPECT_CALL(*env_, DeleteLocalRef(AsNewLocalReference(Fake<jobject>())));
  LocalObject<kJavaLangString> undecorated_object{Fake<jobject>()};
  LocalString decorated_object{std::move(undecorated_object)};
}

TEST_F(JniTest, LocalString_CopiesFromObject) {
  EXPECT_CALL(*env_, DeleteLocalRef(AsNewLocalReference(Fake<jobject>())));
  EXPECT_CALL(*env_, NewLocalRef(Fake<jobject>()));

  LocalString decorated_object{NewRef{}, Fake<jobject>()};

  EXPECT_EQ(jstring{decorated_object}, AsNewLocalReference(Fake<jobject>()));
}

TEST_F(JniTest, LocalString_CopiesFromJString) {
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jstring>(2)));
  EXPECT_CALL(*env_, NewLocalRef(Fake<jstring>(1)))
      .WillOnce(::testing::Return(Fake<jstring>(2)));

  LocalString decorated_object{NewRef{}, Fake<jstring>(1)};

  EXPECT_EQ(jstring{decorated_object}, Fake<jstring>(2));
}

TEST_F(JniTest, LocalString_ConstructsFromOutputOfMethod) {
  LocalObject<kClass> obj{};
  LocalString str{obj.Call<"Foo">()};
}

TEST_F(JniTest, LocalString_ConstructsFromByteArray) {
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), StrEq("([B)V")));
  LocalString str{Fake<jbyteArray>()};
}

TEST_F(JniTest, LocalString_CreatesFromCharPtr) {
  LocalString str{"TestString"};
}

TEST_F(JniTest, LocalString_CreatesFromStringView) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")))
      .WillOnce(Return(Fake<jstring>()));

  // jclass for temp String class reference.
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>()));
  // The variable str (which is itself an object).
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>()));
  // TODO(b/143908983): Currently strings leak one local during proxying.
  // Temporary xref created during construction.
  // EXPECT_CALL(*env_, DeleteLocalRef(Fake<jstring>()));

  LocalString str{std::string_view{char_ptr}};
}

TEST_F(JniTest, LocalString_CreatesFromString) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")))
      .WillOnce(Return(Fake<jstring>()));

  // jclass for temp String class reference.
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>()));
  // The variable str (which is itself an object).
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobject>()));
  // TODO(b/143908983): Currently strings leak one local during proxying.
  // Temporary xref created during construction.
  // EXPECT_CALL(*env_, DeleteLocalRef(Fake<jstring>()));
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

TEST_F(JniTest, LocalString_AllowsLValueLocalString) {
  LocalObject<kClass> obj{};
  LocalString local_string{"abcde"};
  obj.Call<"TakesStrParam">(local_string);
}

TEST_F(JniTest, LocalString_AllowsRValueLocalString) {
  LocalObject<kClass> obj{};
  obj.Call<"TakesStrParam">(LocalString{"abcde"});
}

////////////////////////////////////////////////////////////////////////////////
// Global String Tests.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, GlobalString_NullWorks) {
  GlobalString str{AdoptGlobal{}, jstring{nullptr}};
}

TEST_F(JniTest, GlobalString_ConstructsFromObject) {
  EXPECT_CALL(*env_, DeleteGlobalRef).Times(1);
  GlobalObject<kJavaLangString> undecorated_object{AdoptGlobal{},
                                                   Fake<jobject>()};
  GlobalString decorated_object{std::move(undecorated_object)};
}

TEST_F(JniTest, GlobalString_GlobalsReleaseWithGlobalMechanism) {
  EXPECT_CALL(*env_, DeleteGlobalRef);
  GlobalString str{AdoptGlobal{}, Fake<jstring>()};
}

TEST_F(JniTest, GlobalString_ConstructsFromOutputOfMethod) {
  LocalObject<kClass> obj{};
  GlobalString str{obj.Call<"Foo">()};
}

TEST_F(JniTest, GlobalString_ConstructsFromByteArray) {
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("<init>"), StrEq("([B)V")));
  GlobalString str{Fake<jbyteArray>()};
}

TEST_F(JniTest, GlobalString_CreatesFromCharPtr) {
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("<init>"), StrEq("(Ljava/lang/String;)V")));
  GlobalString str{"TestString"};
}

TEST_F(JniTest, GlobalString_CreatesFromStringView) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")))
      .WillOnce(Return(Fake<jstring>()));
  GlobalString str{std::string_view{char_ptr}};
}

TEST_F(JniTest, GlobalString_CreatesFromString) {
  EXPECT_CALL(*env_, NewStringUTF(StrEq("TestString")))
      .WillOnce(Return(Fake<jstring>()));
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

TEST_F(JniTest, GlobalString_AllowsLValueGlobalString) {
  LocalObject<kClass> obj{};
  GlobalString global_string{"abcde"};
  obj.Call<"TakesStrParam">(global_string);
}

TEST_F(JniTest, GlobalString_AllowsRValueGlobalString) {
  LocalObject<kClass> obj{};
  obj.Call<"TakesStrParam">(GlobalString{"abcde"});
}
*/

TEST_F(JniTest, AllocationThrash)
{
  int i = 0;
  //while (true)
  for (int i = 0; i < 1; i++)
  {
    //for (int j = 0; j < 10000; j++)
    for (int j = 0; j < 1; j++)
    {
      /*
      LocalString str = "akdfajslkfaskflj"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs"
      "alkdsjklafjkldsajklfasdjldasfjklasdfkjdlasfkjdsafljafs";
      */
      LocalString str { "akdfajslkfaskflj" };
    }

    i++;
    if (i % 100 == 0)
    {
      std::cout << "Iteration: " << i << std::endl;
      // I even tried to manually call gc, but it has no effect in this case.
    }
  }
}

}  // namespace
