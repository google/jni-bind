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

#ifndef JNI_BIND_METAPROGRAMMING_ZIP_INVOKE_H_
#define JNI_BIND_METAPROGRAMMING_ZIP_INVOKE_H_

#include <tuple>

#include "apply.h"
#include "invoke.h"
#include "per_element.h"
#include "type_of_nth_element.h"

namespace jni::metaprogramming {

// Takes a set of 2 tuples, |Knobs| and |Args|.
// This is frequently useful in a case where you might want to Apply across a
// set of arguments (e.g. Same {A, A} => Same<A>::template type<A>).
template <template <typename...> class Func, typename Knobs, typename Args,
          typename = void>
struct ZipInvoke {};

template <template <typename...> class Func, typename... Knobs,
          typename... Args>
struct ZipInvoke<
    Func, std::tuple<Knobs...>, std::tuple<Args...>,
    std::enable_if_t<(sizeof...(Knobs) == sizeof...(Args)), void>> {
  using type = std::tuple<Invoke_t<Apply_t<Func, Knobs>, Args>...>;
};

template <template <typename...> class Func, typename Knobs, typename Args>
using ZipInvoke_t = typename ZipInvoke<Func, Knobs, Args>::type;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_ZIP_INVOKE_H_
