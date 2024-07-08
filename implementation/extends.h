/*
 * Copyright 2024 Google LLC
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
#ifndef JNI_BIND_IMPLEMENTATION_EXTENDS_H_
#define JNI_BIND_IMPLEMENTATION_EXTENDS_H_

#include "implementation/no_class_specified.h"
#include "metaprogramming/tuple_manipulation.h"
#include "metaprogramming/type_of_nth_element.h"

namespace jni {

// Metafunction for helping strip the type for input Extends params.
struct ExtendsStrip {
  template <typename T>
  struct Helper {
    // Input is in a tuple of 1 element.
    using StripOuterTuple = metaprogramming::TypeOfNthTupleElement_t<0, T>;
    // Steal the outer Extends parameter pack into a tuple.
    // e.g. Extends<RootClass> => std::tuple<RootClass>.
    using ExtendsToTuple =
        metaprogramming::ExtractTupleFromType_t<StripOuterTuple>;
    // Extracts the single element.
    using type = metaprogramming::TypeOfNthTupleElement_t<0, ExtendsToTuple>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

template <typename T>
using ExtendsStrip_t = typename ExtendsStrip::template type<T>;

template <typename ParentT>
struct Extends : public ExtendsBase {
  constexpr Extends(ParentT parent) : parent_(parent) {}

  const ParentT parent_;
};

template <typename T>
Extends(T) -> Extends<T>;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_EXTENDS_H_
