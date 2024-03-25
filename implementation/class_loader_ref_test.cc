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
#include <optional>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptGlobal;
using ::jni::AdoptLocal;
using ::jni::Class;
using ::jni::ClassLoader;
using ::jni::Constructor;
using ::jni::Fake;
using ::jni::GlobalClassLoader;
using ::jni::Jvm;
using ::jni::JvmRef;
using ::jni::kDefaultJvm;
using ::jni::kNullClassLoader;
using ::jni::LocalClassLoader;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::PromoteToGlobal;
using ::jni::Return;
using ::jni::SupportedClassSet;
using ::jni::test::AsGlobal;
using ::jni::test::JniTest;
using ::jni::test::JniTestWithNoDefaultJvmRef;
using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::StrEq;

static constexpr Class kClass1{"Class1", Constructor{}, Constructor<jint>{},
                               Method{"Foo", Return{Class{"Class2"}}}};

static constexpr Class kClass2{
    "Class2", Constructor{}, Constructor{kClass1},
    Method{"Foo", jni::Return{}, jni::Params{kClass1}}};

static constexpr Class kClass3{"Class3"};
static constexpr Class kClass4{"Class4"};

static constexpr ClassLoader kClassLoader{kNullClassLoader,
                                          SupportedClassSet{kClass1, kClass2}};

static constexpr Jvm kJvm{kClassLoader};

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

    ON_CALL(*env_, CallObjectMethodV)
        .WillByDefault(::testing::Return(Fake<jclass>()));
  }
};

TEST_F(JniTest, LocalsAreMoveable) {
  LocalClassLoader<kClassLoader, kJvm> obj_1{Fake<jobject>()};
  LocalClassLoader<kClassLoader, kJvm> obj_2{std::move(obj_1)};
}

TEST_F(JniTest, GlobalClassLoadersSupportAdoptionMechanics) {
  EXPECT_CALL(*env_, DeleteLocalRef).Times(0);
  GlobalClassLoader<kClassLoader, kJvm> obj_1{AdoptGlobal{}, Fake<jobject>()};
}

TEST_F(JniTest, GlobalClassLoadersSupportPromoteMechanics) {
  EXPECT_CALL(*env_, DeleteLocalRef).Times(1);
  GlobalClassLoader<kClassLoader, kJvm> obj_1{PromoteToGlobal{},
                                              Fake<jobject>()};
}

TEST_F(JniTest, GlobalsAreMoveable) {
  GlobalClassLoader<kClassLoader, kJvm> obj_1{AdoptGlobal{}, Fake<jobject>()};
  GlobalClassLoader<kClassLoader, kJvm> obj_2{std::move(obj_1)};
}

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
  JvmRef<kDefaultJvm> jvm_ref{jvm_.get()};

  // LocalObject<kClass2, kClassLoader> a{}; // doesn't compile (good).
  LocalObject<kClass1, kClassLoader> a{Fake<jobject>()};
  LocalObject<kClass2> b{};
  b("Foo", a);

  default_globals_made_that_should_be_released_.clear();
}

TEST_F(JniTestForClassLoaders,
       ClassLoaderRefTest_ConstructorsAcceptClassLoadedObjects) {
  JvmRef<kDefaultJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader> local_class_loader{Fake<jobject>()};
  auto a = local_class_loader.BuildLocalObject<kClass1>();
  LocalObject<kClass2> b{a};
  b("Foo", a);

  default_globals_made_that_should_be_released_.clear();
  TearDown();
}

TEST_F(JniTestForClassLoaders,
       lassLoaderRefTest_ConstructorsAcceptGlobalClassLoadedObjects) {
  JvmRef<kDefaultJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader> local_class_loader{Fake<jobject>()};
  auto a = local_class_loader.BuildGlobalObject<kClass1>();
  LocalObject<kClass2> b{a};
  b("Foo", a);

  default_globals_made_that_should_be_released_.clear();
  TearDown();
}

////////////////////////////////////////////////////////////////////////////////
// Non standard JVM, non-default classloader (ID teardown on JVM destruction).
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTestForClassLoaders,
       ClassLoaderRefTest_DefaultLoadedObjectBuildsWithClassLoadedObject) {
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{AdoptLocal{},
                                                          Fake<jobject>()};
  LocalObject<kClass1, kClassLoader> a =
      local_class_loader.BuildLocalObject<kClass1>();
  LocalObject<kClass2> b{a};
  b("Foo", a);

  TearDown();
}

TEST_F(JniTestForClassLoaders, ClassLoaderRefTest_ConstructsFromRValue) {
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{Fake<jobject>()};
  LocalObject<kClass1, kClassLoader, kJvm> b{
      local_class_loader.BuildLocalObject<kClass1>()};

  LocalObject<kClass2, kClassLoader, kJvm> c{b("Foo")};

  TearDown();
}

TEST_F(JniTestForClassLoaders,
       ClassLoaderRefTest_ClassLoadedObjectBuildsWithDefaultLoadedObject) {
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{Fake<jobject>()};

  LocalObject<kClass1> a{};
  LocalObject<kClass2, kClassLoader, kJvm> b =
      local_class_loader.BuildLocalObject<kClass2>(a);
  b("Foo", a);

  TearDown();
}

TEST_F(
    JniTestForClassLoaders,
    ClassLoaderRefTest_LocalClassLoaderWithSingleClassAndConstructorCompiles) {
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{Fake<jobject>()};
  auto a = local_class_loader.BuildLocalObject<kClass1>(12345);
  // local_class_loader.BuildLocalObject<kClass1>(); doesn't compile (good)

  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       ClassLoaderRefTest_LocalClassLoaderWithMultipleClassesCompiles) {
  ON_CALL(*env_, CallObjectMethodV)
      .WillByDefault(::testing::Return(Fake<jclass>()));

  static constexpr ClassLoader kClassLoader{
      kNullClassLoader, SupportedClassSet{kClass1, kClass2, kClass3, kClass4}};
  static constexpr Jvm kJvm{kClassLoader};
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{Fake<jobject>()};

  LocalObject a{local_class_loader.BuildLocalObject<kClass1>()};
  auto b = local_class_loader.BuildLocalObject<kClass2>();
  auto c = local_class_loader.BuildLocalObject<kClass3>();
  auto d = local_class_loader.BuildLocalObject<kClass4>();

  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       DefaultLoadedObjectAcceptsClassLoadedObject) {
  ON_CALL(*env_, CallObjectMethodV(testing::_, testing::_, testing::_))
      .WillByDefault(::testing::Return(Fake<jclass>()));

  JvmRef<kJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{Fake<jobject>()};

  LocalObject<kClass1, kClassLoader, kJvm> a =
      local_class_loader.BuildLocalObject<kClass1>();
  LocalObject<kClass2> b{};
  b("Foo", a);

  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       ClassLoaderRefTest_DefaultLoadedClassCompiles) {
  ON_CALL(*env_, CallObjectMethodV(testing::_, testing::_, testing::_))
      .WillByDefault(::testing::Return(Fake<jclass>()));
  JvmRef<kJvm> jvm_ref{jvm_.get()};

  LocalClassLoader<kClassLoader, kJvm> local_class_loader{Fake<jobject>()};

  LocalObject a{local_class_loader.BuildLocalObject<kClass1>()};
  TearDown();
}

TEST_F(JniTestWithNoDefaultJvmRef,
       ClassLoaderRefTest_ClassesOfDifferentClassLoadersAreUnique) {
  static constexpr Class class_under_test{
      "com/google/ARCore",
      Method{"Foo", jni::Return{}},
  };
  static constexpr ClassLoader class_loader{
      kNullClassLoader, SupportedClassSet{class_under_test}};

  static constexpr jni::Jvm atypical_jvm_definition{class_loader};

  InSequence seq;

  // The java/lang/Class and java/lang/ClassLoader will always be from the
  // default loader, and they only need to be cached once.
  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/Class")))
      .WillOnce(testing::Return(Fake<jclass>(2)));

  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/ClassLoader")))
      .WillOnce(testing::Return(Fake<jclass>(3)));

  EXPECT_CALL(*env_, GetObjectClass(Fake<jobject>(1)))
      .WillOnce(testing::Return(Fake<jclass>(1)));

  EXPECT_CALL(*env_,
              GetMethodID(AsGlobal(Fake<jclass>(2)), StrEq("getClassLoader"),
                          StrEq("()Ljava/lang/ClassLoader;")))
      .WillOnce(testing::Return(Fake<jmethodID>(2)));

  EXPECT_CALL(*env_, CallObjectMethodV(Fake<jclass>(1), Fake<jmethodID>(2), _))
      .WillOnce(testing::Return(Fake<jobject>(3)));

  EXPECT_CALL(*env_,
              GetMethodID(Eq(AsGlobal(Fake<jclass>(3))), StrEq("loadClass"),
                          StrEq("(Ljava/lang/String;)Ljava/lang/Class;")))
      .WillOnce(testing::Return(Fake<jmethodID>(1)));

  // Note: While "/" is the mandatory delimiter for describing the class in its
  // definition, load class uses "." delineation. Strangely, when calling
  // Classloader.loadClass on Android both '.' and '/'work, but on x86 Java (and
  // presumably other JVM implementations), only the "." is accepted.
  EXPECT_CALL(*env_, NewStringUTF(StrEq("com.google.ARCore")))
      .WillOnce(testing::Return(Fake<jstring>()));

  EXPECT_CALL(*env_, CallObjectMethodV(Fake<jobject>(3), Fake<jmethodID>(1), _))
      .WillOnce(testing::Return(Fake<jobject>(2)));

  // Make sure we try to get the method with the loaded class, not the direct
  // object class.
  EXPECT_CALL(*env_, GetMethodID(_, StrEq("Foo"), StrEq("()V")))
      .WillOnce(testing::Return(Fake<jmethodID>(1)));

  // Code under test.
  jni::JvmRef<atypical_jvm_definition> jvm_ref{jvm_.get()};
  jni::LocalObject<class_under_test, class_loader, atypical_jvm_definition>
      obj1{AdoptLocal{}, Fake<jobject>(1)};
  obj1("Foo");

  this->TearDown();
}

}  // namespace
