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

namespace jni {

// Represents an indexing into a specific class and method.
template <typename JniType, bool is_constructor, size_t method_idx>
struct MethodSelection;

// Represents a specific overload selection.
template <typename JniType, typename MethodSelectionT, size_t overload_idx>
struct OverloadSelection;

// Helper to find overloads for a given arg set.
template <typename JniType, bool is_constructor, size_t method_idx,
          typename... Args>
struct OverloadSelectionForArgsImpl;

////////////////////////////////////////////////////////////////////////////////
// Helper Aliases.
////////////////////////////////////////////////////////////////////////////////
template <typename JniType, bool is_constructor, size_t method_idx>
using MethodSelection_t = MethodSelection<JniType, is_constructor, method_idx>;

template <typename JniType, bool is_constructor, size_t method_idx,
          typename... Args>
using MethodSelectionForArgs_t =
    OverloadSelectionForArgsImpl<JniType, is_constructor, method_idx, Args...>;

////////////////////////////////////////////////////////////////////////////////
// Implementation Details.
////////////////////////////////////////////////////////////////////////////////
template <typename JniType, bool is_constructor, size_t method_idx>
struct MethodSelection {
  static constexpr IdType kIdType = IdType::OVERLOAD_SET;

  static constexpr std::size_t kMethodIdx =
      is_constructor ? kNoIdx : method_idx;

  using IdT =
      Id<JniType, kIdType, kMethodIdx, is_constructor ? method_idx : kNoIdx>;

  static constexpr bool kIsConstructor = is_constructor;

  static constexpr const auto& GetClass() { return JniType::class_v; }
  static constexpr const auto& GetClassLoader() {
    return JniType::class_loader_v;
  }

  static constexpr const auto& GetMethod() {
    if constexpr (is_constructor) {
      static_assert(method_idx == 0,
                    "If using MethodSelection for a constructor, there is only "
                    "ever one method (\"<init>\"), set method_idx to 0.");
      return JniType::class_v.constructors_;
    } else {
      return std::get<method_idx>(JniType::class_v.methods_);
    }
  }

  static constexpr const char* Name() { return IdT::Name(); }

  static constexpr std::size_t NumOverloads() {
    if constexpr (is_constructor) {
      return std::tuple_size<decltype(JniType::class_v.constructors_)>();
    } else {
      return std::tuple_size<decltype(GetMethod().invocations_)>();
    }
  }

  template <typename Is, typename... Ts>
  struct Helper;

  template <size_t... Is, typename... Ts>
  struct Helper<std::index_sequence<Is...>, Ts...> {
    static constexpr bool val =
        (OverloadSelection<JniType, MethodSelection,
                           Is>::template OverloadViable<Ts...>() ||
         ...);

    // kNoIdx is the max of std::size_t, so, this essentially selects any
    // idx (if a valid one exists), or defaults to kNoIdx.
    static constexpr std::size_t overload_idx_if_valid{std::min(
        {OverloadSelection<JniType, MethodSelection,
                           Is>::template OverloadIdxIfViable<Ts...>()...})};
  };

  template <typename... Ts>
  static constexpr bool ArgSetViable() {
    return Helper<std::make_index_sequence<NumOverloads()>,
                  std::decay_t<Ts>...>::val;
  }

  // The overload that is viable for a set of args, or |kNoIdx|.
  template <typename... Ts>
  static constexpr std::size_t IdxForArgs() {
    return Helper<std::make_index_sequence<NumOverloads()>,
                  std::decay_t<Ts>...>::overload_idx_if_valid;
  }

  template <typename... Ts>
  using FindOverloadSelection =
      OverloadSelection<JniType, MethodSelection, IdxForArgs<Ts...>()>;
};

template <typename OverloadSelectionT>
struct ArgumentValidate {
  template <bool kSameSize, typename T, typename... Ts>
  struct Helper {
    static constexpr bool kValid = false;
  };

  template <std::size_t... Is, typename... Ts>
  struct Helper<true, std::index_sequence<Is...>, Ts...> {
    using IdTmp = typename OverloadSelectionT::IdT::template ChangeIdType<
        IdType::OVERLOAD_PARAM>;

    template <std::size_t I>
    using IdTParamType = typename IdTmp::template ChangeIdx<2, I>;

    static constexpr bool kValid =
        (Proxy_t<typename IdTParamType<Is>::UnstrippedRawVal>::template kViable<
             IdTParamType<Is>,
             metaprogramming::TypeOfNthElement_t<Is, Ts...>> &&
         ...);
  };

  template <typename... Ts>
  static constexpr bool kValid =
      Helper<(sizeof...(Ts) == OverloadSelectionT::kNumParams),
             std::make_index_sequence<sizeof...(Ts)>, Ts...>::kValid;
};

template <typename JniType, typename MethodSelectionT, size_t overload_idx>
struct OverloadSelection {
  using IdT =
      Id<JniType, IdType::OVERLOAD, MethodSelectionT::kMethodIdx, overload_idx>;

  static constexpr Return kObjectWhenConstructed{
      Class{JniType::GetClass().name_}};

  static constexpr const auto& GetReturn() {
    if constexpr (MethodSelectionT::kIsConstructor) {
      return kObjectWhenConstructed;
    } else {
      return std::get<overload_idx>(MethodSelectionT::GetMethod().invocations_)
          .return_;
    }
  }

  static constexpr const auto& GetParams() {
    if constexpr (MethodSelectionT::kIsConstructor) {
      return std::get<overload_idx>(MethodSelectionT::GetMethod()).params_;
    } else {
      return std::get<overload_idx>(MethodSelectionT::GetMethod().invocations_)
          .params_;
    }
  }

  using ReturnT = std::decay_t<decltype(GetReturn())>;
  using ParamsT = std::decay_t<decltype(GetParams().values_)>;

  // CDecl is the type used by the C API for a return type.
  using CDecl = CDecl_t<Raw_t<ReturnT>>;

  // Return type is the richly decorated type returned (e.g LocalArray).
  using AsReturn = Return_t<Raw_t<ReturnT>, OverloadSelection>;

  static constexpr size_t kNumParams = std::tuple_size_v<ParamsT>;

  template <typename... Ts>
  static constexpr bool OverloadViable() {
    return ArgumentValidate<OverloadSelection>::template kValid<Ts...>;
  }

  template <typename... Ts>
  static constexpr size_t OverloadIdxIfViable() {
    return OverloadViable<Ts...>() ? overload_idx : kNoIdx;
  }
};

template <typename JniType, bool is_constructor, size_t method_idx,
          typename... Args>
struct OverloadSelectionForArgsImpl {
  using MethodSelectionForArgs =
      MethodSelection_t<JniType, is_constructor, method_idx>;
  using OverloadSelectionForArgs =
      typename MethodSelectionForArgs::template FindOverloadSelection<Args...>;

  using OverloadRef =
      OverloadRef<JniType, MethodSelectionForArgs, OverloadSelectionForArgs>;

  static constexpr bool kIsValidArgSet =
      MethodSelectionForArgs::template ArgSetViable<Args...>();
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_SELECTION_H_
