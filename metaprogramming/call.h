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
#ifndef JNI_BIND_METAPROGRAMMING_CALL_H_
#define JNI_BIND_METAPROGRAMMING_CALL_H_

#include <tuple>

namespace jni::metaprogramming {

// Call takes a tuple of arguments and returns a tuple of elements
// dereferenced once (with no arguments). Eventually, this should have
// the pack discriminated forwarding (to optionally pass args).
struct Call {
  template <typename T>
  struct Helper;

  template <typename... Ts>
  struct Helper<std::tuple<Ts...>> {
    using type = std::tuple<typename Ts::type...>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

template <typename T>
using Call_t = typename Call::type<T>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_CALL_H_
