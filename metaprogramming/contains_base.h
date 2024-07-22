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

#ifndef JNI_BIND_METAPROGRAMMING_CONTAINS_BASE_H_
#define JNI_BIND_METAPROGRAMMING_CONTAINS_BASE_H_

#include "any.h"
#include "base.h"
#include "invoke.h"
#include "same.h"
#include "tuple_manipulation.h"

namespace jni::metaprogramming {

template <typename SoughtType, typename... Ts>
using ContainsBase_t = Invoke_t<Any<Base<SoughtType>>, Ts...>;

template <typename SoughtType, typename... Tup>
static constexpr bool ContainsBase_v =
    ContainsBase_t<SoughtType, Tup...>::value;

template <typename SoughtType, typename Tup>
using TupContainsBase_t = TupleUnroller_t<Any<Same<SoughtType>>, Tup>;

template <typename SoughtType, typename Tup>
static constexpr bool TupContainsBase_v =
    TupContainsBase_t<SoughtType, Tup>::value;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_CONTAINS_BASE_H_
