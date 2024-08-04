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

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <type_traits>

#include "implementation/jni_helper/lifecycle.h"

namespace jni {

struct RefBaseBase {};

// Used to detect RefBase in type proxying.
// This is useful, e.g. when you want to say "an object that might be passed"
// but the object's type (i.e. full name + loader information) is unknown.
template <typename StorageType>
class RefBase : public RefBaseBase {
 public:
  template <typename Base, LifecycleType lifecycleType, typename JniT,
            typename ViableSpan>
  friend struct EntryBase;

  RefBase(StorageType object) : object_ref_(object) {}

  RefBase(const RefBase& rhs) = delete;

  RefBase(RefBase&& rhs) {
    object_ref_ = rhs.object_ref_;
    rhs.object_ref_ = nullptr;
  }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, StorageType>>>
  RefBase(RefBase<T>&& rhs) : object_ref_(rhs.Release()) {}

  StorageType Release() {
    StorageType return_value = object_ref_;
    object_ref_ = nullptr;

    return return_value;
  }

  explicit operator StorageType() const { return object_ref_; }

  // protected:
  StorageType object_ref_ = nullptr;
};

//==============================================================================
template <typename T>
using RefBaseT_t = typename T::RefBaseT;

}  // namespace jni

#endif  // JNI_BIND_REF_BASE_H_
