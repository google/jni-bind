/*
 * Copyright 2024 Google LLC
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

using ::jni::Class;
using ::jni::Field;
using ::jni::GlobalObject;
using ::jni::kJavaLangClass;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::PromoteToGlobal;
using ::jni::Return;
using ::jni::ThreadGuard;

std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

}  // namespace

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>{pjvm});

  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_jnibind_android_ThreadTest_jniTearDown(JNIEnv* env, jclass) {
  jvm = nullptr;
}

JNIEXPORT void JNICALL
Java_com_jnibind_android_ThreadTest_runsThreadedWorkOnObject(JNIEnv* env,
                                                             jclass,
                                                             jobject fixture) {
  static constexpr Class kFixtureClass{
      "com/jnibind/android/ThreadTest",
      Field{"objectTestHelper", kObjectTestHelperClass}};

  // To avoid premature caching of `ObjectTestHelperClass`'s `jclass`, a
  // copy of the fixture is held, which defers jclass lookup.
  GlobalObject<kFixtureClass> fixture_object{PromoteToGlobal{}, fixture};
  jvm->SetFallbackClassLoaderFromJObject(fixture);

  std::thread worker{[fixture{std::move(fixture_object)}]() mutable {
    ThreadGuard thread_guard{};

    // This will crash without fallback loader (`jclass` will be unknown).
    fixture["objectTestHelper"].Get()("foo");
  }};

  worker.join();
}

}  // extern "C"
