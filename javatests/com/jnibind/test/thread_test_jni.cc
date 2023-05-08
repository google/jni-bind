/*
 * Copyright 2023 Google LLC
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
#include <thread>  // NOLINT

#include "object_test_helper_jni.h"
#include "jni_bind.h"

namespace {

using ::jni::GlobalObject;
using ::jni::PromoteToGlobal;
using ::jni::ThreadGuard;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

}  // namespace

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ThreadTest_jniTearDown(JNIEnv* env,
                                                                    jclass) {
  jvm = nullptr;
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ThreadTest_DoSetup(JNIEnv* env,
                                                                jclass) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>{env});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ThreadTest_RunsThreadedWorkOnObject(JNIEnv* env, jclass,
                                                          jobject obj) {
  GlobalObject<kObjectTestHelperClass> global_obj{PromoteToGlobal{}, obj};

  std::thread worker{
      [global_object_lambda_scope{std::move(global_obj)}]() mutable {
        ThreadGuard thread_guard{};
        global_object_lambda_scope("foo");
      }};

  worker.join();
}

}  // extern "C"
