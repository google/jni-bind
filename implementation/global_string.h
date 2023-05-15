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

#ifndef JNI_BIND_GLOBAL_STRING_H_
#define JNI_BIND_GLOBAL_STRING_H_

#include "class_defs/java_lang_classes.h"
#include "implementation/global_object.h"
#include "implementation/jni_helper/lifecycle_string.h"
#include "implementation/local_string.h"
#include "implementation/promotion_mechanics.h"
#include "implementation/ref_base.h"
#include "implementation/string_ref.h"
#include "jni_dep.h"

namespace jni {

class GlobalString
    : public GlobalCtor<
          GlobalString, StringRefBase<GlobalString>,
          JniT<jstring, kJavaLangString, kDefaultClassLoader, kDefaultJvm>,
          jobject, jstring> {
 public:
  friend class StringRefBase<GlobalString>;

  using Base = GlobalCtor<
      GlobalString, StringRefBase<GlobalString>,
      JniT<jstring, kJavaLangString, kDefaultClassLoader, kDefaultJvm>, jobject,
      jstring>;
  using Base::Base;

  using LifecycleT = LifecycleHelper<jstring, LifecycleType::GLOBAL>;

  GlobalString(GlobalObject<kJavaLangString, kDefaultClassLoader, kDefaultJvm>
                   &&global_string)
      : Base(static_cast<jstring>(global_string.Release())) {}

  GlobalString(LocalString &&local_string)
      : Base(LifecycleT::Promote(local_string.Release())) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {RefBaseTag<jstring>::object_ref_}; }

 private:
  // Construction from jstring requires |PromoteToGlobal| or |AdoptGlobal|.
  explicit GlobalString(jstring obj) : Base(obj) {}

  // Construction from jstring requires |PromoteToGlobal| or |AdoptGlobal|.
  explicit GlobalString(jobject obj) : Base(static_cast<jstring>(obj)) {}

  // Invoked through CRTP on dtor.
  void ClassSpecificDeleteObjectRef(jstring object_ref) {
    LifecycleT::Delete(object_ref);
  }
};

}  // namespace jni

#endif  // JNI_BIND_GLOBAL_STRING_H_
