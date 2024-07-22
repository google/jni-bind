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

#ifndef JNI_BIND_IMPLEMENTATION_CORPUS_H_
#define JNI_BIND_IMPLEMENTATION_CORPUS_H_

#include <tuple>

#include "concatenate.h"
#include "corpus_tag.h"
#include "detect.h"

namespace jni::metaprogramming {

// Provides the universe of keys, including user defined types if any.
// Users define custom types by partially specialising UserDefined (see test).
template <typename... Defaults>
struct Corpus {
  template <typename T, bool = Detect_v<UserDefined, T> >
  struct Helper {
    using type =
        ConcatenateTup_t<Detect_t<UserDefined, T>, std::tuple<Defaults...> >;
  };

  template <typename T>
  struct Helper<T, false> {
    using type = std::tuple<Defaults...>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

template <typename T, typename... Defaults>
using Corpus_t = typename Corpus<Defaults...>::template type<T>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_IMPLEMENTATION_CORPUS_H_
