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

#ifndef JNI_BIND_METAPROGRAMMING_VALS_H_
#define JNI_BIND_METAPROGRAMMING_VALS_H_

#include <array>
#include <cstddef>
#include <utility>

namespace jni::metaprogramming {

// Wrapper to convert a sequence of values into a type.
template <auto... Vs>
struct Vals {
  static constexpr std::array val{Vs...};
};

// Wrapper to convert a sequence of const values into a type.
template <const auto... Vs>
struct ValsConst {
  static constexpr std::array val{Vs...};
};

// Wrapper to convert a sequence of ref values into a type.
template <auto&... Vs>
struct ValsRef {
  static constexpr std::array val{Vs...};
};

// Wrapper to convert a sequence of const ref values into a type.
template <const auto&... Vs>
struct ValsConstRef {
  static constexpr std::array val{Vs...};
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_VALS_H_
