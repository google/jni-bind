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
#ifndef JNI_BIND_TYPE_FORWARD_DECLARATIONS_H_
#define JNI_BIND_TYPE_FORWARD_DECLARATIONS_H_

#include "implementation/default_class_loader.h"
#include "implementation/id_type.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jvm.h"

namespace jni {

// Id.
template <typename JniT_, IdType kIdType_, std::size_t idx,
          std::size_t secondary_idx, std::size_t tertiary_idx>
struct Id;

// Scope.
template <LifecycleType lifecycleType, typename JniT, typename... ViableSpans>
struct Scoped;

// Objects.
template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class GlobalObject;

// Strings.
class LocalString;
class GlobalString;

// Arrays.
template <typename SpanType, std::size_t kRank_, const auto& class_v_,
          const auto& class_loader_v_, const auto& jvm_v_>
class LocalArray;

// Classloaders.
template <LifecycleType lifecycleType, const auto& jvm_v_,
          const auto& class_loader_v_>
class ClassLoaderRef;

// Thread Guards.
class ThreadGuard;
class ThreadLocalGuardDestructor;

// ValidatorProxy.
template <typename JniT>
struct ValidatorProxy;

}  // namespace jni

#endif  // JNI_BIND_TYPE_FORWARD_DECLARATIONS_H_
