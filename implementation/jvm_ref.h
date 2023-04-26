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

#ifndef JNI_BIND_JVM_REF_H_
#define JNI_BIND_JVM_REF_H_

#include <atomic>
#include <utility>

#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/class_ref.h"
#include "implementation/default_class_loader.h"
#include "implementation/field_ref.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/method_ref.h"
#include "jni_dep.h"
#include "metaprogramming/double_locked_value.h"
#include "metaprogramming/function_traits.h"

namespace jni {

template <const auto& jvm_v_ = kDefaultJvm>
class JvmRef;

class ThreadGuard;

// Helper for JvmRef to enforce correct sequencing of getting and setting
// process level static fo JavaVM*.
class JvmRefBase {
 protected:
  friend class ThreadGuard;

  JvmRefBase(JavaVM* vm) { process_level_jvm_.store(vm); }
  ~JvmRefBase() { process_level_jvm_.store(nullptr); }

  static JavaVM* GetJavaVm() { return process_level_jvm_.load(); }
  static void SetJavaVm(JavaVM* jvm) { process_level_jvm_.store(jvm); }

  static inline std::atomic<JavaVM*> process_level_jvm_ = nullptr;
};

// ThreadGuard attaches and detaches JNIEnv* objects on the creation of new
// threads.  All new threads which want to use JNI Wrapper must hold a
// ThreadGuard beyond the scope of all created objects.  If the ThreadGuard
// needs to create an Env, it will also detach itself.
class ThreadGuard {
 public:
  ~ThreadGuard() {
    thread_guard_count_--;
    if (thread_guard_count_ == 0 && detach_thread_when_all_guards_released_) {
      JvmRefBase::GetJavaVm()->DetachCurrentThread();
    }
  }

  ThreadGuard(ThreadGuard&) = delete;
  ThreadGuard(ThreadGuard&&) = delete;

  template <const auto& jvm_v_>
  friend class JvmRef;

  // This constructor must *never* be called before a |JvmRef| has been
  // constructed. It depends on static setup from |JvmRef|.
  [[nodiscard]] ThreadGuard() {
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
      detach_thread_when_all_guards_released_ = true;
    } else {
      detach_thread_when_all_guards_released_ = false;
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
  static inline thread_local bool detach_thread_when_all_guards_released_;
};

// Represents a runtime instance of a Java Virtual Machine.
// The caller is responsible for dropping this object from scope when
// JNI_OnUnload is called.
//
// For any new thread spawned, a ThreadGuard must be held.  The caller is
// responsible for ensuring all ThreadGuards fall from scope before JvmRef falls
// from scope.
//
// The caller is also responsible for thread safety of all objects that have
// been built during the lifetime of the JVM.  i.e If any objects have been
// built, they must fall from scope prior to JvmRef falling from scope.
//
// There should only be one instance of JvmRef created at a time. If the
// lifetimes of multiple JvmRef overlap, then one instance may get an invalid
// JavaVM after the first instance is destroyed.
template <const auto& jvm_v_>
class JvmRef : public JvmRefBase {
 public:
  template <size_t ClassLoaderIdx>
  struct TeardownClassesHelper {
    template <size_t... Is>
    static constexpr void TeardownClass(
        std::index_sequence<Is...> index_sequence) {
      (ClassRef_t<JniT<jobject, kNoClassSpecified, kDefaultClassLoader, jvm_v_,
                       0, Is, ClassLoaderIdx>

                  >::MaybeReleaseClassRef(),
       ...);
    }
  };

  template <size_t... Is>
  constexpr void TeardownClassloadersHelper(
      std::index_sequence<Is...> index_sequence) {
    (TeardownClassesHelper<Is>::TeardownClass(
         std::make_index_sequence<
             std::tuple_size_v<decltype(std::get<Is>(jvm_v_.class_loaders_)
                                            .supported_classes_)>>()),
     ...);
  }

  JavaVM* BuildJavaVMFromEnv(JNIEnv* env) {
    JavaVM* vm = nullptr;
    // 0 Is success.
    if (env->GetJavaVM(&vm) == 0) {
      return vm;
    } else {
      return nullptr;
    }
  }

  explicit JvmRef(JNIEnv* env) : JvmRefBase(BuildJavaVMFromEnv(env)) {}
  explicit JvmRef(JavaVM* vm) : JvmRefBase(vm) {}

  ~JvmRef() {
    TeardownClassloadersHelper(
        std::make_index_sequence<
            std::tuple_size_v<decltype(jvm_v_.class_loaders_)>>());

    // This object has two lifecycle phases in relation to data races
    // 1)  Value is null, when it is guarded by the ClassRef mutex
    //     (implicitly part of ClassRef's behaviour).
    // 2)  JVM is tearing down.  At this point, the caller is responsible for
    //     releasing all native resources.
    //     ReleaseAllClassRefsForDefaultClassLoader will only ever be torn down
    //     by JvmRef::~JvmRef, and JvmRef cannot be moved, therefore it is
    //     guaranteed to be in a single threaded context.
    auto& default_loaded_class_list = GetDefaultLoadedClassList();
    for (metaprogramming::DoubleLockedValue<jclass>* maybe_loaded_class_id :
         default_loaded_class_list) {
      maybe_loaded_class_id->Reset([](jclass clazz) {
        LifecycleHelper<jobject, LifecycleType::GLOBAL>::Delete(clazz);
      });
    }
    default_loaded_class_list.clear();

    // Methods do not need to be released, just forgotten.
    auto& default_loaded_method_ref_list = GetDefaultLoadedMethodList();
    for (metaprogramming::DoubleLockedValue<jmethodID>* cached_method_id :
         default_loaded_method_ref_list) {
      cached_method_id->Reset();
    }
    default_loaded_method_ref_list.clear();

    // Fields do not need to be released, just forgotten.
    auto& default_loaded_field_ref_list = GetDefaultLoadedFieldList();
    for (metaprogramming::DoubleLockedValue<jfieldID>* cached_field_id :
         default_loaded_field_ref_list) {
      cached_field_id->Reset();
    }
    default_loaded_field_ref_list.clear();
  }

  // Deleted in order to make various threading guarantees (see class_ref.h).
  JvmRef(const JvmRef&) = delete;
  JvmRef(JvmRef&&) = delete;

  // All new threads MUST create a guard by calling |BuildThreadGuard|.
  // If a JNIEnv does not exist, this will DetachCurrentThread when done.
  [[nodiscard]] ThreadGuard BuildThreadGuard() const { return {}; }

 private:
  // Main thread has a JNIEnv just like every other thread.
  const ThreadGuard thread_guard_ = {};
};

JvmRef(JNIEnv*) -> JvmRef<kDefaultJvm>;
JvmRef(JavaVM*) -> JvmRef<kDefaultJvm>;

}  // namespace jni

#endif  // JNI_BIND_JVM_REF_H_
