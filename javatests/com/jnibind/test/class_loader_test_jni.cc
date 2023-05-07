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

#include <memory>

#include "object_test_helper_jni.h"
#include "jni_bind.h"

using ::jni::Class;
using ::jni::ClassLoader;
using ::jni::kDefaultClassLoader;
using ::jni::LocalClassLoader;
using ::jni::SupportedClassSet;

static constexpr Class kClassLoaderHelperClass{
    "com/jnibind/test/ClassLoaderHelperClass"};

static constexpr ClassLoader kTestClassLoader{
    kDefaultClassLoader, SupportedClassSet{kClassLoaderHelperClass}};

static constexpr jni::Jvm kJvmWithCustomClassLoaderSupport{kTestClassLoader};

static std::unique_ptr<jni::JvmRef<kJvmWithCustomClassLoaderSupport>> jvm;

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<kJvmWithCustomClassLoaderSupport>(pjvm));

  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ClassLoaderTest_jniTearDown(JNIEnv* env, jclass) {
  jvm.reset();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_test_ClassLoaderTest_jniBuildNewObjectsFromClassLoader(
    JNIEnv* env, jclass, jobject jclass_loader_obj) {
  LocalClassLoader<kTestClassLoader, kJvmWithCustomClassLoaderSupport>
      class_loader{jclass_loader_obj};

  return class_loader.BuildLocalObject<kClassLoaderHelperClass>().Release();
}

}  // extern "C"
