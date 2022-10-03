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
#ifndef JNI_BIND_IMPLEMENTATION_INPUT_PARAM_SELECTION_H_
#define JNI_BIND_IMPLEMENTATION_INPUT_PARAM_SELECTION_H_

#include <cstddef>
#include <limits>

#include "implementation/array.h"
#include "implementation/id.h"
#include "implementation/proxy.h"

namespace jni {

// Represents a return value index for |InputParamSelection|.
static constexpr std::size_t kIsReturnIdx =
    std::numeric_limits<std::size_t>::max();

template <typename OverloadSelectionT, size_t param_idx>
struct InputParamSelection {
  // If |param_idx| is kIsReturnIdx, this is the return value.
  static constexpr bool kIsReturn = (param_idx == kIsReturnIdx);

  using IdTParamType = typename OverloadSelectionT::IdT::template ChangeIdType<
      IdType::OVERLOAD_PARAM>;
  using IdT = typename IdTParamType::template ChangeIdx<2, param_idx>;

  static constexpr inline const auto& Val() { return IdT::Val(); }

  using RawValT = ArrayStrip_t<std::decay_t<decltype(IdT::Val())>>;
  using UnstrippedRawVal = std::decay_t<decltype(IdT::Val())>;

  static constexpr std::size_t kRank = Rankifier<RawValT>::Rank(IdT::Val());

  // Find the appropriate proxy logic for the given argument, and see if that
  // parameter is contextually correct given the arguments.
  template <typename... Ts>
  static constexpr bool kValid = Proxy_t<UnstrippedRawVal>::template kViable<
      InputParamSelection,
      metaprogramming::TypeOfNthElement_t<param_idx, Ts...>>;
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_INPUT_PARAM_SELECTION_H_
