/*
 * Copyright 2023 Google LLC
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

#ifndef JNI_BIND_LOCAL_ARRAY_STRING_H_
#define JNI_BIND_LOCAL_ARRAY_STRING_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

#include "class_defs/java_lang_classes.h"
#include "implementation/class_ref.h"
#include "implementation/default_class_loader.h"
#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/jni_array_helper.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/local_array.h"
#include "implementation/local_object.h"
#include "implementation/local_string.h"
#include "jni_dep.h"

namespace jni {

template <std::size_t kRank_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
class LocalArray<jstring, kRank_, class_v_, class_loader_v_, jvm_v_>
    : public LocalArray<jobject, kRank_, kJavaLangString, kDefaultClassLoader,
                        kDefaultJvm> {
 public:
  using Base = LocalArray<jobject, kRank_, kJavaLangString, kDefaultClassLoader,
                          kDefaultJvm>;

  using Base::Base;
  using StringJniT =
      JniT<jstring, kJavaLangString, kDefaultClassLoader, kDefaultJvm>;

  LocalArray(std::size_t size)
      : Base(JniArrayHelper<jobject, kRank_>::NewArray(
            size, ClassRef<StringJniT>::GetAndMaybeLoadClassRef(nullptr),
            nullptr)) {}

  // Note: Globals are not permitted in a `LocalArray` because it makes
  // reasoning about them confusing.
  void Set(
      std::size_t idx,
      LocalObject<kJavaLangString, kDefaultClassLoader, kDefaultJvm>&& val) {
    return JniArrayHelper<jobject, kRank_>::SetArrayElement(Base::object_ref_,
                                                            idx, val.Release());
  }

  void Set(std::size_t idx, LocalString&& val) {
    return JniArrayHelper<jobject, kRank_>::SetArrayElement(Base::object_ref_,
                                                            idx, val.Release());
  }

  void Set(std::size_t idx, const char* val) { Set(idx, LocalString{val}); }

  void Set(std::size_t idx, std::string val) {
    Set(idx, LocalString{std::move(val)});
  }

  void Set(std::size_t idx, std::string_view val) {
    Set(idx, LocalString{val});
  }
};

}  // namespace jni

#endif  // JNI_BIND_LOCAL_ARRAY_STRING_H_
