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

static constexpr jni::Class kLocalObjectTestClass{
    "com/jnibind/android/LocalObjectTest",
    jni::Method{"methodTakesLocalObjectReturnsNewObject",
                jni::Return{kObjectTestHelperClass},
                jni::Params{kObjectTestHelperClass}},
};

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  static jni::JvmRef<jni::kDefaultJvm> jvm{pjvm};
  return JNI_VERSION_1_6;
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_LocalObjectTest_jniCreateNewObject(JNIEnv* env,
                                                            jclass) {
  return jni::LocalObject<kObjectTestHelperClass>{1, 2, 3}.Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_LocalObjectTest_jniCreateNewLocalObjectFromObject(
    JNIEnv* env, jclass, jobject object) {
  jni::LocalObject<kObjectClass> java_object{object};
  return jni::LocalObject<kObjectTestHelperClass>{java_object}.Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_LocalObjectTest_jniCreateNewLocalObjectSetIntVal123(
    JNIEnv* env, jclass) {
  return jni::LocalObject<kObjectTestHelperClass>{1, 2, 3}.Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_LocalObjectTest_jniBuildNewObjectsFromExistingObjects(
    JNIEnv* env, jclass, jobject jtest_obj, jobject jhelper_obj) {
  jni::LocalObject<kObjectTestHelperClass> helper_obj{jhelper_obj};
  return jni::LocalObject<kLocalObjectTestClass>{jtest_obj}
      .Call<"methodTakesLocalObjectReturnsNewObject">(helper_obj)
      .Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_LocalObjectTest_jniManipulateNewLocalObjectSetIntVal238(
    JNIEnv* env, jclass, jobject jtest_obj) {
  jni::LocalObject<kObjectTestHelperClass> helper_obj{2, 3, 8};
  return jni::LocalObject<kLocalObjectTestClass>{jtest_obj}
      .Call<"methodTakesLocalObjectReturnsNewObject">(std::move(helper_obj))
      .Release();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_LocalObjectTest_jniMaterializeNewLocalObjectSetIntVal159(
    JNIEnv* env, jclass, jobject jtest_obj) {
  return jni::LocalObject<kLocalObjectTestClass>{jtest_obj}
      .Call<"methodTakesLocalObjectReturnsNewObject">(
          jni::LocalObject<kObjectTestHelperClass>{1, 5, 9})
      .Release();
}

}  // extern "C"
