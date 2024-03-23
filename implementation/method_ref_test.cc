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
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptGlobal;
using ::jni::Array;
using ::jni::Class;
using ::jni::Fake;
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
using ::jni::Rank;
using ::jni::Return;
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

  MethodRefT_t<kDefaultClassLoader, kSomeClass, 0>::Invoke(
      Fake<jclass>(), Fake<jobject>(), 123);
}

TEST_F(JniTest, MethodRef_CallsGetMethodCorrectlyForSingleMethod) {
  static constexpr Method m1{"FooV", Return<void>{}};
  static constexpr Class c{"SimpleClass", m1};

  InSequence seq;
  EXPECT_CALL(*env_,
              GetMethodID(Eq(Fake<jclass>()), StrEq("FooV"), StrEq("()V")))
      .WillOnce(testing::Return(Fake<jmethodID>()));
  EXPECT_CALL(*env_, CallVoidMethodV(Fake<jobject>(), Fake<jmethodID>(), _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>());
}

TEST_F(JniTest, MethodRef_ReturnWithObject) {
  static constexpr Class c2{"someClass2"};
  static constexpr Method m1{"FooV", Return{c2}};
  static constexpr Class c{"someClass", m1};

  InSequence seq;
  EXPECT_CALL(*env_, GetMethodID(Eq(Fake<jclass>()), StrEq("FooV"),
                                 StrEq("()LsomeClass2;")))
      .WillOnce(testing::Return(Fake<jmethodID>()));
  EXPECT_CALL(*env_, CallObjectMethodV(Fake<jobject>(), Fake<jmethodID>(), _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>());
}

TEST_F(JniTest, MethodRef_ReturnWithRank1Object) {
  static constexpr Class c2{"someClass2"};
  static constexpr Method m1{"FooV", Return{Array{c2}}};
  static constexpr Class c{"someClass", m1};

  InSequence seq;
  EXPECT_CALL(*env_, GetMethodID(Eq(Fake<jclass>()), StrEq("FooV"),
                                 StrEq("()[LsomeClass2;")))
      .WillOnce(testing::Return(Fake<jmethodID>()));
  EXPECT_CALL(*env_, CallObjectMethodV(Fake<jobject>(), Fake<jmethodID>(), _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>());
}

TEST_F(JniTest, MethodRef_ReturnWithRank2Object) {
  static constexpr Class c2{"someClass2"};
  static constexpr Method m1{"FooV", Return{Array{c2, Rank<2>{}}},
                             Params<>{}};
  static constexpr Class c{"someClass", m1};

  InSequence seq;
  EXPECT_CALL(*env_, GetMethodID(Eq(Fake<jclass>()), StrEq("FooV"),
                                 StrEq("()[[LsomeClass2;")))
      .WillOnce(testing::Return(Fake<jmethodID>()));
  EXPECT_CALL(*env_, CallObjectMethodV(Fake<jobject>(), Fake<jmethodID>(), _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>());
}

TEST_F(JniTest, MethodRef_ReturnWithNoParams) {
  static constexpr Method m1{"FooV", Return<void>{}};
  static constexpr Method m2{"BarI", Return<jint>{}};
  static constexpr Method m3{"BazF", Return<jfloat>{}};
  static constexpr Class c{"someClass", m1, m2, m3};

  InSequence seq;
  EXPECT_CALL(*env_,
              GetMethodID(Eq(Fake<jclass>()), StrEq("FooV"), StrEq("()V")))
      .WillOnce(testing::Return(Fake<jmethodID>(1)));
  EXPECT_CALL(*env_, CallVoidMethodV(Fake<jobject>(), Fake<jmethodID>(1), _));

  EXPECT_CALL(*env_,
              GetMethodID(Eq(Fake<jclass>()), StrEq("BarI"), StrEq("()I")))
      .WillOnce(testing::Return(Fake<jmethodID>(2)));
  EXPECT_CALL(*env_, CallIntMethodV(Fake<jobject>(), Fake<jmethodID>(2), _));

  EXPECT_CALL(*env_,
              GetMethodID(Eq(Fake<jclass>()), StrEq("BazF"), StrEq("()F")))
      .WillOnce(testing::Return(Fake<jmethodID>(3)));
  EXPECT_CALL(*env_, CallFloatMethodV(Fake<jobject>(), Fake<jmethodID>(3), _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>());
  MethodRefT_t<kDefaultClassLoader, c, 1>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>());
  MethodRefT_t<kDefaultClassLoader, c, 2>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>());
}

TEST_F(JniTest, MethodRef_SingleParam) {
  constexpr Method m1{"SomeFunc1", Return<void>{}, Params<jint>{}};
  constexpr Method m2{"SomeFunc2", Return<jint>{}, Params<jfloat>{}};
  constexpr Method m3{"SomeFunc3", Return<jfloat>{}, Params<jfloat>{}};
  static constexpr Class c{"someClass", m1, m2, m3};

  InSequence seq;
  EXPECT_CALL(
      *env_, GetMethodID(Eq(Fake<jclass>()), StrEq("SomeFunc1"), StrEq("(I)V")))
      .WillOnce(testing::Return(Fake<jmethodID>(1)));
  // There is no clear way to test variable vaargs type arguments using Gmock,
  // but at least we can test the correct method is called.
  EXPECT_CALL(*env_, CallVoidMethodV(Fake<jobject>(), Fake<jmethodID>(1), _));

  EXPECT_CALL(
      *env_, GetMethodID(Eq(Fake<jclass>()), StrEq("SomeFunc2"), StrEq("(F)I")))
      .WillOnce(testing::Return(Fake<jmethodID>(2)));
  EXPECT_CALL(*env_, CallIntMethodV(Fake<jobject>(), Fake<jmethodID>(2), _));

  EXPECT_CALL(
      *env_, GetMethodID(Eq(Fake<jclass>()), StrEq("SomeFunc3"), StrEq("(F)F")))
      .WillOnce(testing::Return(Fake<jmethodID>(3)));
  EXPECT_CALL(*env_, CallFloatMethodV(Fake<jobject>(), Fake<jmethodID>(3), _));

  MethodRefT_t<kDefaultClassLoader, c, 0>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>(), 1);
  MethodRefT_t<kDefaultClassLoader, c, 1>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>(), 1.234f);
  MethodRefT_t<kDefaultClassLoader, c, 2>::Invoke(Fake<jclass>(),
                                                  Fake<jobject>(), 5.6789f);
}

TEST_F(JniTest, MethodRef_ReturnsObjects) {
  static constexpr Class c1{"Bazz"};
  static constexpr Class kClass{
      "com/google/ReturnsObjects",
      Method{"Foo", Return{c1}, Params<jint>{}},
  };

  // Note, class refs are not released, so Times() != 2.
  EXPECT_CALL(*env_, NewObjectV).WillOnce(testing::Return(Fake<jobject>()));

  GlobalObject<kClass> global_object{};
  LocalObject<c1> new_obj{global_object("Foo", 5)};
}

TEST_F(JniTest, MethodRef_PassesObjects) {
  static constexpr Class c1{"com/google/Bazz"};
  static constexpr Class kClass{
      "com/google/PassesObjects",
      Method{"Foo", Return<jint>{}, Params{c1}},
  };

  LocalObject<c1> local_object{Fake<jobject>()};
  GlobalObject<kClass> global_object{AdoptGlobal{}, Fake<jobject>(100)};

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

  LocalObject<c1> obj1{Fake<jobject>(1)};
  LocalObject<c2> obj2{Fake<jobject>(2)};
  LocalObject<c3> obj3{Fake<jobject>(3)};
  LocalObject<c4> obj4{Fake<jobject>(4)};
  LocalObject<class_under_test> object_under_test{Fake<jobject>(5)};

  LocalObject<c1> obj5{object_under_test("Foo", obj1, obj2, obj3, obj4)};
}

TEST_F(JniTest, MethodRef_SupportsForwardDefines) {
  static constexpr Class kClass1{
      "kClass1",
      Method{"m1", Return<void>{}, Params{Class{"kClass1"}}},
      Method{"m2", Return<void>{}, Params{Class{"kClass2"}}},
      Method{"m3", Return{Class{"kClass1"}}},
      Method{"m4", Return{Class{"kClass2"}}},
  };

  static constexpr Class kClass2{
      "kClass2",
      Method{"m1", Return<void>{}, Params{Class{"kClass1"}}},
      Method{"m2", Return<void>{}, Params{Class{"kClass2"}}},
      Method{"m3", Return{Class{"kClass1"}}},
      Method{"m4", Return{Class{"kClass2"}}},
  };

  LocalObject<kClass1> c1_obj1{Fake<jobject>(1)};
  LocalObject<kClass1> c1_obj2{Fake<jobject>(2)};

  LocalObject<kClass2> c2_obj1{Fake<jobject>(3)};
  LocalObject<kClass2> c2_obj2{Fake<jobject>(4)};

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
      Method{"Baz", Return<jstring>{}},
  };

  LocalObject<class_under_test> obj1{Fake<jobject>()};
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

  LocalArray<jobject, 1, kClass> local_array{nullptr};
  LocalObject<class_under_test> obj1{Fake<jobject>()};
  obj1("Foo", local_array);
}

}  // namespace
