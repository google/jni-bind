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
#include <utility>

#include "jni_bind.h"
#include "class_defs/java_lang_classes.h"

// This test demonstrates templated type erasure with JNI
// by wrapping various Java objects with kJavaLangObject

// Define Java List interface for our tests
constexpr jni::Class kListClass{
    "java/util/List",
    
    // size() method that works for any List<T>
    jni::Method{"size", jni::Return<jint>{}, jni::Params<>{}}
};

// Define our TypeErasureTest class with the generic methods
constexpr jni::Class kTypeErasureTestClass{
    "com/jnibind/test/TypeErasureTest",
    
    // Define the generic method echoGenericValue which takes Object and returns Object
    // In Java this is actually <T> T echoGenericValue(T value)
    // but due to type erasure, JNI sees it as Object echoGenericValue(Object value)
    jni::Method{"echoGenericValue", 
                jni::Return{jni::kJavaLangObject}, 
                jni::Params{jni::kJavaLangObject}},
    
};

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_jnibind_test_TypeErasureTest_jniTearDown(
    JNIEnv* env, jclass) {
  jvm = nullptr;
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_test_TypeErasureTest_jniEchoGenericValue(
    JNIEnv* env, jobject thiz, jobject obj) {
  // Wrap the test object instance to call its methods
  jni::LocalObject<kTypeErasureTestClass> test_instance{thiz};
  
  // Wrap the input object with kJavaLangObject for type erasure
  // This is where JNI-Bind uses type erasure to handle any type (Integer, String, etc.)
  jni::LocalObject<jni::kJavaLangObject> input_obj{obj};
  
  // Call the generic Java method which will maintain the actual type
  // The type information is preserved by Java even though JNI Bind
  // only sees it as a kJavaLangObject
  auto result = test_instance.Call<"echoGenericValue">(input_obj);
  
  // Return the result, which still maintains its original type in Java
  return result.Release();
}

}  // extern "C"
