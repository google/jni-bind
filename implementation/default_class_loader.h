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

#ifndef JNI_BIND_DEFAULT_CLASS_LOADER_H_
#define JNI_BIND_DEFAULT_CLASS_LOADER_H_

#include <cstddef>
#include <tuple>

#include "implementation/no_idx.h"
#include "metaprogramming/modified_max.h"

namespace jni {

static constexpr std::size_t kDefaultClassLoaderIdx = 0;
static constexpr std::size_t kClassNotInLoaderSetIdx =
    metaprogramming::kNegativeOne;

// Class loader that can supply any class (to be used when none is specified).
// Setting this as the root loader for user defined classes will disable checks
// that classes are explicitly listed under a loader's class list.
class DefaultClassLoader {
 public:
  const char* name_ = "__JNI_BIND_DEFAULT_CLASS_LOADER__";
  std::tuple<> supported_classes_{};

  // Note, this will return true iff ignore_default_loader is true, but the
  // corresponding IdxOfAncestor will be kClassNotInLoaderSetIdx.
  template <const auto&, bool ignore_default_loader = false>
  constexpr bool SupportedDirectlyOrIndirectly() const {
    if constexpr (ignore_default_loader) {
      // Note, it's an implementation detail that kDefaultClassLoader returns
      // false for |SupportedByThisSet|.  Otherwise, all classes will always
      // defer to the default loader.  See |ParentLoaderForClass|.
      return false;
    } else {
      return true;
    }
  }

  template <const auto& class_v>
  constexpr std::size_t IdxOfClass() const {
    return kClassNotInLoaderSetIdx;
  }

  template <const auto& possibly_supported_class>
  constexpr std::size_t IdxOfAncestor(std::size_t cur_idx = 0) const {
    return kClassNotInLoaderSetIdx;
  }

  template <typename T>
  bool constexpr operator==(const T& rhs) const {
    return false;
  }
  bool constexpr operator==(const DefaultClassLoader&) const { return true; }

  template <typename T>
  bool constexpr operator!=(const T& rhs) const {
    return !(*this == rhs);
  }
};

// Class loader that cannot supply any classes. This should be the root loader
// for most user defined classes.
class NullClassLoader {
 public:
  const char* name_ = "__JNI_BIND_NULL_CLASS_LOADER__";

  template <const auto&, bool ignore_default_loader = false>
  constexpr bool SupportedDirectlyOrIndirectly() const {
    return false;
  }

  template <const auto& class_v>
  constexpr std::size_t IdxOfClass() const {
    return kNoIdx;
  }

  template <const auto& possibly_supported_class>
  constexpr std::size_t IdxOfAncestor(std::size_t cur_idx = 0) const {
    return kClassNotInLoaderSetIdx;
  }

  template <typename T>
  bool constexpr operator==(const T& rhs) const {
    return false;
  }
  bool constexpr operator==(const NullClassLoader&) const { return true; }

  template <typename T>
  bool constexpr operator!=(const T& rhs) const {
    return !(*this == rhs);
  }
};

static constexpr NullClassLoader kNullClassLoader;
static constexpr DefaultClassLoader kDefaultClassLoader;

// DO NOT USE: This obviates a compiler bug for value based enablement on ctor.
static constexpr auto kShadowNullClassLoader = kNullClassLoader;

// DO NOT USE: This obviates a compiler bug for value based enablement on ctor.
static constexpr auto kShadowDefaultClassLoader = kDefaultClassLoader;

}  // namespace jni

#endif  // JNI_BIND_DEFAULT_CLASS_LOADER_H_
