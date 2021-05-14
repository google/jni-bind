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

#ifndef JNI_BIND_METAPROGRAMMING_INCREMENT_H_
#define JNI_BIND_METAPROGRAMMING_INCREMENT_H_

#include <type_traits>
#include <utility>

namespace jni::metaprogramming {

// Metafunction to increment types a certain number of times.
// Types must export a type alias "Increment" which increments the type once.
template <std::size_t I>
struct Increment {
  template <typename T, std::size_t Idx>
  struct IncrementCountHelper {
    using type = typename Increment<Idx - 1>::template type<T>::Increment;
  };

  template <typename T>
  struct IncrementCountHelper<T, 0> {
    using type = T;
  };

  template <typename T>
  using type = typename IncrementCountHelper<T, I>::type;
};

template <typename T, std::size_t I = 1>
using Increment_t = typename Increment<I>::template type<T>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_INCREMENT_H_
