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

#ifndef JNI_BIND_METAPROGRAMMING_INTERLEAVE_H_
#define JNI_BIND_METAPROGRAMMING_INTERLEAVE_H_

#include <tuple>
#include <type_traits>
#include <utility>

namespace jni::metaprogramming {

// Returns a null pointer of the type of the two input tuples interleaved.
template <class Tuple1, class Tuple2, std::size_t... indices>
auto Interleave(std::integer_sequence<std::size_t, indices...>)
    -> decltype(std::tuple_cat(
        std::make_tuple(std::get<indices>(std::declval<Tuple1>()),
                        std::get<indices>(std::declval<Tuple2>()))...))* {
  // This interleave is for *types only*, all values within the tuples are
  // completely incidental.  In the event there is no default constructor, it
  // won't be possible to return a value, so, instead, return a pointer (which
  // won't be used) and infer the type by stripping the pointer.
  return nullptr;
}

template <class Tuple1, class Tuple2>
auto Interleave() {
  return Interleave<Tuple1, Tuple2>(
      std::make_index_sequence<std::tuple_size<Tuple1>::value>());
}

template <typename T0, typename T1>
struct Interleaved;

template <typename... T0, typename... T1>
struct Interleaved<std::tuple<T0...>, std::tuple<T1...>> {
  using type = std::remove_pointer_t<decltype(
      Interleave<std::tuple<T0...>, std::tuple<T1...>>())>;
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_INTERLEAVE_H_
