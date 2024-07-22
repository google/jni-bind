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

#ifndef JNI_BIND_METAPROGRAMMING_TUPLE_FROM_SIZE_H_
#define JNI_BIND_METAPROGRAMMING_TUPLE_FROM_SIZE_H_

#include <cstddef>
#include <tuple>
#include <utility>

namespace jni::metaprogramming {

template <class T, std::size_t>
using T_ = T;

template <class DefaultType, std::size_t... Is>
auto TupleFromSize(std::index_sequence<Is...>) {
  return std::tuple<T_<DefaultType, Is>...>{};
}

// Takes a type and returns a std::tuple of DefaultValues.
template <class DefaultType, std::size_t N>
auto TupleFromSize() {
  return TupleFromSize<DefaultType>(std::make_index_sequence<N>{});
}

template <class DefaultType, std::size_t N>
using TupleFromSize_t = decltype(TupleFromSize<DefaultType, N>());

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_TUPLE_FROM_SIZE_H_
