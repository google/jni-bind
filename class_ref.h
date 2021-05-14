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

#ifndef JNI_BIND_CLASS_REF_H_
#define JNI_BIND_CLASS_REF_H_

#include <atomic>
#include <mutex>  // NOLINT
#include <vector>

#include "class.h"
#include "class_defs/java_lang_classes.h"
#include "class_loader.h"
#include "jni_dep.h"
#include "jni_helper/jni_helper.h"
#include "jni_helper/jni_method_invoke.h"
#include "jvm.h"
#include "metaprogramming/double_locked_value.h"
#include "method.h"

namespace jni {

// See JvmRef::~JvmRef.
static std::vector<metaprogramming::DoubleLockedValue<jclass>*>&
GetDefaultLoadedClassList() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<jclass>*>{};
  return *ret_val;
}

static inline jclass LoadClassFromObject(const char* name, jobject object_ref);

// Represents a fully specified class and class loader pair for a given Jvm.
// Because this is fully specified, classes associated with this type of loader
// can be programmatically torn down.
template <const auto& jvm_v_, size_t class_loader_idx_, size_t class_idx_>
class ClassRef {
 public:
  static_assert(kDefaultJvm != jvm_v_, "For default Jvm use DefaultClassRef.");

  static const auto& GetClassLoader() {
    return std::get<class_loader_idx_>(jvm_v_.class_loaders_);
  }

  static const auto& GetClass() {
    return std::get<class_idx_>(GetClassLoader().supported_classes_);
  }

  static void PrimeJClassFromClassLoader(jclass class_val) {
    class_ref_.LoadAndMaybeInit([=]() { return class_val; });
  }

  static jclass GetAndMaybeLoadClassRef(
      jobject optional_object_to_build_loader_from) {
    return class_ref_.LoadAndMaybeInit([=]() {
      return LoadClassFromObject(GetClass().name_,
                                 optional_object_to_build_loader_from);
    });
  }

  static jclass GetAlreadyLoadedClassRef() {
    return class_ref_.LoadAndMaybeInit([]() { return jclass{0}; });
  }

  static void MaybeReleaseClassRef() {
    class_ref_.Reset([](jclass maybe_loaded_class) {
      JniHelper::ReleaseClass(maybe_loaded_class);
    });
  }

 private:
  // A global reference to a jclass object that is returned from FindClass.
  // The variable has static storage because ClassIDs are static to the lifetime
  // of a JVM.  See GetAndMaybeLoadClassRef and MaybeReleaseClassRef.
  static inline metaprogramming::DoubleLockedValue<jclass> class_ref_;
};

template <const auto& class_loader_v, const auto& class_v>
class DefaultClassRef {
 public:
  static void PrimeJClassFromClassLoader(jclass class_val) {
    class_ref_.LoadAndMaybeInit([=]() { return class_val; });
  }

  // Gets the class ref for class_v, possibly loading it in the process.
  // Note, all class refs are pinned with GlobalRef and will be released
  // when JvmRef is torn down.
  //
  // This function is threadsafe, although the result is immediately stale.
  // This will never matter however, as part of the API's contract is call
  // Jvm::Destroy on all threads and release all objects in order to transition
  // to a new JVM.
  static jclass GetAndMaybeLoadClassRef(
      jobject optional_object_to_build_loader_from) {
    return class_ref_.LoadAndMaybeInit([=]() {
      GetDefaultLoadedClassList().push_back(&class_ref_);
      return LoadClassFromObject(class_v.name_,
                                 optional_object_to_build_loader_from);
    });
  }

 private:
  // A global reference to a jclass object that is returned from FindClass.
  // The variable has static storage because ClassIDs are static to the lifetime
  // of a JVM.  See GetAndMaybeLoadClassRef and MaybeReleaseClassRef.
  static inline metaprogramming::DoubleLockedValue<jclass> class_ref_;
};

// Default class loader, default Jvm.
template <const auto& class_v>
class DefaultClassRef<kDefaultClassLoader, class_v> {
 public:
  static jclass GetAndMaybeLoadClassRef(jobject /*unused*/ = nullptr) {
    static metaprogramming::DoubleLockedValue<jclass> return_value;
    return return_value.LoadAndMaybeInit([]() {
      // (b/174287131): This can be removed, but tests must be updated as they
      // guarantee all globals must be unpinned which this will break.
      GetDefaultLoadedClassList().push_back(&return_value);
      return JniHelper::PromoteLocalToGlobalClass(
          JniHelper::FindClass(class_v.name_));
    });
  }
};

// When we get an object_ref_ as a return value from a Java method, it may be
// an instance of a subclass of ClassRefT. In this case, if we directly used
// the object_ref_'s class, then we might incorrectly get member information
// for the subclass instead of the original class. However, the original class
// should still be loadable from the subclass's class loader, so we load the
// ClassRef explicitly by class name.
static inline jclass LoadClassFromObject(const char* name, jobject object_ref) {
  // We cannot refer to the wrapper MethodRefs here, so we just manually use
  // the class loader through JNI.

  // Gets the ClassLoader of java/lang/class (the primordial loader).
  // Note, these aren't static methods, they're member methods to be invoked
  // on the object's class itself.  The class may not have been loaded yet,
  // and all you have is a jobject
  jclass java_lang_class_jclass =
      DefaultClassRef<kDefaultClassLoader,
                      kJavaLangClass>::GetAndMaybeLoadClassRef();

  jclass java_lang_class_loader_jclass =
      DefaultClassRef<kDefaultClassLoader,
                      kJavaLangClassLoader>::GetAndMaybeLoadClassRef();

  jclass class_of_object_jclass = JniHelper::GetObjectClass(object_ref);

  jmethodID get_class_loader_jmethod = JniHelper::GetMethodID(
      java_lang_class_jclass, "getClassLoader", "()Ljava/lang/ClassLoader;");

  jobject object_ref_class_loader_jobject = JniMethodInvoke<jobject>::Invoke(
      class_of_object_jclass, get_class_loader_jmethod);

  jmethodID load_class_jmethod =
      JniHelper::GetMethodID(java_lang_class_loader_jclass, "loadClass",
                             "(Ljava/lang/String;)Ljava/lang/Class;");

  jstring name_string = JniHelper::NewLocalString(name);
  jobject local_jclass_of_correct_loader = JniMethodInvoke<jobject>::Invoke(
      object_ref_class_loader_jobject, load_class_jmethod, name_string);
  jobject promote_jclass_of_correct_loader =
      JniHelper::PromoteLocalToGlobalObject(local_jclass_of_correct_loader);

  JniHelper::DeleteLocalObject(object_ref_class_loader_jobject);

  return static_cast<jclass>(promote_jclass_of_correct_loader);
}

// Selects among either |ClassRef| (which can be reset) or |DefaultClassRef|
// (which supports only Jvm during it's lifetime).
//
// jclass IDs are queried through FindClass for default class loaded
// objects, but derived from class loaders otherwise.  There are 4 options:
//
//   1) Default JVM, default class loader.
//   2) Non-default JVM, default class loader.
//   3) Default JVM, non-default class loader.
//   4) Non-default JVM, non default class loader.
template <const auto& jvm_v_, const auto& class_loader_v_, const auto& class_v_>
struct ClassRefSelector {
  using type =
      ClassRef<jvm_v_, jvm_v_.template IdxOfClassLoader<class_loader_v_>(),
               class_loader_v_.template IdxOfClass<class_v_>()>;
};

template <const auto& class_loader_v_, const auto& class_v_>
struct ClassRefSelector<kDefaultJvm, class_loader_v_, class_v_> {
  using type = DefaultClassRef<class_loader_v_, class_v_>;
};

template <const auto& jvm_v_, const auto& class_v_>
struct ClassRefSelector<jvm_v_, kDefaultClassLoader, class_v_> {
  using type = DefaultClassRef<kDefaultClassLoader, class_v_>;
};

template <const auto& class_v_>
struct ClassRefSelector<kDefaultJvm, kDefaultClassLoader, class_v_> {
  using type = DefaultClassRef<kDefaultClassLoader, class_v_>;
};

template <const auto& jvm_v_, const auto& class_loader_v_, const auto& class_v_>
using ClassRef_t =
    typename ClassRefSelector<jvm_v_, class_loader_v_, class_v_>::type;

}  // namespace jni

#endif  // JNI_BIND_CLASS_REF_H_
