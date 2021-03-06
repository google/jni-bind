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

#include "local_string.h"
#include "ref_base.h"
#include "string.h"
#include "jni_dep.h"

namespace jni {

class GlobalString : public StringRefBase<GlobalString> {
 public:
  using ClassT = decltype(kJavaLangString);
  friend class StringRefBase<GlobalString>;

  GlobalString(GlobalString &&local_string)
      : StringRefBase<GlobalString>(
            JniHelper::PromoteLocalToGlobalString(local_string.Release())) {}

  GlobalString(std::string_view str)
      : StringRefBase<GlobalString>(JniHelper::PromoteLocalToGlobalString(
            JniHelper::NewLocalString(str.data()))) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {*RefBaseTag<jstring>::object_ref_}; }

 private:
  // Invoked through CRTP on dtor.
  void ClassSpecificDeleteObjectRef(jstring object_ref) {
    JniHelper::DeleteGlobalString(object_ref);
  }
};

}  // namespace jni

#endif  // JNI_BIND_GLOBAL_STRING_H_
