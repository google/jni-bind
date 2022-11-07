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

#ifndef JNI_BIND_METHOD_SELECTION_H_
#define JNI_BIND_METHOD_SELECTION_H_

#include <limits>
#include <type_traits>
#include <utility>

#include "implementation/array.h"
#include "implementation/array_ref.h"
#include "implementation/default_class_loader.h"
#include "implementation/id.h"
#include "implementation/jni_helper/jni_typename_to_string.h"
#include "implementation/method_ref.h"
#include "implementation/no_idx.h"
#include "implementation/proxy.h"
#include "implementation/selector_static_info.h"
#include "metaprogramming/concatenate.h"
#include "metaprogramming/invoke.h"
#include "metaprogramming/n_bit_sequence.h"
#include "metaprogramming/tuple_manipulation.h"
#include "metaprogramming/type_index_mask.h"
#include "metaprogramming/type_of_nth_element.h"
#include "metaprogramming/unfurl.h"

namespace jni {

// Viablility helper for an exact parameter.
template <std::size_t I, typename IdT, typename... Ts>
struct Viable {
  using IdTmp = typename IdT::template ChangeIdType<IdType::OVERLOAD_PARAM>;
  using IdTParamType = typename IdTmp::template ChangeIdx<2, I>;

  static constexpr bool val =
      Proxy_t<typename IdTParamType::UnstrippedRawVal>::template kViable<
          IdTParamType, metaprogramming::TypeOfNthElement_t<I, Ts...>>;
};

template <typename OverloadId>
struct ArgumentValidate {
  // Helper to prevents instantiating mismatching size unrolls.
  template <typename... Ts>
  static constexpr bool ViableHelper() {
    if constexpr (sizeof...(Ts) == OverloadId::kNumParams) {
      return metaprogramming::UnfurlConjunction_v<OverloadId::kNumParams,
                                                  Viable, OverloadId, Ts...>;

    } else {
      return false;
    }
  }

  template <typename... Ts>
  static constexpr bool kValid = ViableHelper<Ts...>();
};

template <typename IdT_>
struct OverloadSelection {
  using IdT = IdT_;

  template <typename... Ts>
  static constexpr bool OverloadViable() {
    return ArgumentValidate<IdT>::template kValid<Ts...>;
  }

  template <typename... Ts>
  static constexpr size_t OverloadIdxIfViable() {
    return OverloadViable<Ts...>() ? IdT::kSecondaryIdx : kNoIdx;
  }
};

template <typename IdT_>
struct MethodSelection {
  using IdT = IdT_;
  using JniType = typename IdT::JniType;

  template <typename Is, typename... Ts>
  struct Helper;

  template <size_t... Is, typename... Ts>
  struct Helper<std::index_sequence<Is...>, Ts...> {
    static constexpr bool val =
        (OverloadSelection<Id<JniType, IdType::OVERLOAD, IdT::kIdx,
                              Is>>::template OverloadViable<Ts...>() ||
         ...);

    // kNoIdx is the max of std::size_t, so, this essentially selects any
    // idx (if a valid one exists), or defaults to kNoIdx.
    static constexpr std::size_t overload_idx_if_valid{std::min(
        {OverloadSelection<Id<JniType, IdType::OVERLOAD, IdT::kIdx,
                              Is>>::template OverloadIdxIfViable<Ts...>()...})};
  };

  template <typename... Ts>
  static constexpr bool ArgSetViable() {
    return Helper<std::make_index_sequence<IdT::NumParams()>,
                  std::decay_t<Ts>...>::val;
  }

  // The overload that is viable for a set of args, or |kNoIdx|.
  template <typename... Ts>
  static constexpr std::size_t IdxForArgs() {
    return Helper<std::make_index_sequence<IdT::NumParams()>,
                  std::decay_t<Ts>...>::overload_idx_if_valid;
  }

  template <typename... Ts>
  using FindOverloadSelection = OverloadSelection<
      Id<JniType, IdType::OVERLOAD, IdT::kIdx, IdxForArgs<Ts...>()>>;
};

template <typename IdT, typename... Args>
struct OverloadSelector {
  using OverloadSelectionForArgs =
      typename MethodSelection<IdT>::template FindOverloadSelection<Args...>;

  using OverloadIdT = Id<typename IdT::JniType, IdType::OVERLOAD, IdT::kIdx,
                         OverloadSelectionForArgs::IdT::kSecondaryIdx>;

  using OverloadRef = OverloadRef<OverloadIdT, typename IdT::JniType>;

  static constexpr bool kIsValidArgSet =
      MethodSelection<IdT>::template ArgSetViable<Args...>();
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_SELECTION_H_
