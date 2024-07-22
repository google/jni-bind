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

#ifndef JNI_BIND_SUPPORTED_CLASS_SET_H_
#define JNI_BIND_SUPPORTED_CLASS_SET_H_

#include <tuple>

namespace jni {

// The set of classes that a ClassLoader can load.
template <typename... Classes_>
class SupportedClassSet {
 public:
  const std::tuple<Classes_...> supported_classes_;

  constexpr SupportedClassSet(Classes_... supported_classes)
      : supported_classes_(supported_classes...) {
    // TODO(b/143908983): Classloaders should enforce unique classes.
    // static_assert(metaprogramming::AllUniqueValues(supported_classes...),
    //"All classes supported by the class loader must be unique.");
  }
};

template <typename... Classes>
SupportedClassSet(Classes...) -> SupportedClassSet<Classes...>;

}  // namespace jni

#endif  // JNI_BIND_SUPPORTED_CLASS_SET_H_
