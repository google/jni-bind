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

#ifndef JNI_BIND_CLASS_LOADER_REF_H_
#define JNI_BIND_CLASS_LOADER_REF_H_

#include <type_traits>

#include "class.h"
#include "class_loader.h"
#include "global_object.h"
#include "local_object.h"
#include "local_string.h"
#include "method.h"
#include "method_ref.h"
#include "object_ref.h"
#include "params.h"
#include "return.h"
#include "class_defs/java_lang_classes.h"
#include "implementation/class_loader.h"
#include "implementation/jni_helper/jni_env.h"
#include "implementation/jvm_ref.h"
#include "jni_dep.h"
#include "metaprogramming/contains.h"
#include "metaprogramming/queryable_map.h"

namespace jni {

template <const auto& jvm_v_, const auto& class_loader_v_>
class ClassLoaderRef
    : public ObjectRef<kDefaultJvm, kJavaLangClassLoader, kDefaultClassLoader> {
 private:
  // Returns kDefaultJvm for default class loaded objects, otherwise returns the
  // jvm associated with this loader.  Default loaders do not use indexing,
  // whereas non-standard loaders do (to allow for programmatic Jvm teardown).
  template <const auto& class_v>
  static constexpr auto& JvmForLoader() {
    if constexpr (ParentLoaderForClass<class_loader_v_, class_v>() !=
                  kDefaultClassLoader) {
      return jvm_v_;
    } else {
      return kDefaultJvm;
    }
  }

 public:
  ClassLoaderRef(jobject class_loader)
      : ObjectRef<kDefaultJvm, kJavaLangClassLoader, kDefaultClassLoader>(
            class_loader) {}

  static_assert(class_loader_v_ != kDefaultClassLoader,
                "Custom class loaders should not use the default class loader,"
                "objects will automatically use the default.");

  template <const auto& class_v, typename... Params>
  [[nodiscard]] auto BuildLocalObject(Params&&... params) {
    static_assert(
        !(ParentLoaderForClass<class_loader_v_, class_v>() == kNullClassLoader),
        "Cannot build this class with this loader.");

    if constexpr (ParentLoaderForClass<class_loader_v_, class_v>() !=
                  kDefaultClassLoader) {
      ClassRef_t<jvm_v_, class_loader_v_, class_v>::PrimeJClassFromClassLoader(
          [=]() {
            // Prevent the object (which is a runtime instance of a class) from
            // falling out of scope so it is not released.
            LocalObject loaded_class = (*this)("loadClass", class_v.name_);

            // We only want to create global references if we are actually going
            // to use it them so that they do not leak.
            jclass test_class{
                static_cast<jclass>(static_cast<jobject>(loaded_class))};
            return static_cast<jclass>(
                JniEnv::GetEnv()->NewGlobalRef(test_class));
          });
    }
    return LocalObject<class_v,
                       ParentLoaderForClass<class_loader_v_, class_v>(),
                       JvmForLoader<class_v>()>{
        std::forward<Params>(params)...};
  }

  template <const auto& class_v, typename... Params>
  [[nodiscard]] auto BuildGlobalObject(Params&&... params) {
    // TODO(b/174256299): Promotion of locals to globals is clumsy.
    LocalObject obj =
        BuildLocalObject<class_v>(std::forward<Params>(params)...);
    jobject promoted_local =
        JniHelper::PromoteLocalToGlobalObject(obj.Release());
    return GlobalObject<class_v,
                        ParentLoaderForClass<class_loader_v_, class_v>(),
                        JvmForLoader<class_v>()>{promoted_local};
  }
};

}  // namespace jni

#endif  // JNI_BIND_CLASS_LOADER_REF_H_
