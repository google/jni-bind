/*
 * Copyright 2022 Google LLC
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
#ifndef JNI_BIND_METAPROGRAMMING_NEXT_H_
#define JNI_BIND_METAPROGRAMMING_NEXT_H_

#include <type_traits>

#include "pack_discriminator.h"

namespace jni::metaprogramming {

template <template <typename...> class Container>
struct NextType;

template <template <typename...> class Container>
struct EndType;

template <template <auto...> class Container>
struct NextVal;

template <template <auto...> class Container>
struct EndVal;

template <template <const auto&...> class Container>
struct NextConstRefVal;

template <template <const auto...> class Container>
struct EndConstRefVal;

// Returns the "next" value in given a value in type space.
// |T| must always be a container of a single kind of template expression.
template <typename T>
using Next_t =
    typename PackDiscriminatedForward<NextType, NextVal,
                                      NextConstRefVal>::template type<T>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_NEXT_H_
