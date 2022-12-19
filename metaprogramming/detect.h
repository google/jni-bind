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
#ifndef JNI_BIND_METAPROGRAMMING_DETECT_H_
#define JNI_BIND_METAPROGRAMMING_DETECT_H_

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Metafunction that detects a partial specialisation for a Container.
template <template <typename> class Container>
struct Detect {
  template <typename T, typename Enable = void>
  struct Helper {
    static constexpr bool val = false;
    using type = std::tuple<>;
  };

  template <typename T>
  struct Helper<T, std::void_t<typename Container<T>::type>> {
    static constexpr bool val = true;
    using type = typename Container<T>::type;
  };

  template <typename T>
  using type = typename Helper<T>::type;

  template <typename T>
  static constexpr bool val = Helper<T>::val;
};

template <template <typename> class Container, typename T>
using Detect_t = typename Detect<Container>::template type<T>;

template <template <typename> class Container, typename T>
static constexpr bool Detect_v = Detect<Container>::template val<T>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_DETECT_H_
