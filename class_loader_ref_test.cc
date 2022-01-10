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

#include <optional>
#include <utility>

#include "jni_test.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"

namespace {

using ::jni::Class;
using ::jni::ClassLoader;
using ::jni::Constructor;
using ::jni::Jvm;
using ::jni::JvmRef;
using ::jni::kDefaultClassLoader;
using ::jni::kNullClassLoader;
using ::jni::LocalClassLoader;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;
using ::jni::SupportedClassSet;
using ::jni::test::AsGlobal;
using ::jni::test::JniTest;
using ::jni::test::JniTestWithNoDefaultJvmRef;
using ::jni::test::kDefaultClassForTests;
using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::StrEq;

// Helper class for classloader tests that gives the standard default class
// object when using CallObjectMethodV.  This is because objects built from
// class loaders are built by calling "loadClass" on the respective classloader
// instance.  JniTest is strict about the number of DeleteGlobalRef calls,
// so this satisfies that requirement.
//
// Note, when using this, you must call |TearDown| to pre-empt and class
// teardown prior to expectations being set.
class JniTestForClassLoaders : public JniTestWithNoDefaultJvmRef {
  void SetUp() override {
    JniTestWithNoDefaultJvmRef::SetUp();

    ON_CALL(*env_, CallObjectMethodV(testing::_, testing::_, testing::_))
        .WillByDefault(
            testing::Invoke([&](jobject obj, jmethodID methodID, va_list) {
              return kDefaultClassForTests;
            }));
  }
};

////////////////////////////////////////////////////////////////////////////////
// Default JVM, non-default classloader (No ID teardown on JVM destruction).
//
// Because these tests use |jni::kDefaultJvm|, the global refs bound to class
// and method IDs won't be freed (they are static across the process).  As a
// hack,  skip testing for them by calling clearing expected globals.
//
// An alternate (more effective) emulation would be to have these tests run
// as independent processes which would reflect the static nature of the memory.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, LocalObject_SupportsPassingAnObjectWithAClassLoader) {
  static constexpr Class kTestClass1{"TestClass1"};

  static constexpr Class kTestClass2{
      "TestClass2", Method{"Foo", jni::Return{}, jni::Params{kTestClass1}}};

  const jobject class_loader_jobject{reinterpret_cast<jobject>(0XABCDE)};

  static constexpr ClassLoader kTestClassLoader{
      jni::kNullClassLoader, jni::SupportedClassSet{kTestClass1}};

  jni::JvmRef<jni::kDefaultJvm> jvm_ref{jvm_.get()};

  // LocalObject<kTestClass2, kTestClassLoader> a{}; // doesn't compile (good).
  LocalObject<kTestClass1, kTestClassLoader> a{class_loader_jobject};
  LocalObject<kTestClass2> b{};
  b("Foo", a);

  default_globals_made_that_should_be_released_.clear();
}

TEST_F(JniTestForClassLoaders,
       ClassLoaderRefTest_ConstructorsAcceptClassLoadedObjects) {
  static constexpr Class kTestClass1{"TestClass1"};
  static constexpr Class kTestClass2{
      "TestClass2", Constructor{kTestClass1},
      Method{"Foo", Return{}, Params{kTestClass1}}};
  static constexpr ClassLoader kClassLoader{kNullClassLoader,
                                            SupportedClassSet{kTestClass1}};

  JvmRef<jni::kDefaultJvm> jvm_ref{jvm_.get()};
  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};

  LocalClassLoader<kClassLoader> local_class_loader{
      class_loader_local_to_be_wrapped};
  auto a = local_class_loader.BuildLocalObject<kTestClass1>();
  LocalObject<kTestClass2> b{a};
  b("Foo", a);

  default_globals_made_that_should_be_released_.clear();
  TearDown();
}

////////////////////////////////////////////////////////////////////////////////
// Non standard JVM, non-default classloader (ID teardown on JVM destruction).
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTestForClassLoaders,
       ClassLoaderRefTest_DefaultLoadedObjectBuildsWithClassLoadedObject) {
  static constexpr Class kTestClass1{"TestClass1"};
  static constexpr Class kTestClass2{
      "TestClass2", Constructor{kTestClass1},
      Method{"Foo", Return{}, Params{kTestClass1}}};
  static constexpr ClassLoader kClassLoader{kNullClassLoader,
                                            SupportedClassSet{kTestClass1}};
  static constexpr Jvm kJvm{kClassLoader};

  JvmRef<kJvm> jvm_ref{jvm_.get()};
  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{
      class_loader_local_to_be_wrapped};
  LocalObject<kTestClass1, kClassLoader> a =
      local_class_loader.BuildLocalObject<kTestClass1>();
  LocalObject<kTestClass2> b{a};
  b("Foo", a);

  TearDown();
}

TEST_F(JniTestForClassLoaders, ClassLoaderRefTest_ConstructsFromRValue) {
  static constexpr Class kTestClass2{"TestClass2"};
  static constexpr Class kTestClass1{
      "TestClass1", Method{"Foo", Return{kTestClass2}, Params{}}};
  static constexpr ClassLoader kClassLoader{
      kNullClassLoader, SupportedClassSet{kTestClass1, kTestClass2}};
  static constexpr Jvm kJvm{kClassLoader};

  JvmRef<kJvm> jvm_ref{jvm_.get()};
  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{
      class_loader_local_to_be_wrapped};
  LocalObject<kTestClass1, kClassLoader, kJvm> b{
      local_class_loader.BuildLocalObject<kTestClass1>()};

  LocalObject<kTestClass2, kClassLoader, kJvm> c{b("Foo")};

  TearDown();
}

TEST_F(JniTestForClassLoaders,
       ClassLoaderRefTest_ClassLoadedObjectBuildsWithDefaultLoadedObject) {
  static constexpr Class kTestClass1{"TestClass1"};
  static constexpr Class kTestClass2{
      "TestClass2", Constructor{kTestClass1},
      Method{"Foo", Return{}, Params{kTestClass1}}};
  static constexpr ClassLoader kClassLoader{kNullClassLoader,
                                            SupportedClassSet{kTestClass2}};
  static constexpr Jvm kJvm{kClassLoader};

  JvmRef<kJvm> jvm_ref{jvm_.get()};
  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{
      class_loader_local_to_be_wrapped};

  LocalObject<kTestClass1> a{};
  LocalObject<kTestClass2, kClassLoader, kJvm> b =
      local_class_loader.BuildLocalObject<kTestClass2>(a);
  b("Foo", a);

  TearDown();
}

TEST_F(
    JniTestForClassLoaders,
    ClassLoaderRefTest_LocalClassLoaderWithSingleClassAndConstructorCompiles) {
  static constexpr Class kTestClass1{
      "TestClass1",
      Constructor<jint>{},
  };
  static constexpr ClassLoader kClassLoader{kNullClassLoader,
                                            SupportedClassSet{kTestClass1}};
  static constexpr Jvm kJvm{kClassLoader};

  JvmRef<kJvm> jvm_ref{jvm_.get()};
  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{
      class_loader_local_to_be_wrapped};
  auto a = local_class_loader.BuildLocalObject<kTestClass1>(12345);
  // local_class_loader.BuildLocalObject<kTestClass1>(); doesn't compile (good)

  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       ClassLoaderRefTest_LocalClassLoaderWithMultipleClassesCompiles) {
  static constexpr Class kTestClass1{"TestClass1"};
  static constexpr Class kTestClass2{"TestClass2"};
  static constexpr Class kTestClass3{"TestClass3"};
  static constexpr Class kTestClass4{"TestClass4"};

  ON_CALL(*env_, CallObjectMethodV(testing::_, testing::_, testing::_))
      .WillByDefault(
          testing::Invoke([&](jobject obj, jmethodID methodID, va_list) {
            return kDefaultClassForTests;
          }));

  static constexpr ClassLoader kClassLoader{
      kNullClassLoader,
      SupportedClassSet{kTestClass1, kTestClass2, kTestClass3, kTestClass4}};
  static constexpr Jvm kJvm{kClassLoader};
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};
  LocalClassLoader<kClassLoader, kJvm> local_class_loader{
      class_loader_local_to_be_wrapped};

  LocalObject a{local_class_loader.BuildLocalObject<kTestClass1>()};
  auto b = local_class_loader.BuildLocalObject<kTestClass2>();
  auto c = local_class_loader.BuildLocalObject<kTestClass3>();
  auto d = local_class_loader.BuildLocalObject<kTestClass4>();

  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       DefaultLoadedObjectAcceptsClassLoadedObject) {
  static constexpr Class kTestClass1{"TestClass1"};
  static constexpr Class kTestClass2{
      "TestClass2", Method{"Foo", Return{}, Params{kTestClass1}}};

  ON_CALL(*env_, CallObjectMethodV(testing::_, testing::_, testing::_))
      .WillByDefault(
          testing::Invoke([&](jobject obj, jmethodID methodID, va_list) {
            return kDefaultClassForTests;
          }));

  static constexpr ClassLoader kClassLoader{kNullClassLoader,
                                            SupportedClassSet{kTestClass1}};
  static constexpr Jvm kJvm{kClassLoader};
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};
  LocalClassLoader<kClassLoader, kJvm> local_class_loader{
      class_loader_local_to_be_wrapped};

  LocalObject<kTestClass1, kClassLoader, kJvm> a =
      local_class_loader.BuildLocalObject<kTestClass1>();
  LocalObject<kTestClass2> b{};
  b("Foo", a);

  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       ClassLoaderRefTest_DefaultLoadedClassCompiles) {
  static constexpr Class kTestClass1{"TestClass1"};

  ON_CALL(*env_, CallObjectMethodV(testing::_, testing::_, testing::_))
      .WillByDefault(
          testing::Invoke([&](jobject obj, jmethodID methodID, va_list) {
            return kDefaultClassForTests;
          }));

  static constexpr ClassLoader kClassLoader{kDefaultClassLoader,
                                            SupportedClassSet{}};
  static constexpr Jvm kJvm{kClassLoader};
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};
  LocalClassLoader<kClassLoader, kJvm> local_class_loader{
      class_loader_local_to_be_wrapped};

  LocalObject a{local_class_loader.BuildLocalObject<kTestClass1>()};
  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       ClassLoaderRefTest_ClassLoadersDoNotConflict) {
  static constexpr Class kTestClassNoCrossTalk{
      "TestClassNoCrossTalk", Constructor<jint>{},
      Method{"methodNoCrossTalk", Return<jint>{}, Params<jint>{}}};
  static constexpr ClassLoader kClassLoader{
      kNullClassLoader, SupportedClassSet{kTestClassNoCrossTalk}};

  static constexpr Jvm kClassLoaderJvm{kClassLoader};

  // We will use this ClassLoader instead of the default loader to load
  // TestClass.
  const jobject class_loader_local_to_be_wrapped{
      reinterpret_cast<jobject>(0XAAAAAA)};

  const jclass class_loader_local_jclass{reinterpret_cast<jclass>(0XACACAC)};

  const jmethodID load_class_jmethod{reinterpret_cast<jmethodID>(0XAEAEAE)};
  const jobject custom_test_jobject{reinterpret_cast<jobject>(0XBABABA)};
  const jobject second_custom_test_jobject{reinterpret_cast<jobject>(0XBBBBBB)};
  const jclass custom_test_local_jclass{reinterpret_cast<jclass>(0XBCBCBC)};
  const jmethodID custom_test_init_jmethod{
      reinterpret_cast<jmethodID>(0XBDBDBD)};
  const jmethodID custom_test_method_jmethod{
      reinterpret_cast<jmethodID>(0XBEBEBE)};
  const jstring custom_test_class_name{reinterpret_cast<jstring>(0XBFBFBF)};

  InSequence sequence;

  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/ClassLoader")))
      .WillOnce(testing::Return(class_loader_local_jclass));

  EXPECT_CALL(*env_, NewGlobalRef(class_loader_local_jclass))
      .WillOnce(testing::Return(AsGlobal(class_loader_local_jclass)));

  EXPECT_CALL(
      *env_,
      GetMethodID(AsGlobal(class_loader_local_jclass), StrEq("loadClass"),
                  StrEq("(Ljava/lang/String;)Ljava/lang/Class;")))
      .WillOnce(testing::Return(load_class_jmethod));

  EXPECT_CALL(*env_, NewStringUTF(_))
      .WillOnce(testing::Return(custom_test_class_name));

  // We should only try to load the class once even if we create multiple
  // instances.
  EXPECT_CALL(*env_, CallObjectMethodV(class_loader_local_to_be_wrapped,
                                       load_class_jmethod, _))
      .WillOnce(testing::Return(custom_test_local_jclass));

  EXPECT_CALL(*env_, NewGlobalRef(custom_test_local_jclass))
      .WillOnce(testing::Return(AsGlobal(custom_test_local_jclass)));
  EXPECT_CALL(*env_, GetMethodID(AsGlobal(custom_test_local_jclass),
                                 StrEq("<init>"), StrEq("(I)V")))
      .WillOnce(testing::Return(custom_test_init_jmethod));
  EXPECT_CALL(*env_, NewObjectV(AsGlobal(custom_test_local_jclass),
                                custom_test_init_jmethod, _))
      .WillOnce(testing::Return(custom_test_jobject));
  EXPECT_CALL(*env_, NewObjectV(AsGlobal(custom_test_local_jclass),
                                custom_test_init_jmethod, _))
      .WillOnce(testing::Return(second_custom_test_jobject));

  EXPECT_CALL(*env_, GetMethodID(AsGlobal(custom_test_local_jclass),
                                 StrEq("methodNoCrossTalk"),

                                 StrEq("(I)I")))
      .WillOnce(testing::Return(custom_test_method_jmethod));
  EXPECT_CALL(
      *env_, CallIntMethodV(custom_test_jobject, custom_test_method_jmethod, _))
      .WillOnce(testing::Return(123));

  // Code under test.
  JvmRef<kClassLoaderJvm> jvm_ref{jvm_.get()};
  LocalClassLoader<kClassLoader, kClassLoaderJvm> class_loader{
      class_loader_local_to_be_wrapped};

  auto custom_loader_object =
      class_loader.BuildLocalObject<kTestClassNoCrossTalk>(jint{1});

  auto second_custom_loader_object =
      class_loader.BuildLocalObject<kTestClassNoCrossTalk>(jint{2});

  EXPECT_EQ(custom_loader_object("methodNoCrossTalk", jint{2}), 123);

  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       ClassLoaderRefTest_ClassesOfDifferentClassLoadersAreUnique) {
  static constexpr Class class_under_test{
      "com.google.ARCore",
      Method{"Foo", jni::Return{}, Params<>{}},
  };
  static constexpr ClassLoader class_loader{
      kNullClassLoader, SupportedClassSet{class_under_test}};

  static constexpr jni::Jvm atypical_jvm_definition{class_loader};

  const jobject test_jobject{reinterpret_cast<jobject>(0XAAAAAA)};
  const jstring test_name_jstring{reinterpret_cast<jstring>(0XABABAB)};
  const jclass test_jclass{reinterpret_cast<jclass>(0XACACAC)};
  const jmethodID test_method_jmethod{reinterpret_cast<jmethodID>(0XAFAFAF)};

  const jclass class_jclass{reinterpret_cast<jclass>(0XBBBBBB)};
  const jmethodID get_class_loader_jmethod{
      reinterpret_cast<jmethodID>(0XBDBDBD)};
  const jclass class_loader_jclass{reinterpret_cast<jclass>(0XCACACA)};
  const jmethodID load_class_jmethod{reinterpret_cast<jmethodID>(0XCACCAC)};
  const jobject class_loader_jobject{reinterpret_cast<jobject>(0XCDCDCD)};
  const jobject class_loader_from_object_ref_jobject{
      reinterpret_cast<jobject>(0XAAADCD)};

  InSequence seq;

  // The java/lang/Class and java/lang/ClassLoader will always be from the
  // default loader, and they only need to be cached once.
  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/Class")))
      .WillOnce(testing::Return(class_jclass));

  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/ClassLoader")))
      .WillOnce(testing::Return(class_loader_jclass));

  EXPECT_CALL(*env_, GetObjectClass(test_jobject))
      .WillOnce(testing::Return(test_jclass));

  EXPECT_CALL(*env_,
              GetMethodID(AsGlobal(class_jclass), StrEq("getClassLoader"),
                          StrEq("()Ljava/lang/ClassLoader;")))
      .WillOnce(testing::Return(get_class_loader_jmethod));

  EXPECT_CALL(*env_,
              CallObjectMethodV(test_jclass, get_class_loader_jmethod, _))
      .WillOnce(testing::Return(class_loader_from_object_ref_jobject));

  EXPECT_CALL(*env_,
              GetMethodID(Eq(AsGlobal(class_loader_jclass)), StrEq("loadClass"),
                          StrEq("(Ljava/lang/String;)Ljava/lang/Class;")))
      .WillOnce(testing::Return(load_class_jmethod));

  EXPECT_CALL(*env_, NewStringUTF(StrEq("com.google.ARCore")))
      .WillOnce(testing::Return(test_name_jstring));

  EXPECT_CALL(*env_, CallObjectMethodV(class_loader_from_object_ref_jobject,
                                       load_class_jmethod, _))
      .WillOnce(testing::Return(class_loader_jobject));

  // Make sure we try to get the method with the loaded class, not the direct
  // object class.
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("()V")))
      .WillOnce(testing::Return(test_method_jmethod));

  // Code under test.
  jni::JvmRef<atypical_jvm_definition> jvm_ref{jvm_.get()};
  jni::LocalObject<class_under_test, class_loader, atypical_jvm_definition>
      obj1{test_jobject};
  obj1("Foo");

  this->TearDown();
}

}  // namespace
