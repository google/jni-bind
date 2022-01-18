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

#ifndef JNI_BIND_LOCAL_STRING_H_
#define JNI_BIND_LOCAL_STRING_H_

#include "ref_base.h"
#include "string.h"
#include "implementation/jni_helper/jni_helper.h"
#include "jni_dep.h"

namespace jni {

// Represents and possibly builds a runtime Java String object.
//
// In order to use a string in memory (as opposed to only using it for function
// arguments), "Pin" the string.
//
// Like jobjects, jstrings can be either local or global with the same ownership
// semantics.
class LocalString : public StringRefBase<LocalString> {
 public:
  friend class StringRefBase<LocalString>;

  LocalString(jobject java_string_as_object)
      : StringRefBase<LocalString>(
            static_cast<jstring>(java_string_as_object)) {}

  LocalString(std::string_view str)
      : LocalString(JniHelper::NewLocalString(str.data())) {}

  explicit LocalString(RefBaseTag<jstring> &&rhs)
      : LocalString(static_cast<jstring>(rhs.Release())) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {*RefBaseTag<jstring>::object_ref_}; }

 private:
  // Invoked through CRTP on dtor.
  void ClassSpecificDeleteObjectRef(jstring object_ref) {
    JniHelper::DeleteLocalObject(object_ref);
  }
};

}  // namespace jni

#endif  // JNI_BIND_LOCAL_STRING_H_
