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

#include "base/logging.h"
#include "class_defs/java_lang_classes.h"
#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/default_class_loader.h"
#include "implementation/jni_helper/invoke.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/method.h"
#include "jni_dep.h"
#include "metaprogramming/double_locked_value.h"

namespace jni {

// See JvmRef::~JvmRef.
static std::vector<metaprogramming::DoubleLockedValue<jclass>*>&
GetDefaultLoadedClassList() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<jclass>*>{};
  return *ret_val;
}

static inline jclass LoadClassFromObject(const char* name, jobject object_ref);

template <const auto& jvm>
metaprogramming::DoubleLockedValue<jclass> g_class_ref_;

template <typename JniT>
class ClassRef {
 public:
  template <typename Lambda>
  static void PrimeJClassFromClassLoader(Lambda lambda) {
    class_ref_.LoadAndMaybeInit(lambda);
  }

  static void MaybeReleaseClassRef() {
    LOG(ERROR) << "jif MaybeReleaseClassRef " << JniT::GetClass().name_
               << " , zif class_ref_ (want same) " << &class_ref_
               << " , zif kClassIdx << " << JniT::kClassIdx
               << " , zif kClassLoaderIdx << " << JniT::kClassLoaderIdx
               << " , jvm " << &JniT::jvm_v;

    class_ref_.Reset([](jclass maybe_loaded_class) {
      LOG(ERROR) << "jif MaybeReleaseClassRef 2 YES YES YES ; "
                 << maybe_loaded_class;
      JniHelper::ReleaseClass(maybe_loaded_class);
    });
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
      jobject optional_object_to_build_loader_from = nullptr) {
    LOG(ERROR) << "jif GetAndMaybeLoadClassRef " << JniT::GetClass().name_
               << " , zif class_ref_ (want same) " << &class_ref_
               << " , zif kClassIdx << " << JniT::kClassIdx
               << " , zif kClassLoaderIdx << " << JniT::kClassLoaderIdx
               << " , jvm " << &JniT::jvm_v << "(default ? "
               << (JniT::jvm_v == kDefaultJvm);

    if constexpr (JniT::class_loader_v == kDefaultClassLoader &&
                  JniT::jvm_v == kDefaultJvm) {
      // static_assert(JniT::kIdx == kNoIdx);
      static_assert(JniT::kClassIdx == kNoIdx);

      static metaprogramming::DoubleLockedValue<jclass> return_value;
      return return_value.LoadAndMaybeInit([]() {
        GetDefaultLoadedClassList().push_back(&return_value);
        return JniHelper::PromoteLocalToGlobalClass(
            JniHelper::FindClass(JniT::class_v.name_));
      });
    } else {
      // static_assert(std::is_same_v<JniT, void>, "jif adsfa");
      return class_ref_.LoadAndMaybeInit([=]() {
        return LoadClassFromObject(JniT::GetClass().name_,
                                   optional_object_to_build_loader_from);
      });
    }
  }

 private:
  // A global reference to a jclass object that is returned from FindClass.
  // The variable has static storage because ClassIDs are static to the lifetime
  // of a JVM.  See GetAndMaybeLoadClassRef and MaybeReleaseClassRef.
  static inline metaprogramming::DoubleLockedValue<jclass> class_ref_;
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
      ClassRef<JniT<jobject, kJavaLangClass, kDefaultClassLoader, kDefaultJvm,
                    1> >::GetAndMaybeLoadClassRef();

  jclass java_lang_class_loader_jclass =
      ClassRef<JniT<jobject, kJavaLangClassLoader, kDefaultClassLoader,
                    kDefaultJvm, 1> >::GetAndMaybeLoadClassRef();

  jclass class_of_object_jclass = JniHelper::GetObjectClass(object_ref);

  jmethodID get_class_loader_jmethod = JniHelper::GetMethodID(
      java_lang_class_jclass, "getClassLoader", "()Ljava/lang/ClassLoader;");

  jobject object_ref_class_loader_jobject =
      InvokeHelper<jobject, 1, false>::Invoke(class_of_object_jclass, nullptr,
                                              get_class_loader_jmethod);

  jmethodID load_class_jmethod =
      JniHelper::GetMethodID(java_lang_class_loader_jclass, "loadClass",
                             "(Ljava/lang/String;)Ljava/lang/Class;");

  jstring name_string = JniHelper::NewLocalString(name);
  jobject local_jclass_of_correct_loader =
      InvokeHelper<jobject, 1, false>::Invoke(object_ref_class_loader_jobject,
                                              nullptr, load_class_jmethod,
                                              name_string);
  jobject promote_jclass_of_correct_loader =
      JniHelper::PromoteLocalToGlobalObject(local_jclass_of_correct_loader);

  JniHelper::DeleteLocalObject(object_ref_class_loader_jobject);

  return static_cast<jclass>(promote_jclass_of_correct_loader);
}

// Primes a JniType with index data for class (if applicable).
// This logic can eventually be shifted into JniT if needed.
//
//
// jclass IDs are queried through FindClass for default class loaded
// objects, but derived from class loaders otherwise.  There are 4 options:
//
//   1) Default JVM, default class loader.
//   2) Non-default JVM, default class loader.
//   3) Default JVM, non-default class loader.
//   4) Non-default JVM, non default class loader.
template <typename JniT_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
struct ClassRefSelector {
  static_assert(std::is_same_v<JniT_, void>, "jif");
  using type =
      ClassRef<JniT<jobject, kNoClassSpecified, kDefaultClassLoader, jvm_v_, 0,
                    class_loader_v_.template IdxOfClass<class_v_>(),
                    jvm_v_.template IdxOfClassLoader<class_loader_v_>()> >;
};

template <typename JniT, const auto& class_v_, const auto& class_loader_v_>
struct ClassRefSelector<JniT, class_v_, class_loader_v_, kDefaultJvm> {
  using type = ClassRef<JniT>;
};

template <typename JniT, const auto& class_v_, const auto& jvm_v_>
struct ClassRefSelector<JniT, class_v_, kDefaultClassLoader, jvm_v_> {
  using type = ClassRef<JniT>;
};

template <typename JniT, const auto& class_v_>
struct ClassRefSelector<JniT, class_v_, kDefaultClassLoader, kDefaultJvm> {
  using type = ClassRef<JniT>;
};

template <typename JniT>
using ClassRef_t =
    // typename ClassRefSelector<JniT, JniT::class_v, JniT::class_loader_v,
    //                          JniT::jvm_v>::type;
    ClassRef<JniT>;

}  // namespace jni

#endif  // JNI_BIND_CLASS_REF_H_
