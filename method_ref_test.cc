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

#include "method_ref.h"

#include "class.h"
#include "global_object.h"
#include "jni_test.h"
#include "local_object.h"
#include "local_string.h"
#include "method.h"
#include "mock_jni_env.h"
#include "params.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_dep.h"
#include "jni_helper/jni_helper.h"
#include "return.h"

namespace {

using jni::Class;
using jni::Invocation;
using jni::kDefaultClassLoader;
using jni::Method;
using jni::MethodRefT_t;
using jni::Params;
using jni::test::JniTest;
using testing::_;
using testing::Eq;
using testing::InSequence;
using testing::StrEq;

TEST_F(JniTest, MethodRef_DoesntStaticCrossTalkWithTagUse) {
  static constexpr Method m{"FooV", jni::Return<void>{}, Params{jint{}}};
  static constexpr Class kSomeClass{"someClass", m};

  const jclass clazz{reinterpret_cast<jclass>(0XAAAAA)};
  const jobject object{reinterpret_cast<jobject>(0XBBBBBB)};

  MethodRefT_t<kDefaultClassLoader, kSomeClass, 0>::Invoke(clazz, object, 123);
}

TEST_F(JniTest, MethodRef_CallsGetMethodCorrectlyForSingleMethod) {
  static constexpr Method m1{"FooV", jni::Return<void>{}, Params<>{}};
  static constexpr Class c{"SimpleClass", m1};

  InSequence seq;
  const jclass clazz{reinterpret_cast<jclass>(0XABABA)};
  const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};

  const jmethodID fake_jmethod_1{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, GetMethodID(Eq(clazz), StrEq("FooV"), StrEq("()V")))
      .WillOnce(testing::Return(fake_jmethod_1));
  EXPECT_CALL(*env_, CallVoidMethodV(object, fake_jmethod_1, _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(clazz, object);
}

TEST_F(JniTest, MethodRef_ReturnWithNoParams) {
  static constexpr Method m1{"FooV", jni::Return<void>{}, Params<>{}};
  static constexpr Method m2{"BarI", jni::Return<jint>{}, Params<>{}};
  static constexpr Method m3{"BazF", jni::Return<jfloat>{}, Params<>{}};
  static constexpr Class c{"someClass", m1, m2, m3};

  InSequence seq;
  const jclass clazz{reinterpret_cast<jclass>(0XABABA)};
  const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};

  const jmethodID fake_jmethod_1{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_, GetMethodID(Eq(clazz), StrEq("FooV"), StrEq("()V")))
      .WillOnce(testing::Return(fake_jmethod_1));
  EXPECT_CALL(*env_, CallVoidMethodV(object, fake_jmethod_1, _));

  const jmethodID fake_jmethod_2{reinterpret_cast<jmethodID>(0XCCCCCC)};
  EXPECT_CALL(*env_, GetMethodID(Eq(clazz), StrEq("BarI"), StrEq("()I")))
      .WillOnce(testing::Return(fake_jmethod_2));
  EXPECT_CALL(*env_, CallIntMethodV(object, fake_jmethod_2, _));

  const jmethodID fake_jmethod_3{reinterpret_cast<jmethodID>(0XDDDDDD)};
  EXPECT_CALL(*env_, GetMethodID(Eq(clazz), StrEq("BazF"), StrEq("()F")))
      .WillOnce(testing::Return(fake_jmethod_3));
  EXPECT_CALL(*env_, CallFloatMethodV(object, fake_jmethod_3, _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(clazz, object);
  MethodRefT_t<kDefaultClassLoader, c, 1>::Invoke(clazz, object);
  MethodRefT_t<kDefaultClassLoader, c, 2>::Invoke(clazz, object);
}

TEST_F(JniTest, MethodRef_SingleParam) {
  constexpr Method m1{"SomeFunc1", jni::Return<void>{}, Params<jint>{}};
  constexpr Method m2{"SomeFunc2", jni::Return<jint>{}, Params<jfloat>{}};
  constexpr Method m3{"SomeFunc3", jni::Return<jfloat>{}, Params<jfloat>{}};
  static constexpr Class c{"someClass", m1, m2, m3};

  InSequence seq;
  const jclass clazz{reinterpret_cast<jclass>(0XAAAAAA)};
  const jobject object{reinterpret_cast<jobject>(0XBBBBBB)};
  const jmethodID fake_jmethod_1{reinterpret_cast<jmethodID>(0XCCCCCC)};

  EXPECT_CALL(*env_, GetMethodID(Eq(clazz), StrEq("SomeFunc1"), StrEq("(I)V")))
      .WillOnce(testing::Return(fake_jmethod_1));
  // There is no clear way to test variable vaargs type arguments using Gmock,
  // but at least we can test the correct method is called.
  EXPECT_CALL(*env_, CallVoidMethodV(object, fake_jmethod_1, _));

  const jmethodID fake_jmethod_2{reinterpret_cast<jmethodID>(0XDDDDDD)};
  EXPECT_CALL(*env_, GetMethodID(Eq(clazz), StrEq("SomeFunc2"), StrEq("(F)I")))
      .WillOnce(testing::Return(fake_jmethod_2));
  EXPECT_CALL(*env_, CallIntMethodV(object, fake_jmethod_2, _));

  const jmethodID fake_jmethod_3{reinterpret_cast<jmethodID>(0XEEEEEE)};
  EXPECT_CALL(*env_, GetMethodID(Eq(clazz), StrEq("SomeFunc3"), StrEq("(F)F")))
      .WillOnce(testing::Return(fake_jmethod_3));
  EXPECT_CALL(*env_, CallFloatMethodV(object, fake_jmethod_3, _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(clazz, object, 1);
  MethodRefT_t<kDefaultClassLoader, c, 1>::Invoke(clazz, object, 1.234f);
  MethodRefT_t<kDefaultClassLoader, c, 2>::Invoke(clazz, object, 5.6789f);
}

TEST_F(JniTest, MethodRef_ReturnsObjects) {
  static constexpr Class c1{"Bazz"};
  static constexpr Class kClass{
      "com/google/ReturnsObjects",
      Method{"Foo", jni::Return{c1}, Params<jint>{}},
  };

  const jobject local_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  // Note, class refs are not released, so Times() != 2.
  EXPECT_CALL(*env_, NewObjectV).WillOnce(testing::Return(local_jobject));

  jni::GlobalObject<kClass> global_object{};
  jni::LocalObject<kClass> new_obj{global_object("Foo", 5)};
}

TEST_F(JniTest, MethodRef_PassesObjects) {
  static constexpr Class c1{"com/google/Bazz"};
  static constexpr Class kClass{
      "com/google/PassesObjects",
      Method{"Foo", jni::Return<jint>{}, Params{c1}},
  };

  const jobject local_instance{reinterpret_cast<jobject>(0XAAAAAA)};
  const jobject global_c1_instance{reinterpret_cast<jobject>(0XBBBBBB)};

  jni::LocalObject<c1> local_object{local_instance};
  jni::GlobalObject<kClass> global_object{global_c1_instance};

  EXPECT_CALL(*env_,
              GetMethodID(_, StrEq("Foo"), StrEq("(Lcom/google/Bazz;)I")));

  global_object("Foo", local_object);
}

TEST_F(JniTest, MethodRef_PassesAndReturnsMultipleObjects) {
  static constexpr Class c1{"Class1"};
  static constexpr Class c2{"Class2"};
  static constexpr Class c3{"Class3"};
  static constexpr Class c4{"Class4"};

  static constexpr Class class_under_test{
      "com/google/PassesAndReturnsMultipleObjects",
      Method{"Foo", jni::Return{c1}, Params{c1, c2, c3, c4}},
  };

  const jobject fake_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  jni::LocalObject<c1> obj1{fake_jobject};
  jni::LocalObject<c2> obj2{fake_jobject};
  jni::LocalObject<c3> obj3{fake_jobject};
  jni::LocalObject<c4> obj4{fake_jobject};
  jni::LocalObject<class_under_test> object_under_test{fake_jobject};

  jni::LocalObject<c1> obj5{object_under_test("Foo", obj1, obj2, obj3, obj4)};
}

TEST_F(JniTest, MethodRef_SupportsStrings) {
  static constexpr Class class_under_test{
      "com/google/SupportsStrings",
      Method{"Foo", jni::Return<void>{}, Params<jstring>{}},
      Method{"Bar", jni::Return<void>{}, Params<jstring, jstring>{}},
      Method{"Baz", jni::Return<jstring>{}, Params<>{}},
  };

  const jobject fake_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  jni::LocalObject<class_under_test> obj1{fake_jobject};
  obj1("Foo", "This is a method.");
  obj1("Bar", "This is a method.", "It takes strings");
  obj1("Baz");
}

// TODO(b/175083373):  Overloads currently default to the 0th method for regular
// functions.  This simply tests that the syntax is not breaking compilation.
TEST_F(JniTest, MethodRef_OverloadsCompile1) {
  static constexpr Method m{
      "FooV",
      Invocation{jni::Return<void>{}, Params{jint{}}},
  };

  static constexpr Class kSomeClass{"someClass", m};
  const jclass clazz{reinterpret_cast<jclass>(0XAAAAA)};
  const jobject object{reinterpret_cast<jobject>(0XBBBBBB)};

  MethodRefT_t<kDefaultClassLoader, kSomeClass, 0>::Invoke(clazz, object, 123);
}

TEST_F(JniTest, MethodRef_OverloadsCompile2) {
  static constexpr Method m{"FooV",
                            Invocation{jni::Return<void>{}, Params{jint{}}},
                            Invocation{jni::Return<void>{}, Params{jfloat{}}}};

  static constexpr Class kSomeClass{"someClass", m};
  const jclass clazz{reinterpret_cast<jclass>(0XAAAAA)};
  const jobject object{reinterpret_cast<jobject>(0XBBBBBB)};

  MethodRefT_t<kDefaultClassLoader, kSomeClass, 0>::Invoke(clazz, object, 123);
}

TEST_F(JniTest, MethodRef_OverloadsCompile3) {
  static constexpr Method m{
      "FooV", Invocation{jni::Return<void>{}, Params{jint{}}},
      Invocation{jni::Return<void>{}, Params{jfloat{}}},
      Invocation{jni::Return<void>{}, Params{jint{}, jfloat{}}}};

  static constexpr Class kSomeClass{"someClass", m};
  const jclass clazz{reinterpret_cast<jclass>(0XAAAAA)};
  const jobject object{reinterpret_cast<jobject>(0XBBBBBB)};

  MethodRefT_t<kDefaultClassLoader, kSomeClass, 0>::Invoke(clazz, object, 123);
}

}  // namespace
