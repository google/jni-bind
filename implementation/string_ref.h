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

#ifndef JNI_BIND_STRING_REF_H_
#define JNI_BIND_STRING_REF_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <string_view>

#include "jni_helper/jni_helper.h"
#include "class_defs/java_lang_classes.h"
#include "implementation/default_class_loader.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/object_ref.h"
#include "jni_dep.h"

namespace jni {

template <typename CrtpBase>
class StringRefBase
    : public ConstructorValidator<
          JniT<jstring, kJavaLangString, kDefaultClassLoader, kDefaultJvm>> {
 public:
  using ValidT = ConstructorValidator<
      JniT<jstring, kJavaLangString, kDefaultClassLoader, kDefaultJvm>>;
  using ValidT::ValidT;

  StringRefBase(std::nullptr_t) : ValidT(jstring{nullptr}) {}
  StringRefBase(jstring object) : ValidT(object) {}

  ~StringRefBase() {
    if (object_ref_) {
      static_cast<CrtpBase &>(*this).ClassSpecificDeleteObjectRef(object_ref_);
    }
  }
};

// Represents a UTF view into a jstring (see jni::String::Pin()).
//
// This class will immediately pin memory associated with the jstring, and
// release on leaving scope. Note, this class will *always* make an expensive
// copy, as strings are natively represented in Java as Unicode.
//
// (C++20 will offer a compatible std::string_view but C++17 does not).
class UtfStringView {
 public:
  UtfStringView(jstring java_string)
      : java_string_(java_string),
        chars_(java_string_ ? JniHelper::GetStringUTFChars(java_string)
                            : nullptr) {}

  ~UtfStringView() {
    if (chars_) {
      JniHelper::ReleaseStringUTFChars(java_string_, chars_);
    }
  }

  UtfStringView(UtfStringView &&) = delete;
  UtfStringView(const UtfStringView &) = delete;

  // Returns a view into the pinned character string.
  // Warning: std::string_view{nullptr} is undefined behaviour and may crash.
  const std::string_view ToString() const { return std::string_view{chars_}; }

 private:
  const jstring java_string_;
  const char *chars_;
};

}  // namespace jni

#endif  // JNI_BIND_STRING_REF_H_
