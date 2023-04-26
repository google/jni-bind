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

#include "class_defs/java_lang_classes.h"
#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/default_class_loader.h"
#include "implementation/jni_helper/invoke.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_helper/lifecycle_string.h"
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

// Represents a a jclass instance for a specific class. 4 flavours exist:
//   1) Default JVM, default class loader.
//   2) Non-default JVM, default class loader.
//   3) Default JVM, non-default class loader.
//   4) Non-default JVM, non default class loader (i.e. fully specified).
//
// Use |ClassRef_t| to provide |JniT| in its minimal form.
template <typename JniT>
class ClassRef {
 public:
  static_assert(std::is_same_v<JniT, typename JniT::MinimallySpanningType>,
                "JniT must be in its minimal form for best caching.");

  template <typename Lambda>
  static void PrimeJClassFromClassLoader(Lambda lambda) {
    class_ref_.LoadAndMaybeInit(lambda);
  }

  static jclass GetAndMaybeLoadClassRef(
      jobject optional_object_to_build_loader_from) {
    // For the default classloader, storage in function local static.
    if constexpr (JniT::GetClassLoader() == kDefaultClassLoader) {
      static metaprogramming::DoubleLockedValue<jclass> return_value;
      return return_value.LoadAndMaybeInit([]() {
        GetDefaultLoadedClassList().push_back(&return_value);

        return static_cast<jclass>(
            LifecycleHelper<jobject, LifecycleType::GLOBAL>::Promote(
                JniHelper::FindClass(JniT::kName.data())));
      });
    } else {
      // For non default classloader, storage in class member.
      return class_ref_.LoadAndMaybeInit([=]() {
        return LoadClassFromObject(JniT::kNameWithDots.data(),
                                   optional_object_to_build_loader_from);
      });
    }
  }

  static jclass GetAlreadyLoadedClassRef() {
    return class_ref_.LoadAndMaybeInit([]() { return jclass{0}; });
  }

  static void MaybeReleaseClassRef() {
    class_ref_.Reset([](jclass maybe_loaded_class) {
      LifecycleHelper<jclass, LifecycleType::GLOBAL>::Delete(
          maybe_loaded_class);
    });
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
      ClassRef<JniT<jobject, kJavaLangClass>>::GetAndMaybeLoadClassRef(nullptr);

  jclass java_lang_class_loader_jclass =
      ClassRef<JniT<jobject, kJavaLangClassLoader>>::GetAndMaybeLoadClassRef(
          nullptr);

  jclass class_of_object_jclass = JniHelper::GetObjectClass(object_ref);

  jmethodID get_class_loader_jmethod = JniHelper::GetMethodID(
      java_lang_class_jclass, "getClassLoader", "()Ljava/lang/ClassLoader;");

  jobject object_ref_class_loader_jobject =
      InvokeHelper<jobject, 1, false>::Invoke(class_of_object_jclass, nullptr,
                                              get_class_loader_jmethod);

  jmethodID load_class_jmethod =
      JniHelper::GetMethodID(java_lang_class_loader_jclass, "loadClass",
                             "(Ljava/lang/String;)Ljava/lang/Class;");

  jstring name_string =
      LifecycleHelper<jstring, LifecycleType::LOCAL>::Construct(name);
  jobject local_jclass_of_correct_loader =
      InvokeHelper<jobject, 1, false>::Invoke(object_ref_class_loader_jobject,
                                              nullptr, load_class_jmethod,
                                              name_string);
  jobject promote_jclass_of_correct_loader =
      LifecycleHelper<jobject, LifecycleType::GLOBAL>::Promote(
          local_jclass_of_correct_loader);

  LifecycleHelper<jobject, LifecycleType::LOCAL>::Delete(
      object_ref_class_loader_jobject);

  return static_cast<jclass>(promote_jclass_of_correct_loader);
}

template <typename JniT>
using ClassRef_t = ClassRef<typename JniT::MinimallySpanningType>;

}  // namespace jni

#endif  // JNI_BIND_CLASS_REF_H_
