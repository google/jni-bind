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

#include <memory>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

using ::jni::Array;
using ::jni::ArrayStrip_t;
using ::jni::CDecl_t;
using ::jni::Class;
using ::jni::Fake;
using ::jni::Field;
using ::jni::kJavaLangString;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::Method;
using ::jni::Params;
using ::jni::Rank;
using ::jni::RegularToArrayTypeMap_t;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

namespace {

static constexpr Class kClass{"kClass"};

////////////////////////////////////////////////////////////////////////////////
// Strings are unusual in that they have their own type (jstring) but are
// almost completely objects otherwise.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Construction / Materialisation.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_ConstructsFromAnotherStringArray) {
  LocalArray<jstring> arr_1{Fake<jobjectArray>()};
  LocalArray<jstring> arr_2{std::move(arr_1)};
}

TEST_F(JniTest, Array_CorrectSignatureForStringParams) {
  EXPECT_CALL(*env_, FindClass(StrEq("ClassThatReturnsArrays")));
  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/String")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("StringArray"),
                                 StrEq("([Ljava/lang/String;)V")));

  static constexpr Class kClass{
      "ClassThatReturnsArrays",
      Method{"StringArray", jni::Return{}, jni::Params{Array{jstring{}}}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  LocalArray<jstring> arr{3};
  obj("StringArray", arr);
}

////////////////////////////////////////////////////////////////////////////////
// Caches Length.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_CachesLengthExactlyOnceOnFirstRequest) {
  EXPECT_CALL(*env_, GetArrayLength).Times(0);

  LocalArray<jint> obj{Fake<jintArray>()};

  EXPECT_CALL(*env_, GetArrayLength).Times(1).WillOnce(Return(5));
  EXPECT_EQ(obj.Length(), 5);
  EXPECT_EQ(obj.Length(), 5);
  EXPECT_EQ(obj.Length(), 5);
}

////////////////////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_LocalStringRValuesCanBeSet) {
  EXPECT_CALL(*env_, DeleteLocalRef(_))
      .Times(1);  // array (object is moved from)
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>()));  // FindClass
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jstring>())).Times(0);

  LocalArray<jstring> arr{3};
  arr.Set(0, LocalString{Fake<jstring>()});
}

TEST_F(JniTest, Array_LocalVanillaObjectRValuesCanBeSet) {
  // Unfortunately this is getting cached separately by `LocalArray`.
  // In the future, this should drop to 1.
  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/String"))).Times(2);

  EXPECT_CALL(*env_, DeleteLocalRef(_)).Times(2);  // array, in place obj
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>())).Times(2);  // FindClass
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jstring>())).Times(0);

  LocalArray<jobject, 1, kJavaLangString> arr{
      3, LocalObject<kJavaLangString>{"Foo"}};
  arr.Set(0, LocalObject<kJavaLangString>{Fake<jstring>()});
}

////////////////////////////////////////////////////////////////////////////////
// As Return.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_CorrectReturnSignatureForStrings) {
  static constexpr Class kClass{
      "kClass",
      Method{"StringArray", jni::Return{Array{jstring{}}}},
  };

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("StringArray"),
                                 StrEq("()[Ljava/lang/String;")));
  LocalArray<jstring> arr = obj("StringArray");
}

////////////////////////////////////////////////////////////////////////////////
// As Param.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_CorrectParamSignatureForStrings) {
  static constexpr Class kClass{
      "kClass",
      Method{"StringArray", jni::Return{}, Params{Array{jstring{}}}},
  };

  LocalObject<kClass> obj{Fake<jobject>()};
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("StringArray"),
                                 StrEq("([Ljava/lang/String;)V")));
  LocalArray<jstring> arr{2};
  obj("StringArray", arr);
}

TEST_F(JniTest, LocalStringArray_ConstructsObjectsForLValues) {
  // Unlike POD, objects are constructed with a size, a jclass, and an init
  // object.  This makes for a slightly different API then other objects.
  EXPECT_CALL(*env_, NewObjectArray);

  LocalObject<kClass> default_object{};
  LocalArray<jobject, 1, kClass> local_object_array{5, default_object};
}

/*
// TODO(b/143908983): Restore after fixing ubsan failures.
TEST_F(JniTest, Array_StringsCanBeSetOnLocalString) {
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 0, _));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 1, _));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 2, _));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 3, _));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(), 4, _));

  const char* kFoo = "kFoo";
  const std::string kNar = "kNar";

  LocalArray<jstring> arr{5, LocalObject<kJavaLangString>{"Foo"}};
  arr.Set(0, "Bar");
  arr.Set(1, std::string{"Baz"});
  arr.Set(2, std::string_view{"Bar"});
  arr.Set(3, std::string_view{kFoo});
  arr.Set(4, std::string_view{kNar});
}
*/

////////////////////////////////////////////////////////////////////////////////
// String Fields.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, Array_CorrectFieldSignatureForStrings) {
  static constexpr Class kClass{
      "kClass",
      Field{"StringArrayRank1", Array{jstring{}}},
      Field{"StringArrayRank2", Array{jstring{}, Rank<2>{}}},
      Field{"StringArrayRank3", Array{jstring{}, Rank<3>{}}},
  };

  LocalObject<kClass> obj{jobject{nullptr}};
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("StringArrayRank1"),
                                StrEq("[Ljava/lang/String;")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("StringArrayRank2"),
                                StrEq("[[Ljava/lang/String;")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("StringArrayRank3"),
                                StrEq("[[[Ljava/lang/String;")));

  LocalArray<jstring> arr1 = obj["StringArrayRank1"].Get();
  LocalArray<jstring, 2> arr2 = obj["StringArrayRank2"].Get();
  LocalArray<jstring, 3> arr3 = obj["StringArrayRank3"].Get();
}

}  // namespace
