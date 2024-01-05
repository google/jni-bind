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

#ifndef JNI_BIND_IMPLEMENTATION_THREAD_GUARD_H_
#define JNI_BIND_IMPLEMENTATION_THREAD_GUARD_H_

#include "implementation/forward_declarations.h"
#include "implementation/jvm_ref_base.h"
#include "jni_dep.h"
#include "metaprogramming/function_traits.h"

namespace jni {

// Designed to be the very last JniBind object to execute on the thread.
// Objects passed by move for lambdas will be destructed after any contents
// statements within their lambda, and `ThreadGuard` can't be moved into the
// lambda because its construction will be on the host thread. This static
// teardown guarantees a delayed destruction beyond any GlobalObject.
class ThreadLocalGuardDestructor {
 public:
  bool detach_thread_when_all_guards_released_ = false;

  // By calling this the compiler is obligated to perform initalisation.
  void ForceDestructionOnThreadClose() {}

  ~ThreadLocalGuardDestructor() {
    if (detach_thread_when_all_guards_released_) {
      JavaVM* jvm = JvmRefBase::GetJavaVm();
      if (jvm) {
        jvm->DetachCurrentThread();
      }
    }
  }
};

// ThreadGuard attaches and detaches JNIEnv* objects on the creation of new
// threads.  All new threads which want to use JNI Wrapper must hold a
// ThreadGuard beyond the scope of all created objects.  If the ThreadGuard
// needs to create an Env, it will also detach itself.
class ThreadGuard {
 public:
  ~ThreadGuard() { thread_guard_count_--; }

  ThreadGuard(ThreadGuard&) = delete;
  ThreadGuard(ThreadGuard&&) = delete;

  template <const auto& jvm_v_>
  friend class JvmRef;

  // This constructor must *never* be called before a |JvmRef| has been
  // constructed. It depends on static setup from |JvmRef|.
  [[nodiscard]] ThreadGuard() {
    thread_local_guard_destructor.ForceDestructionOnThreadClose();

    // Nested ThreadGuards should be permitted in the same way mutex locks are.
    thread_guard_count_++;
    if (thread_guard_count_ != 1) {
      // SetEnv has been called prior, GetEnv is currently valid.
      return;
    }

    // Declarations for AttachCurrentThread are inconsistent across different
    // JNI headers.  This forces a cast to whatever the expected type is.
    JavaVM* const vm = JvmRefBase::GetJavaVm();
    JNIEnv* jni_env = 0;

    using TypeForGetEnv =
        metaprogramming::FunctionTraitsArg_t<decltype(&JavaVM::GetEnv), 1>;
    const int code =
        vm->GetEnv(reinterpret_cast<TypeForGetEnv>(&jni_env), JNI_VERSION_1_6);

    if (code != JNI_OK) {
      using TypeForAttachment = metaprogramming::FunctionTraitsArg_t<
          decltype(&JavaVM::AttachCurrentThread), 1>;
      vm->AttachCurrentThread(reinterpret_cast<TypeForAttachment>(&jni_env),
                              nullptr);
      thread_local_guard_destructor.detach_thread_when_all_guards_released_ =
          true;
    }
    // Why not store this locally to ThreadGuard?
    //
    // JNIEnv is thread local static, and the context an object is built from
    // may not have easy access to a JNIEnv* (or this ThreadGuard).  For most
    // constructions of new objects, the env is likely trivial (it's passed as
    // part of the JNI call), however, if an object reference is moved from one
    // thread to another, the JNIEnv* is certainly not available.
    JniEnv::SetEnv(jni_env);
  }

 private:
  static inline thread_local int thread_guard_count_ = 0;
  static inline thread_local ThreadLocalGuardDestructor
      thread_local_guard_destructor{};
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_THREAD_GUARD_H_
