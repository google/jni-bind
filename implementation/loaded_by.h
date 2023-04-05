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
#ifndef JNI_BIND_IMPLEMENTATION_LOADED_BY_H_
#define JNI_BIND_IMPLEMENTATION_LOADED_BY_H_

#include "implementation/class_loader.h"
#include "implementation/default_class_loader.h"

namespace jni {

struct LoaderTag {};

// Annotation for use in function and field declarations. When used as argument
// the underlying object must come from the same class loader.
template <typename ClassLoaderT, typename ClassT>
struct LoadedBy : LoaderTag {
  const ClassLoaderT class_loader_;
  const ClassT class_;

  static_assert(
      !std::is_same_v<ClassLoaderT, ClassLoader<DefaultClassLoader>>,
      "LoadedBy is not required for the default loader (it's implicit).");

  constexpr LoadedBy(ClassLoaderT class_loader, ClassT clazz)
      : class_loader_(class_loader), class_(clazz) {}
};

template <typename ClassLoaderT, typename ClassT>
LoadedBy(ClassLoaderT, ClassT) -> LoadedBy<ClassLoaderT, ClassT>;

template <typename T>
struct IsLoadedBy {
  static constexpr bool val = false;
};

template <typename ClassLoaderT, typename ClassT>
struct IsLoadedBy<LoadedBy<ClassLoaderT, ClassT>> {
  static constexpr bool val = true;
};

template <typename T>
static constexpr bool IsLoadedBy_v = IsLoadedBy<T>::val;

template <typename T>
constexpr auto StripClassFromLoadedBy(T val) {
  if constexpr (IsLoadedBy_v<T>) {
    return val.class_;
  } else {
    return val;
  }
}

template <typename T>
constexpr auto StripClassLoaderFromLoadedBy(T val) {
  if constexpr (IsLoadedBy_v<T>) {
    return val.class_loader_;
  } else {
    return val;
  }
}

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_LOADED_BY_H_
