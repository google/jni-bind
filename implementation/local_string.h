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

#include "class_defs/java_lang_classes.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jni_helper/lifecycle_string.h"
#include "implementation/local_object.h"
#include "implementation/promotion_mechanics.h"
#include "implementation/ref_base.h"
#include "implementation/string_ref.h"
#include "jni_dep.h"

namespace jni {

using LocalStringImpl =
    Scoped<LifecycleType::LOCAL, JniT<jstring, kJavaLangString>, jobject,
           jstring>;

// Represents and possibly builds a runtime Java String object.
//
// In order to use a string in memory (as opposed to only using it for function
// arguments), "Pin" the string.
//
// Like |jobjects|, |jstring|s can be either local or global with the same
// ownership semantics.
class LocalString : public LocalStringImpl {
 public:
  using Base = LocalStringImpl;
  using Base::Base;

  LocalString(std::nullptr_t) : Base(jstring{nullptr}) {}
  LocalString(LocalObject<kJavaLangString>&& obj)
      : Base(static_cast<jstring>(obj.Release())) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {RefBaseTag<jstring>::object_ref_}; }
};

}  // namespace jni

#endif  // JNI_BIND_LOCAL_STRING_H_
