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

#ifndef JNI_BIND_JVM_H_
#define JNI_BIND_JVM_H_

#include <tuple>
#include <type_traits>
#include <utility>

#include "class.h"
#include "class_loader.h"
#include "jni_dep.h"
#include "jni_helper/jni_env.h"

namespace jni {

template <typename... ClassLoaderTs>
class Jvm {
 public:
  const std::tuple<ClassLoaderTs...> class_loaders_;

  constexpr Jvm(ClassLoaderTs... class_loaders)
      : class_loaders_(class_loaders...) {}

  template <const auto& class_loader_v, std::size_t... Is>
  constexpr size_t IdxOfClassLoaderHelper(
      std::integer_sequence<std::size_t, Is...>) const {
    return metaprogramming::ModifiedMax(
        {((std::get<Is>(class_loaders_) == class_loader_v) ? Is : -1)...});
  }

  // Returns the index for a given classloader within this set (any given class
  // ref is defined by this index).
  template <const auto& class_loader_v>
  constexpr size_t IdxOfClassLoader() const {
    return IdxOfClassLoaderHelper<class_loader_v>(
        std::make_integer_sequence<std::size_t, sizeof...(ClassLoaderTs)>());
  }

  template <typename T>
  bool constexpr operator==(const T& rhs) const {
    return false;
  }
  bool constexpr operator==(const Jvm&) const { return true; }

  template <typename T>
  bool constexpr operator!=(const T& rhs) const {
    return !(*this == rhs);
  }
};

template <typename... ClassLoaderTs>
Jvm(ClassLoaderTs...) -> Jvm<ClassLoaderTs...>;

// Convenience Jvm definition.
// Compatible with default class loader or specified loaders.
inline constexpr Jvm kDefaultJvm{kDefaultClassLoader};

}  // namespace jni

#endif  // JNI_BIND_JVM_H_
