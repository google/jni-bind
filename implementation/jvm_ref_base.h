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

#ifndef JNI_BIND_IMPLEMENTATION_JVM_REF_BASE_H_
#define JNI_BIND_IMPLEMENTATION_JVM_REF_BASE_H_

#include <atomic>

#include "implementation/forward_declarations.h"
#include "jni_dep.h"

namespace jni {

// Helper for JvmRef to enforce correct sequencing of getting and setting
// process level static fo JavaVM*.
class JvmRefBase {
 protected:
  friend class ThreadGuard;
  friend class ThreadLocalGuardDestructor;

  JvmRefBase(JavaVM* vm) { process_level_jvm_.store(vm); }
  ~JvmRefBase() { process_level_jvm_.store(nullptr); }

  static JavaVM* GetJavaVm() { return process_level_jvm_.load(); }
  static void SetJavaVm(JavaVM* jvm) { process_level_jvm_.store(jvm); }

  static inline std::atomic<JavaVM*> process_level_jvm_ = nullptr;
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_JVM_REF_BASE_H_
