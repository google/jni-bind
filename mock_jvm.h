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

#ifndef JNI_BIND_MOCK_JVM_H_
#define JNI_BIND_MOCK_JVM_H_

#include <gmock/gmock.h>
#include "jni_dep.h"

namespace jni::test {

// Functions in JavaVM are not virtual and therefore we cannot mock them.
// Fortunately, it uses function pointers to find the real methods. So, we
// can override the function pointers and redirect them to MockJvmBase
// virtual functions. Then, we build a mock on top of it.
class MockJvmBase : public JavaVM {
 public:
  MockJvmBase() {
    // Set the static function table used in JNI.
    functions = &functions_;

    functions_.DestroyJavaVM = &CallDestroyJavaVM;
    functions_.AttachCurrentThread = &CallAttachCurrentThread;
    functions_.DetachCurrentThread = &CallDetachCurrentThread;
    functions_.GetEnv = &CallGetEnv;
    functions_.AttachCurrentThreadAsDaemon = &CallAttachCurrentThreadAsDaemon;
  }
  virtual ~MockJvmBase() = default;

  virtual jint DestroyJavaVM() = 0;
  virtual jint AttachCurrentThread(void** penv, void* args) = 0;
  virtual jint DetachCurrentThread() = 0;
  virtual jint GetEnv(void** penv, jint version) = 0;
  virtual jint AttachCurrentThreadAsDaemon(
      void** penv, void* args) = 0;

 private:
  static jint JNICALL CallDestroyJavaVM(JavaVM* vm) {
    return static_cast<MockJvmBase*>(vm)->DestroyJavaVM();
  }
  static jint JNICALL CallAttachCurrentThread(
      JavaVM* vm, void** penv, void* args) {
    return static_cast<MockJvmBase*>(vm)->AttachCurrentThread(penv, args);
  }
  static jint JNICALL CallDetachCurrentThread(JavaVM* vm) {
    return static_cast<MockJvmBase*>(vm)->DetachCurrentThread();
  }
  static jint JNICALL CallGetEnv(JavaVM* vm, void** penv, jint version) {
    return static_cast<MockJvmBase*>(vm)->GetEnv(penv, version);
  }
  static jint JNICALL CallAttachCurrentThreadAsDaemon(
      JavaVM* vm, void** penv, void* args) {
    return static_cast<MockJvmBase*>(vm)->
        AttachCurrentThreadAsDaemon(penv, args);
  }

  JNIInvokeInterface_ functions_;
};

class MockJvm : public MockJvmBase {
 public:
  MOCK_METHOD(jint, DestroyJavaVM, (), (override));
  MOCK_METHOD(jint, AttachCurrentThread, (void** penv, void* args), (override));
  MOCK_METHOD(jint, DetachCurrentThread, (), (override));
  MOCK_METHOD(jint, GetEnv, (void** penv, jint version), (override));
  MOCK_METHOD(jint, AttachCurrentThreadAsDaemon,
              (void** penv, void* args),
              (override));
};

} // jni::test

#endif  // JNI_BIND_MOCK_JVM_H_
