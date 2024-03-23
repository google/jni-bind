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

#include "object_test_helper_jni.h"
#include "jni_bind.h"

using ::jni::Class;
using ::jni::ClassLoader;
using ::jni::Constructor;
using ::jni::GlobalClassLoader;
using ::jni::kDefaultClassLoader;
using ::jni::LocalClassLoader;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::PromoteToGlobal;
using ::jni::Return;
using ::jni::SupportedClassSet;

static constexpr Class kClassLoaderTestClass{
    "com/jnibind/android/ClassLoaderTest",
    Method{"methodTakesLocalObjectReturnsNewObject",
           Return{kObjectTestHelperClass}, Params{kObjectTestHelperClass}},
};

static constexpr ClassLoader kTestClassLoader{
    kDefaultClassLoader,
    SupportedClassSet{kClassLoaderTestClass, kObjectTestHelperClass}};

static constexpr ClassLoader kLiteTestClassLoader{
    kDefaultClassLoader, SupportedClassSet{kClassLoaderTestClass}};

static constexpr Class kClassLoaderHelperClass{
    "com/jnibind/android/ClassLoaderHelperClass", Constructor<jint>{},
    Method{"getValue", Return<jint>{}}};
static constexpr Class kClassLoaderHelperSubclass{
    "com/jnibind/android/ClassLoaderHelperSubclass", Constructor<jint>{},
    Method{"getValue", Return<jint>{}},
    Method{"castToParent", Return{kClassLoaderHelperClass}}};

static constexpr ClassLoader kCustomClassLoader{
    kDefaultClassLoader,
    SupportedClassSet{kClassLoaderHelperClass, kClassLoaderHelperSubclass}};

static constexpr jni::Jvm kJvmWithCustomClassLoaderSupport{
    kTestClassLoader, kLiteTestClassLoader, kCustomClassLoader};

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  static jni::JvmRef<kJvmWithCustomClassLoaderSupport> jvm{pjvm};
  return JNI_VERSION_1_6;
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_ClassLoaderTest_jniBuildNewObjectsFromClassLoader(
    JNIEnv* env, jobject jtest_obj, jobject jclass_loader_obj) {
  GlobalClassLoader<kTestClassLoader, kJvmWithCustomClassLoaderSupport>
      class_loader{PromoteToGlobal{}, jclass_loader_obj};

  LocalObject helper_obj =
      class_loader.BuildLocalObject<kObjectTestHelperClass>(1, 2, 3);
  return LocalObject<kClassLoaderTestClass, kTestClassLoader,
                     kJvmWithCustomClassLoaderSupport>{jtest_obj}(
             "methodTakesLocalObjectReturnsNewObject", helper_obj)
      .Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_ClassLoaderTest_jniBuildNewObjectsFromDefaultClassLoader(
    JNIEnv* env, jobject jtest_obj, jobject jclass_loader_obj) {
  GlobalClassLoader<kLiteTestClassLoader, kJvmWithCustomClassLoaderSupport>
      class_loader{PromoteToGlobal{}, jclass_loader_obj};
  LocalObject<kObjectTestHelperClass> helper_obj{2, 3, 4};
  return LocalObject<kClassLoaderTestClass>{jtest_obj}(
             "methodTakesLocalObjectReturnsNewObject", helper_obj)
      .Release();
}

JNIEXPORT jint JNICALL
Java_com_jnibind_android_ClassLoaderTest_jniBuildsRemoteSubclassFromClassLoader(
    JNIEnv* env, jobject jtest_obj, jobject jclass_loader_obj, jint value) {
  LocalClassLoader<kCustomClassLoader, kJvmWithCustomClassLoaderSupport>
      class_loader{jclass_loader_obj};
  LocalObject<kClassLoaderHelperSubclass, kCustomClassLoader,
              kJvmWithCustomClassLoaderSupport>
      helper_obj = class_loader.BuildLocalObject<kClassLoaderHelperSubclass>(
          jint{value});
  LocalObject<kClassLoaderHelperClass, kCustomClassLoader,
              kJvmWithCustomClassLoaderSupport>
      parent_obj{helper_obj("castToParent")};

  return parent_obj("getValue");
}

JNIEXPORT jint JNICALL
Java_com_jnibind_android_ClassLoaderTest_jniBuildsRemoteClassFromClassLoader(
    JNIEnv* env, jobject jtest_obj, jobject jclass_loader_obj, jint value) {
  LocalClassLoader<kCustomClassLoader, kJvmWithCustomClassLoaderSupport>
      class_loader{jclass_loader_obj};
  LocalObject<kClassLoaderHelperClass, kCustomClassLoader,
              kJvmWithCustomClassLoaderSupport>
      helper_obj =
          class_loader.BuildLocalObject<kClassLoaderHelperClass>(jint{value});

  return helper_obj("getValue");
}

}  // extern "C"
