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

#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/jni_helper/jni_env.h"

namespace jni {

// Used to detect RefBase in type proxying.
// This is useful, e.g. when you want to say "an object that might be passed"
// but the object's type (i.e. full name + loader information) is unknown.
template <typename StorageType>
class RefBaseTag {
 public:
  RefBaseTag(StorageType object) : object_ref_(object) {}

  RefBaseTag(const RefBaseTag& rhs) = delete;

  RefBaseTag(RefBaseTag&& rhs) {
    object_ref_ = rhs.object_ref_;
    rhs.object_ref_ = nullptr;
  }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, StorageType>>>
  RefBaseTag(RefBaseTag<T>&& rhs) : object_ref_(rhs.Release()) {}

  StorageType Release() {
    StorageType return_value = object_ref_;
    object_ref_ = nullptr;

    return return_value;
  }

  explicit operator StorageType() const { return object_ref_; }

 protected:
  StorageType object_ref_ = nullptr;
};

// Represents a runtime object with only Name information.  It is ephemeral and
// can be used a return value which is immediately wrapped in a rich object type
// (such as LocalObject, LocalString) which has the complete class definition.
//
// This can also be used as a temporary when passed into a function that accepts
// objects.  This ensures type correctness (names must match) but doesn't
// require the full class description be used when describing the function.
template <typename JniT>
class RefBase : public RefBaseTag<typename JniT::StorageType> {
 public:
  using StorageType = typename JniT::StorageType;
  using RefBaseTag<StorageType>::RefBaseTag;
  using RefBaseTag<StorageType>::operator typename JniT::StorageType;

  static inline const char* name_ = JniT::class_v.name_;

  RefBase(const RefBase& rhs) = delete;

  RefBase(RefBaseTag<StorageType>&& rhs)
      : RefBaseTag<StorageType>(std::move(rhs)) {}
};

//==============================================================================
template <typename T>
using RefBaseT_t = typename T::RefBaseT;

}  // namespace jni

#endif  // JNI_BIND_REF_BASE_H_
