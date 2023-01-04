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
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptGlobal;
using ::jni::Array;
using ::jni::Class;
using ::jni::GlobalObject;
using ::jni::Id;
using ::jni::IdType;
using ::jni::JniT;
using ::jni::kDefaultClassLoader;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::OverloadRef;
using ::jni::Params;
using ::jni::Return;
using ::jni::Rank;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::StrEq;

template <const auto& class_loader_v, const auto& class_v, size_t I>
using MethodRefT_t = OverloadRef<
    Id<JniT<jobject, class_v, class_loader_v>, IdType::OVERLOAD, I, 0>,
    IdType::OVERLOAD_PARAM>;

TEST_F(JniTest, MethodRef_DoesntStaticCrossTalkWithTagUse) {
  static constexpr Method m{"FooV", Return<void>{}, Params{jint{}}};
  static constexpr Class kSomeClass{"someClass", m};

  const jclass clazz{reinterpret_cast<jclass>(0XAAAAA)};
  const jobject object{reinterpret_cast<jobject>(0XBBBBBB)};

  MethodRefT_t<kDefaultClassLoader, kSomeClass, 0>::Invoke(clazz, object, 123);
}

TEST_F(JniTest, MethodRef_CallsGetMethodCorrectlyForSingleMethod) {
  static constexpr Method m1{"FooV", Return<void>{}, Params<>{}};
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

TEST_F(JniTest, MethodRef_ReturnWithObject) {
  static constexpr Class c2{"someClass2"};
  static constexpr Method m1{"FooV", Return{c2}, Params<>{}};
  static constexpr Class c{"someClass", m1};

  InSequence seq;
  const jclass clazz{reinterpret_cast<jclass>(0XABABA)};
  const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};

  const jmethodID fake_jmethod_1{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_,
              GetMethodID(Eq(clazz), StrEq("FooV"), StrEq("()LsomeClass2;")))
      .WillOnce(testing::Return(fake_jmethod_1));
  EXPECT_CALL(*env_, CallObjectMethodV(object, fake_jmethod_1, _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(clazz, object);
}

TEST_F(JniTest, MethodRef_ReturnWithRank1Object) {
  static constexpr Class c2{"someClass2"};
  static constexpr Method m1{"FooV", Return{Array{c2}}, Params<>{}};
  static constexpr Class c{"someClass", m1};

  InSequence seq;
  const jclass clazz{reinterpret_cast<jclass>(0XABABA)};
  const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};

  const jmethodID fake_jmethod_1{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_,
              GetMethodID(Eq(clazz), StrEq("FooV"), StrEq("()[LsomeClass2;")))
      .WillOnce(testing::Return(fake_jmethod_1));
  EXPECT_CALL(*env_, CallObjectMethodV(object, fake_jmethod_1, _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(clazz, object);
}

TEST_F(JniTest, MethodRef_ReturnWithRank2Object) {
  static constexpr Class c2{"someClass2"};
  static constexpr Method m1{"FooV", Return{Array{c2, Rank<2>{}}},
                             Params<>{}};
  static constexpr Class c{"someClass", m1};

  InSequence seq;
  const jclass clazz{reinterpret_cast<jclass>(0XABABA)};
  const jobject object{reinterpret_cast<jobject>(0XAAAAAA)};

  const jmethodID fake_jmethod_1{reinterpret_cast<jmethodID>(0XBBBBBB)};
  EXPECT_CALL(*env_,
              GetMethodID(Eq(clazz), StrEq("FooV"), StrEq("()[[LsomeClass2;")))
      .WillOnce(testing::Return(fake_jmethod_1));
  EXPECT_CALL(*env_, CallObjectMethodV(object, fake_jmethod_1, _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(clazz, object);
}

TEST_F(JniTest, MethodRef_ReturnWithNoParams) {
  static constexpr Method m1{"FooV", Return<void>{}, Params<>{}};
  static constexpr Method m2{"BarI", Return<jint>{}, Params<>{}};
  static constexpr Method m3{"BazF", Return<jfloat>{}, Params<>{}};
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
  constexpr Method m1{"SomeFunc1", Return<void>{}, Params<jint>{}};
  constexpr Method m2{"SomeFunc2", Return<jint>{}, Params<jfloat>{}};
  constexpr Method m3{"SomeFunc3", Return<jfloat>{}, Params<jfloat>{}};
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
      Method{"Foo", Return{c1}, Params<jint>{}},
  };

  const jobject local_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  // Note, class refs are not released, so Times() != 2.
  EXPECT_CALL(*env_, NewObjectV).WillOnce(testing::Return(local_jobject));

  GlobalObject<kClass> global_object{};
  LocalObject<c1> new_obj{global_object("Foo", 5)};
}

TEST_F(JniTest, MethodRef_PassesObjects) {
  static constexpr Class c1{"com/google/Bazz"};
  static constexpr Class kClass{
      "com/google/PassesObjects",
      Method{"Foo", Return<jint>{}, Params{c1}},
  };

  const jobject local_instance{reinterpret_cast<jobject>(0XAAAAAA)};
  const jobject global_c1_instance{reinterpret_cast<jobject>(0XBBBBBB)};

  LocalObject<c1> local_object{local_instance};
  GlobalObject<kClass> global_object{AdoptGlobal{}, global_c1_instance};

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
      Method{"Foo", Return{c1}, Params{c1, c2, c3, c4}},
  };

  const jobject fake_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  LocalObject<c1> obj1{fake_jobject};
  LocalObject<c2> obj2{fake_jobject};
  LocalObject<c3> obj3{fake_jobject};
  LocalObject<c4> obj4{fake_jobject};
  LocalObject<class_under_test> object_under_test{fake_jobject};

  LocalObject<c1> obj5{object_under_test("Foo", obj1, obj2, obj3, obj4)};
}

TEST_F(JniTest, MethodRef_SupportsForwardDefines) {
  static constexpr Class kClass1{
      "kClass1",
      Method{"m1", Return<void>{}, Params{Class{"kClass1"}}},
      Method{"m2", Return<void>{}, Params{Class{"kClass2"}}},
      Method{"m3", Return{Class{"kClass1"}}, Params{}},
      Method{"m4", Return{Class{"kClass2"}}, Params{}},
  };

  static constexpr Class kClass2{
      "kClass2",
      Method{"m1", Return<void>{}, Params{Class{"kClass1"}}},
      Method{"m2", Return<void>{}, Params{Class{"kClass2"}}},
      Method{"m3", Return{Class{"kClass1"}}, Params{}},
      Method{"m4", Return{Class{"kClass2"}}, Params{}},
  };

  const jobject fake_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  LocalObject<kClass1> c1_obj1{fake_jobject};
  LocalObject<kClass1> c1_obj2{fake_jobject};

  LocalObject<kClass2> c2_obj1{fake_jobject};
  LocalObject<kClass2> c2_obj2{fake_jobject};

  c1_obj1("m1", c1_obj1);
  c1_obj1("m2", c2_obj1);
  c1_obj1("m1", c1_obj1("m3"));
  c1_obj1("m2", c1_obj1("m4"));

  c2_obj1("m1", c1_obj1);
  c2_obj1("m2", c2_obj2);
  c2_obj1("m2", std::move(std::move(c2_obj2)));

  c1_obj1("m2", std::move(c2_obj1));

  // c2_obj1("m1", c1_obj1); // illegal! triggers warnings (post move read).
  // c2_obj1("m2", c2_obj2); // illegal! triggers warnings (post move read).
  // c2_obj1("m2", std::move(c2_obj2));  // illegal! triggers warnings (post
  // move read).
}

TEST_F(JniTest, MethodRef_SupportsStrings) {
  static constexpr Class class_under_test{
      "com/google/SupportsStrings",
      Method{"Foo", Return<void>{}, Params<jstring>{}},
      Method{"Bar", Return<void>{}, Params<jstring, jstring>{}},
      Method{"Baz", Return<jstring>{}, Params<>{}},
  };

  const jobject fake_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  LocalObject<class_under_test> obj1{fake_jobject};
  obj1("Foo", "This is a method.");
  obj1("Bar", "This is a method.", "It takes strings");
  obj1("Baz");
}

TEST_F(JniTest, MethodRef_SupportsArrays) {
  static constexpr Class kClass{"kClass"};
  static constexpr Class class_under_test{
      "com/google/SupportsArrays",
      Method{"Foo", Return<void>{}, Params{Array{kClass}}},
      Method{"Bar", Return<void>{}, Params<int>{}}};

  const jobject fake_jobject{reinterpret_cast<jobject>(0XAAAAAA)};

  LocalArray<jobject, 1, kClass> local_array{nullptr};
  LocalObject<class_under_test> obj1{fake_jobject};
  obj1("Foo", local_array);
}

}  // namespace
