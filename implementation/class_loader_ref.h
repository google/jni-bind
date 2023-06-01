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

#include "class_defs/java_lang_classes.h"
#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/global_object.h"
#include "implementation/id.h"
#include "implementation/jni_helper/jni_env.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/jvm_ref.h"
#include "implementation/local_object.h"
#include "implementation/local_string.h"
#include "implementation/method.h"
#include "implementation/method_ref.h"
#include "implementation/object_ref.h"
#include "implementation/params.h"
#include "implementation/return.h"
#include "jni_dep.h"
#include "metaprogramming/contains.h"
#include "metaprogramming/queryable_map.h"

namespace jni {

template <LifecycleType lifecycleType>
using ClassLoaderImpl =
    Scoped<lifecycleType, JniT<jobject, kJavaLangClassLoader>, jobject>;

template <LifecycleType lifecycleType, const auto& class_loader_v_,
          const auto& jvm_v_>
class ClassLoaderRef : public ClassLoaderImpl<lifecycleType> {
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
  using Base = ClassLoaderImpl<lifecycleType>;
  using Base::Base;

  static_assert(class_loader_v_ != kDefaultClassLoader,
                "Custom class loaders should not use the default class loader,"
                "objects will automatically use the default.");

  template <const auto& class_v, typename... Params>
  [[nodiscard]] auto BuildLocalObject(Params&&... params) {
    using JniClassT = JniT<jobject, class_v>;
    using IdClassT = Id<JniClassT, IdType::CLASS>;
    static_assert(
        !(ParentLoaderForClass<class_loader_v_, class_v>() == kNullClassLoader),
        "Cannot build this class with this loader.");

    if constexpr (ParentLoaderForClass<class_loader_v_, class_v>() !=
                  kDefaultClassLoader) {
      ClassRef_t<JniT<jobject, class_v, class_loader_v_, jvm_v_,
                      0>>::PrimeJClassFromClassLoader([=]() {
        // Prevent the object (which is a runtime instance of a class) from
        // falling out of scope so it is not released.
        LocalObject loaded_class =
            (*this)("loadClass", IdClassT::kNameUsingDots);

        // We only want to create global references if we are actually going
        // to use them so that they do not leak.
        jclass test_class{
            static_cast<jclass>(static_cast<jobject>(loaded_class))};
        return static_cast<jclass>(JniEnv::GetEnv()->NewGlobalRef(test_class));
      });
    }
    return LocalObject<class_v,
                       ParentLoaderForClass<class_loader_v_, class_v>(),
                       JvmForLoader<class_v>()>{
        std::forward<Params>(params)...};
  }

  template <const auto& class_v, typename... Params>
  [[nodiscard]] auto BuildGlobalObject(Params&&... params) {
    LocalObject obj =
        BuildLocalObject<class_v>(std::forward<Params>(params)...);
    jobject promoted_local =
        LifecycleHelper<jobject, LifecycleType::GLOBAL>::Promote(obj.Release());

    return GlobalObject<class_v,
                        ParentLoaderForClass<class_loader_v_, class_v>(),
                        JvmForLoader<class_v>()>{AdoptGlobal{}, promoted_local};
  }
};

}  // namespace jni

#endif  // JNI_BIND_CLASS_LOADER_REF_H_
