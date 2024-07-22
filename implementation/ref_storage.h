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

#ifndef JNI_BIND_IMPLEMENTATION_REF_STORAGE_H_
#define JNI_BIND_IMPLEMENTATION_REF_STORAGE_H_

#include <functional>
#include <vector>

#include "metaprogramming/double_locked_value.h"
#include "metaprogramming/lambda_string.h"

namespace jni {

// Used as shared storage of lists for IDs like jclass, jMethod, etc.
// Only applicable for Jvms not fully specified (i.e. default classloader).
// See JvmRef::~JvmRef.
template <typename T>
static std::vector<metaprogramming::DoubleLockedValue<T>*>& DefaultRefs() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<T>*>{};
  return *ret_val;
}

// Provides a static inline `DoubleLockedValue<T>` val against a `UniqueID`.
template <typename UniqueID, typename T>
struct StaticDoubleLock {
  static inline metaprogramming::DoubleLockedValue<T> val;
};

// Takes a GetLambda and only invokes it for the first time on equal values of
// `SignatureLambda`. This is useful for putting `const char*` into type IDs.
template <typename GetLambda, typename SignatureLambda>
struct RefStorage {
  // Return of `GetLambda`.
  using ReturnT = decltype(std::declval<GetLambda>()(nullptr));

  // Compile-time unique ID.
  static constexpr auto kSignature = []() {
    return SignatureLambda::TypeName().data();
  };
  using Signature = metaprogramming::LambdaStringToType<decltype(kSignature)>;

  // Common ID-wide double locked value.
  using Storage = StaticDoubleLock<Signature, ReturnT>;

  // Retrieves the guarded value, possibly invoking the expensive lambda.
  static ReturnT Get(GetLambda lambda) {
    return StaticDoubleLock<Signature, ReturnT>::val.LoadAndMaybeInit(
        std::bind(lambda, &Storage::val));
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_REF_STORAGE_H_
