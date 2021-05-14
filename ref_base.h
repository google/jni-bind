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

#ifndef JNI_BIND_REF_BASE_H_
#define JNI_BIND_REF_BASE_H_

#include <optional>

#include "class.h"
#include "class_loader.h"
#include "jni_helper/jni_env.h"

namespace jni {

// Used to detect RefBase in type proxying.
// This is useful, e.g. when you want to say "an object that might be passed"
// but the object's type is unknown.
template <typename NativeJavaType_>
class RefBaseTag {
 public:
  RefBaseTag(NativeJavaType_ object) : object_ref_(object) {}

  RefBaseTag(RefBaseTag&& rhs) : object_ref_(rhs.Release()) {}

  NativeJavaType_ Release() {
    NativeJavaType_ return_value = *object_ref_;
    object_ref_ = std::nullopt;
    return return_value;
  }

  explicit operator NativeJavaType_() {
    if (object_ref_) {
      return *object_ref_;
    }
    return nullptr;
  }

 protected:
  std::optional<NativeJavaType_> object_ref_;
};

// Represents a runtime object with only Name information.  It is ephemeral and
// can be used a return value which is immediately wrapped in a rich object type
// (such as LocalObject, LocalString) which has the complete class definition.
//
// This can also be used as a temporary when passed into a function that accepts
// objects.  This ensures type correctness (names must match) but doesn't
// require the full class description be used when describing the function.
//
// The tag is a way to separate RefBases that can be converted between each
// other. For ObjectRefs, this will be the ClassLoader that loads the object's
// class (not a child loader that is also able to use the class).
template <typename NativeJavaType_, const auto& class_v_ = kNoClassSpecified,
          const auto& loader_v_ = kDefaultClassLoader>
class RefBase : public RefBaseTag<NativeJavaType_> {
 public:
  static inline const char* name_ = class_v_.name_;

  using RefBaseTag<NativeJavaType_>::RefBaseTag;
  using RefBaseTag<NativeJavaType_>::operator NativeJavaType_;

  RefBase(RefBaseTag<NativeJavaType_>&& rhs)
      : RefBaseTag<NativeJavaType_>(std::move(rhs)) {}
  RefBase(RefBase&&) = default;
};

//==============================================================================
template <typename T>
using RefBaseT_t = typename T::RefBaseT;

}  // namespace jni

#endif  // JNI_BIND_REF_BASE_H_
