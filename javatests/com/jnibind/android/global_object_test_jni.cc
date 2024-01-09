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

constexpr jni::Class kGlobalObjectTestClass{
    "com/jnibind/android/GlobalObjectTest",
    jni::Method{"methodTakesGlobalObjectReturnsNewObject",
                jni::Return{kObjectTestHelperClass},
                jni::Params{kObjectTestHelperClass}},
};

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  static jni::JvmRef<jni::kDefaultJvm> jvm{pjvm};
  return JNI_VERSION_1_6;
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_GlobalObjectTest_jniCreateNewObject(JNIEnv* env, jclass) {
  return jni::GlobalObject<kObjectTestHelperClass>{1, 2, 3}.Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_GlobalObjectTest_jniCreateNewGlobalObjectSetIntVal123(
    JNIEnv* env, jclass) {
  return jni::GlobalObject<kObjectTestHelperClass>{1, 2, 3}.Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_GlobalObjectTest_jniBuildNewObjectsFromExistingObjects(
    JNIEnv* env, jclass, jobject test_helper_object, jobject object_to_mutate) {
  jni::LocalObject<kObjectTestHelperClass> helper_obj{object_to_mutate};
  return jni::LocalObject<kGlobalObjectTestClass>{test_helper_object}(
             "methodTakesGlobalObjectReturnsNewObject", helper_obj)
      .Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_GlobalObjectTest_jniManipulateNewGlobalObjectSetIntVal238(
    JNIEnv* env, jclass, jobject jtest_obj) {
  jni::GlobalObject<kObjectTestHelperClass> helper_obj{2, 3, 8};
  return jni::LocalObject<kGlobalObjectTestClass>{jtest_obj}(
             "methodTakesGlobalObjectReturnsNewObject", std::move(helper_obj))
      .Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_GlobalObjectTest_jniMaterializeNewGlobalObjectSetIntVal159(
    JNIEnv* env, jclass, jobject jtest_obj) {
  return jni::LocalObject<kGlobalObjectTestClass>{jtest_obj}(
             "methodTakesGlobalObjectReturnsNewObject",
             jni::GlobalObject<kObjectTestHelperClass>{1, 5, 9})
      .Release();
}

}  // extern "C"
