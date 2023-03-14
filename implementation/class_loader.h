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

#ifndef JNI_BIND_CLASS_LOADER_H_
#define JNI_BIND_CLASS_LOADER_H_

#include <tuple>
#include <type_traits>
#include <utility>

#include "class_defs/java_lang_classes.h"
#include "implementation/class.h"
#include "implementation/default_class_loader.h"
#include "implementation/no_idx.h"
#include "implementation/object.h"
#include "implementation/supported_class_set.h"
#include "metaprogramming/all_unique.h"
#include "metaprogramming/base_filter.h"
#include "metaprogramming/modified_max.h"

namespace jni {

inline constexpr struct NoClassLoader {
} kNoClassLoaderSpecified;

// this is just the list of classes we expect to be loadable from a class loader
// and its parent loader.
//
// Classes from different loaders are typically incompatible, but Class loaders
// delegate classes that they cannot directly load to their parent loaders, so
// classes attached to two different class loaders will still be compatible if
// they were loaded by a shared parent loader.
template <typename ParentLoader_, typename... SupportedClasses_>
class ClassLoader : Object {
 public:
  const ParentLoader_ parent_loader_;
  const std::tuple<SupportedClasses_...> supported_classes_;

  // Default classloader (no name needed).
  explicit constexpr ClassLoader(
      ParentLoader_ parent_loader,
      SupportedClassSet<SupportedClasses_...> supported_class_set)
      __attribute__((
          enable_if(parent_loader == kDefaultClassLoader,
                    "You must provide a name for classloaders (except "
                    "kNullClassLoader and kDefaultClassLoader)")))
      : Object("__JNI_BIND_DEFAULT_CLASS_LOADER__"),
        parent_loader_(parent_loader),
        supported_classes_(supported_class_set.supported_classes_) {}

  // Null classloader (no name needed).
  explicit constexpr ClassLoader(
      ParentLoader_ parent_loader,
      SupportedClassSet<SupportedClasses_...> supported_class_set)
      __attribute__((
          enable_if(parent_loader == kNullClassLoader,
                    "You must provide a name for classloaders (except "
                    "kNullClassLoader and kDefaultClassLoader)")))
      : Object("__JNI_BIND_NULL_CLASS_LOADER__"),
        parent_loader_(parent_loader),
        supported_classes_(supported_class_set.supported_classes_) {}

  // TODO (b/143908983): Loaders should not be able to supply classes that their
  // parents do.
  explicit constexpr ClassLoader(
      const char* class_loader_name, ParentLoader_ parent_loader,
      SupportedClassSet<SupportedClasses_...> supported_class_set)

      : Object(class_loader_name),
        parent_loader_(parent_loader),
        supported_classes_(supported_class_set.supported_classes_) {}

  bool constexpr operator==(
      const ClassLoader<ParentLoader_, SupportedClasses_...>& rhs) const {
    return (*this).parent_loader_ == rhs.parent_loader_ &&
           (*this).supported_classes_ == rhs.supported_classes_;
  }
  template <typename T>
  bool constexpr operator==(const T& rhs) const {
    return false;
  }
  template <typename T>
  bool constexpr operator!=(const T& rhs) const {
    return !(*this == rhs);
  }

  template <const auto& class_v, std::size_t... Is>
  constexpr std::size_t IdxOfClassHelper(
      std::integer_sequence<std::size_t, Is...>) const {
    // std::max appears to be missing the initializer list overload in Bazel's
    // implementation of clang.  This should simply be std::max.
    return metaprogramming::ModifiedMax(
        {((std::get<Is>(supported_classes_) == class_v)
              ? std::size_t{Is}
              : metaprogramming::kNegativeOne)...,
         metaprogramming::kNegativeOne});
  }

  // Returns the index for a given class within this set (any given class ref is
  // defined by this index).
  template <const auto& class_v>
  constexpr std::size_t IdxOfClass() const {
    return IdxOfClassHelper<class_v>(
        std::make_integer_sequence<std::size_t,
                                   sizeof...(SupportedClasses_)>());
  }

  // Tests if a class is supported by this class loader (not ancestors).
  template <const auto& possibly_supported_class>
  constexpr bool SupportedByThisSet() const {
    return IdxOfClass<possibly_supported_class>() != kClassNotInLoaderSetIdx;
  }

  // Tests if a class is supported by this class loader (ancestors included).
  template <const auto& possibly_supported_class,
            bool ignore_default_loader = false>
  constexpr bool SupportedDirectlyOrIndirectly() const {
    return parent_loader_.template SupportedDirectlyOrIndirectly<
               possibly_supported_class>() ||
           SupportedByThisSet<possibly_supported_class>();
  }

  // Returns the index for parent-most ancestor that supports a given class.
  template <const auto& possibly_supported_class>
  constexpr std::size_t IdxOfAncestor(std::size_t cur_idx = 0) const {
    if (SupportedByThisSet<possibly_supported_class>() &&
        !parent_loader_.template SupportedDirectlyOrIndirectly<
            possibly_supported_class, true>()) {
      return cur_idx;
    }
    return parent_loader_.template IdxOfAncestor<possibly_supported_class>(
        cur_idx + 1);
  }
};

template <typename ParentLoader_, typename... SupportedClasses_>
ClassLoader(ParentLoader_ parent_loader,
            SupportedClassSet<SupportedClasses_...> supported_classes)
    -> ClassLoader<ParentLoader_, SupportedClasses_...>;

template <typename... SupportedClasses_>
ClassLoader(SupportedClassSet<SupportedClasses_...>)
    -> ClassLoader<DefaultClassLoader, SupportedClasses_...>;

template <typename T, std::size_t I>
constexpr auto& GetAncestor(const T& loader) {
  if constexpr (I == 0) {
    return loader;
  } else {
    return GetAncestor<decltype(loader.parent_loader_), I - 1>(
        loader.parent_loader_);
  }
}

template <const auto& loader, const auto& possibly_supported_class>
constexpr const auto& ParentLoaderForClass() {
  if constexpr (!loader.template SupportedDirectlyOrIndirectly<
                    possibly_supported_class>()) {
    return kNullClassLoader;
  } else if constexpr (loader.template IdxOfAncestor<
                           possibly_supported_class>() !=
                       kClassNotInLoaderSetIdx) {
    return GetAncestor<
        decltype(loader),
        loader.template IdxOfAncestor<possibly_supported_class>()>(loader);
  } else {
    return kDefaultClassLoader;
  }
}

}  // namespace jni

#endif  // JNI_BIND_CLASS_LOADER_H_
