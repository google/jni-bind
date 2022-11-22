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
#include "metaprogramming/call.h"
#include "metaprogramming/concatenate.h"
#include "metaprogramming/invoke.h"
#include "metaprogramming/min_max.h"
#include "metaprogramming/n_bit_sequence.h"
#include "metaprogramming/per_element.h"
#include "metaprogramming/reduce.h"
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
          IdTParamType,
          metaprogramming::TypeOfNthElement_t<I, std::decay_t<Ts>...>>;
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

  template <std::size_t I, typename... Ts>
  struct Helper {
    using type = metaprogramming::Val_t<
        OverloadSelection<Id<JniType, IdType::OVERLOAD, IdT::kIdx,
                             I>>::template OverloadIdxIfViable<Ts...>()>;
  };

  template <typename... Ts>
  static constexpr std::size_t kIdxForTs = metaprogramming::ReduceAsPack_t<
      metaprogramming::Min, metaprogramming::Call_t<metaprogramming::Unfurl_t<
                                IdT::NumParams(), Helper, Ts...>>>::val;

  template <typename... Ts>
  using FindOverloadSelection = OverloadSelection<
      Id<JniType, IdType::OVERLOAD, IdT::kIdx, kIdxForTs<Ts...>>>;

  template <typename... Ts>
  static constexpr bool ArgSetViable() {
    return kIdxForTs<Ts...> != kNoIdx;
  }
};

template <typename IdT, typename... Args>
struct OverloadSelector {
  using OverloadSelectionForArgs =
      typename MethodSelection<IdT>::template FindOverloadSelection<Args...>;

  using OverloadRef =
      OverloadRef<Id<typename IdT::JniType, IdType::OVERLOAD, IdT::kIdx,
                     OverloadSelectionForArgs::IdT::kSecondaryIdx>>;

  static constexpr bool kIsValidArgSet =
      MethodSelection<IdT>::template ArgSetViable<Args...>();
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_SELECTION_H_
