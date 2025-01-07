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

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <memory>
#include <utility>

#include "class_defs/java_lang_classes.h"
#include "implementation/class_ref.h"
#include "implementation/configuration.h"
#include "implementation/default_class_loader.h"
#include "implementation/field_ref.h"
#include "implementation/forward_declarations.h"
#include "implementation/global_class_loader.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jni_helper/lifecycle.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/jvm_ref_base.h"
#include "implementation/no_class_specified.h"
#include "implementation/promotion_mechanics_tags.h"
#include "implementation/ref_storage.h"
#include "implementation/thread_guard.h"
#include "jni_dep.h"
#include "metaprogramming/double_locked_value.h"

namespace jni {

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

  explicit JvmRef(JNIEnv* env, const Configuration& configuration = {})
      : JvmRefBase(BuildJavaVMFromEnv(env), configuration) {}
  explicit JvmRef(JavaVM* vm, const Configuration& configuration = {})
      : JvmRefBase(vm, configuration) {}

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
    auto& default_loaded_class_list = DefaultRefs<jclass>();
    for (metaprogramming::DoubleLockedValue<jclass>* maybe_loaded_class_id :
         default_loaded_class_list) {
      maybe_loaded_class_id->Reset([](jclass clazz) {
        LifecycleHelper<jobject, LifecycleType::GLOBAL>::Delete(clazz);
      });
    }
    default_loaded_class_list.clear();

    // Methods do not need to be released, just forgotten.
    auto& default_loaded_method_ref_list = DefaultRefs<jmethodID>();
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

  // Sets a "fallback" loader for use when default Jvm classes fail to load.
  // This is useful for first use of classes on secondary threads where the
  // jclass is not yet cached and the classloader isn't available directly.
  void SetFallbackClassLoader(
      jni::GlobalClassLoader<kDefaultClassLoader, kDefaultJvm>&& loader) {
    fallback_loader_.reset(
        new jni::GlobalClassLoader<kDefaultClassLoader, kDefaultJvm>(
            AdoptGlobal{}, loader.Release()));

    FallbackLoader() = static_cast<jobject>(*fallback_loader_);
  }

  // Sets a "fallback" loader for use when default Jvm classes fail to load.
  // `host_object *must* be local and will *not* be released.
  void SetFallbackClassLoaderFromJObject(jobject host_object) {
#if __cplusplus >= 202002L
    SetFallbackClassLoader(LocalObject<kJavaLangObject>{host_object}
                               .Call<"getClass">()
                               .Call<"getClassLoader">());
#elif __clang__
    SetFallbackClassLoader(LocalObject<kJavaLangObject>{host_object}(
        "getClass")("getClassLoader"));
#else
    static_assert(false,
                  "JNI Bind requires C++20 (or later) or C++17 with clang.");
#endif
  }

 private:
  // Main thread has a JNIEnv just like every other thread.
  const ThreadGuard thread_guard_ = {};

  std::unique_ptr<jni::GlobalClassLoader<kDefaultClassLoader, kDefaultJvm>>
      fallback_loader_;
};

JvmRef(JNIEnv*) -> JvmRef<kDefaultJvm>;
JvmRef(JavaVM*) -> JvmRef<kDefaultJvm>;

}  // namespace jni

#endif  // JNI_BIND_JVM_REF_H_
