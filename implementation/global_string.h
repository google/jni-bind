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

#include "implementation/global_object.h"
#include "implementation/local_string.h"
#include "implementation/promotion_mechanics.h"
#include "implementation/ref_base.h"
#include "implementation/string_ref.h"
#include "jni_dep.h"

namespace jni {

class GlobalString : public StringRefBase<GlobalString> {
 public:
  using Base = StringRefBase<GlobalString>;
  using Base::Base;
  friend class StringRefBase<GlobalString>;

  // "Copy" constructor, creates new reference (standard).
  GlobalString(CreateCopy, jstring object)
      : Base(static_cast<jstring>(
            JniHelper::NewGlobalRef(static_cast<jobject>(object)))) {}

  // "Copy" constructor, creates new reference (standard).
  GlobalString(CreateCopy, jobject object)
      : Base(static_cast<jstring>(JniHelper::NewGlobalRef(object))) {}

  // "Promote" constructor, creates a new global, frees passed arg (standard).
  GlobalString(PromoteToGlobal, jstring object)
      : Base(JniHelper::PromoteLocalToGlobalString(object)) {}

  // "Promote" constructor, creates a new global, frees passed arg (standard).
  GlobalString(PromoteToGlobal, jobject object)
      : Base(JniHelper::PromoteLocalToGlobalString(
            static_cast<jstring>(object))) {}

  // "Adopts" a global by wrapping a jobject (non-standard).
  GlobalString(AdoptGlobal, jstring object) : Base(object) {}

  // "Adopts" a global by wrapping a jstring (non-standard).
  GlobalString(AdoptGlobal, jobject object)
      : Base(static_cast<jstring>(object)) {}

  GlobalString(GlobalObject<kJavaLangString, kDefaultClassLoader, kDefaultJvm>
                   &&global_string)
      : Base(static_cast<jstring>(global_string.Release())) {}

  GlobalString(LocalString &&local_string)
      : Base(JniHelper::PromoteLocalToGlobalString(local_string.Release())) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {RefBaseTag<jstring>::object_ref_}; }

 private:
  // Construction from jstring requires |PromoteToGlobal| or |AdoptGlobal|.
  explicit GlobalString(jstring obj) : StringRefBase(obj) {}

  // Construction from jstring requires |PromoteToGlobal| or |AdoptGlobal|.
  explicit GlobalString(jobject obj)
      : StringRefBase(static_cast<jstring>(obj)) {}

  // Invoked through CRTP on dtor.
  void ClassSpecificDeleteObjectRef(jstring object_ref) {
    JniHelper::DeleteGlobalString(object_ref);
  }
};

}  // namespace jni

#endif  // JNI_BIND_GLOBAL_STRING_H_
