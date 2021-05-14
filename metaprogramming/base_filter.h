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

#ifndef JNI_BIND_METAPROGRAMMING_BASE_FILTER_H_
#define JNI_BIND_METAPROGRAMMING_BASE_FILTER_H_

#include <tuple>
#include <type_traits>

#include "concatenate.h"

namespace jni::metaprogramming {

// Returns all elements derived from DesiredBase.
//
// Note, if no values found, this will be std::tuple<>.  If you'd like a default
// value use BaseFilterWithDefault.
template <typename DesiredBase, typename Enable = void,
          typename T = std::tuple<>>
struct BaseFilter {
  static_assert(std::is_same_v<T, std::tuple<>>);
  using type = std::tuple<>;
};

template <typename DesiredBase, typename... Ts>
using BaseFilter_t =
    typename BaseFilter<DesiredBase, void, std::tuple<Ts...>>::type;

template <typename DesiredBase, typename T, typename... Ts>
struct BaseFilter<DesiredBase,
                  std::enable_if_t<std::is_base_of_v<DesiredBase, T>, void>,
                  std::tuple<T, Ts...>> {
  using type =
      ConcatenateTup_t<std::tuple<T>, BaseFilter_t<DesiredBase, Ts...>>;
};

template <typename DesiredBase, typename T, typename... Ts>
struct BaseFilter<DesiredBase,
                  std::enable_if_t<!std::is_base_of_v<DesiredBase, T>, void>,
                  std::tuple<T, Ts...>> {
  using type = BaseFilter_t<DesiredBase, Ts...>;
};

//==============================================================================
template <typename PostFilterValue, typename DefaultValue>
using DefaultIfEmpty_Tup =
    std::conditional_t<std::is_same_v<PostFilterValue, std::tuple<>>,
                       DefaultValue, PostFilterValue>;

template <typename DesiredBase, typename DefaultValue, typename... Ts>
using BaseFilterWithDefault_t =
    DefaultIfEmpty_Tup<BaseFilter_t<DesiredBase, Ts...>,
                       std::tuple<DefaultValue>>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_BASE_FILTER_H_
