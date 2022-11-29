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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/jni_helper.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using jni::Class;
using jni::LocalObject;
using jni::Method;
using jni::Overload;
using jni::Params;
using jni::test::JniTest;
using testing::_;
using testing::StrEq;

TEST_F(JniTest, MethodRef_AsksForCorrectMethods1) {
  static constexpr Class kClass{
      "com/google/SupportsStrings",
      Method{"Foo", jni::Return<void>{}, Params<jstring>{}},
  };
  LocalObject<kClass> obj{};

  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Foo"), StrEq("(Ljava/lang/String;)V")));

  obj("Foo", "test");
}

TEST_F(JniTest, MethodRef_AsksForCorrectMethods2) {
  static constexpr Class kClass{
      "com/google/SupportsStrings",
      Method{"Foo", Overload{jni::Return<void>{}, Params{jint{}}},
             Overload{jni::Return<void>{}, Params{jstring{}}}}};
  LocalObject<kClass> obj{};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("(I)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Foo"), StrEq("(Ljava/lang/String;)V")));

  obj("Foo", 1);
  // obj("Foo", 2.f); doesn't compile (good).
  obj("Foo", "test");
}

TEST_F(JniTest, MethodRef_AsksForCorrectMethods3) {
  static constexpr Class kClass{
      "com/google/SupportsStrings",
      Method{
          "Foo",
          Overload{jni::Return<void>{}, Params{jint{}}},
          Overload{jni::Return<void>{}, Params{jstring{}}},
          Overload{jni::Return<void>{}, Params{jstring{}, jstring{}}},
      }};
  LocalObject<kClass> obj{};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("(I)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Foo"), StrEq("(Ljava/lang/String;)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Foo"),
                          StrEq("(Ljava/lang/String;Ljava/lang/String;)V")));

  obj("Foo", 1);
  // obj("Foo", 2.f); // doesn't compile (good).
  obj("Foo", "test");
  obj("Foo", "test1", "test2");
  obj("Foo", "this_doesnt", "trigger_method_lookup");
}

TEST_F(JniTest, MethodRef_AsksForCorrectMethodsWhenMultiplePresent4) {
  static constexpr Class kClass{
      "com/google/SupportsStrings",
      Method{
          "Foo",
          Overload{jni::Return<void>{}, Params{jint{}}},
          Overload{jni::Return<void>{}, Params{jstring{}}},
          Overload{jni::Return<void>{}, Params{jstring{}, jstring{}}},
      },
      Method{
          "Baz",
          Overload{jni::Return<void>{}, Params{jint{}}},
          Overload{jni::Return<void>{}, Params{jstring{}, jstring{}}},
          Overload{jni::Return<void>{}, Params{jfloat{}, jfloat{}}},
      }};
  LocalObject<kClass> obj{};

  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("(I)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Foo"), StrEq("(Ljava/lang/String;)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Foo"),
                          StrEq("(Ljava/lang/String;Ljava/lang/String;)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("(I)V")));
  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Baz"),
                          StrEq("(Ljava/lang/String;Ljava/lang/String;)V")));
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Baz"), StrEq("(FF)V")));

  obj("Foo", 1);
  // obj("Foo", 2.f); // doesn't compile (good).
  obj("Foo", "test");
  obj("Foo", "test1", "test2");
  obj("Foo", "this_doesnt", "trigger_method_lookup");
  obj("Baz", 1);
  obj("Baz", "test3", "test4");
  obj("Baz", 1234.f, 5678.f);
}

}  // namespace
