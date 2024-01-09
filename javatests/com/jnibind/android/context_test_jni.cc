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

namespace {

using ::jni::GlobalObject;

// A struct that could represent context to be maintained across multiple native
// JNI invocations.  Only global objects can be held in this way, local objects
// cannot persist past the life of their containing JNI call.
struct ContextStruct {
  GlobalObject<kObjectTestHelperClass> obj;
};

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

}  // namespace

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_jnibind_android_ContextTest_DoSetup(JNIEnv* env,
                                                                 jclass) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>{env});
}

JNIEXPORT jlong JNICALL
Java_com_jnibind_android_ContextTest_CreateNativeContextObject(JNIEnv* env, jclass,
                                                            jint val) {
  auto* ctx_struct = new ContextStruct{GlobalObject<kObjectTestHelperClass>{}};
  ctx_struct->obj["intVal1"].Set(jint{val});

  return reinterpret_cast<jlong>(ctx_struct);
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_android_ContextTest_CreateNativeContextObjectSetValToSum(
    JNIEnv* env, jclass, jint val1, jint val2) {
  // Creates a temporary test helper, calls its member method, and releases the
  // returned object across the C API boundary (then destroys the temporary).
  return jni::LocalObject<kObjectTestHelperClass>{}(
             "returnNewObjectWithFieldSetToSum", val1, val2)
      .Release();
}

JNIEXPORT jobject JNICALL Java_com_jnibind_android_ContextTest_QueryNativeObject(
    JNIEnv* env, jclass, void* ctx_void_ptr) {
  ContextStruct* ctx_struct = static_cast<ContextStruct*>(ctx_void_ptr);
  return jobject{ctx_struct->obj};
}

JNIEXPORT jobject JNICALL Java_com_jnibind_android_ContextTest_ExtractNativeObject(
    JNIEnv* env, jclass, void* ctx_void_ptr) {
  ContextStruct* ctx_struct = static_cast<ContextStruct*>(ctx_void_ptr);
  return ctx_struct->obj.Release();
}

JNIEXPORT void JNICALL Java_com_jnibind_android_ContextTest_DestroyNativeContext(
    JNIEnv* env, jclass, void* ctx_void_ptr) {
  delete static_cast<ContextStruct*>(ctx_void_ptr);
}

}  // extern "C"
