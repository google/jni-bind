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
#include "implementation/input_param_selection.h"
#include "implementation/jni_helper/jni_typename_to_string.h"
#include "implementation/method_ref.h"
#include "implementation/proxy.h"
#include "implementation/selector_static_info.h"
#include "metaprogramming/concatenate.h"
#include "metaprogramming/invoke.h"
#include "metaprogramming/n_bit_sequence.h"
#include "metaprogramming/tuple_manipulation.h"
#include "metaprogramming/type_index_mask.h"
#include "metaprogramming/type_of_nth_element.h"

namespace jni {

static constexpr std::size_t kNoSelection = std::numeric_limits<size_t>::max();

// The the type of an exact selection parameter in a method as part of the class
// specification (vs. the selection of a parameter in some generated candidate).
template <typename OverloadSelectionT, size_t param_idx>
struct InputParamSelection;

// Represents an indexing into a specific class and method.
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx>
struct MethodSelection;

// Represents a specific overload selection.
template <typename MethodSelectionT, size_t overload_idx>
struct OverloadSelection;

// Helper to find overloads for a given arg set.
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
struct OverloadSelectionForArgsImpl;

////////////////////////////////////////////////////////////////////////////////
// Helper Aliases.
////////////////////////////////////////////////////////////////////////////////
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx>
using MethodSelection_t =
    MethodSelection<class_loader_v_, class_v_, is_constructor, method_idx>;

template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
using MethodSelectionForArgs_t =
    OverloadSelectionForArgsImpl<class_loader_v_, class_v_, is_constructor,
                                 method_idx, Args...>;

////////////////////////////////////////////////////////////////////////////////
// Implementation Details.
////////////////////////////////////////////////////////////////////////////////
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx>
struct MethodSelection {
  static constexpr bool kIsConstructor = is_constructor;

  static constexpr const auto& GetClass() { return class_v_; }
  static constexpr const auto& GetClassLoader() { return class_loader_v_; }

  static constexpr const auto& GetMethod() {
    if constexpr (is_constructor) {
      static_assert(method_idx == 0,
                    "If using MethodSelection for a constructor, there is only "
                    "ever one method (\"<init>\"), set method_idx to 0.");
      return class_v_.constructors_;
    } else {
      return std::get<method_idx>(class_v_.methods_);
    }
  }

  static constexpr const auto& Name() {
    if constexpr (is_constructor) {
      return "<init>";
    } else {
      return std::get<method_idx>(class_v_.methods_).name_;
    }
  }

  static constexpr std::size_t NumOverloads() {
    if constexpr (is_constructor) {
      return std::tuple_size<decltype(class_v_.constructors_)>();
    } else {
      return std::tuple_size<decltype(GetMethod().invocations_)>();
    }
  }

  template <typename Is, typename... Ts>
  struct Helper;

  template <size_t... Is, typename... Ts>
  struct Helper<std::index_sequence<Is...>, Ts...> {
    static constexpr bool val =
        (OverloadSelection<MethodSelection, Is>::template OverloadViable<Ts...>() ||
         ...);

    // kNoSelection is the max of std::size_t, so, this essentially selects any
    // idx (if a valid one exists), or defaults to kNoSelection.
    static constexpr std::size_t overload_idx_if_valid{std::min(
        {OverloadSelection<MethodSelection,
                           Is>::template OverloadIdxIfViable<Ts...>()...})};
  };

  template <typename... Ts>
  static constexpr bool ArgSetViable() {
    return Helper<std::make_index_sequence<NumOverloads()>,
                  std::decay_t<Ts>...>::val;
  }

  // The overload that is viable for a set of args, or |kNoSelection|.
  template <typename... Ts>
  static constexpr std::size_t IdxForArgs() {
    return Helper<std::make_index_sequence<NumOverloads()>,
                  std::decay_t<Ts>...>::overload_idx_if_valid;
  }

  template <typename... Ts>
  using FindOverloadSelection =
      OverloadSelection<MethodSelection, IdxForArgs<Ts...>()>;
};

template <typename OverloadSelectionT>
struct ArgumentValidate {
  template <bool kSameSize, typename T, typename... Ts>
  struct Helper {
    static constexpr bool kValid = false;
  };

  template <std::size_t... Is, typename... Ts>
  struct Helper<true, std::index_sequence<Is...>, Ts...> {
    static constexpr bool kValid =
        (InputParamSelection<OverloadSelectionT, Is>::template kValid<Ts...> &&
         ...);
  };

  template <typename... Ts>
  static constexpr bool kValid =
      Helper<(sizeof...(Ts) == OverloadSelectionT::kNumParams),
             std::make_index_sequence<sizeof...(Ts)>, Ts...>::kValid;
};

template <typename MethodSelectionT, size_t overload_idx>
struct OverloadSelection {
  static constexpr Return kObjectWhenConstructed{
      Class{MethodSelectionT::GetClass().name_}};

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
    return OverloadViable<Ts...>() ? overload_idx : kNoSelection;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Static Signature Generation.
  ////////////////////////////////////////////////////////////////////////////////
  template <typename Is>
  struct ParamHelper;

  template <>
  struct ParamHelper<std::index_sequence<>> {
    static constexpr std::string_view val = "";
  };

  template <size_t... Is>
  struct ParamHelper<std::index_sequence<Is...>> {
    static constexpr std::string_view val =
        metaprogramming::StringConcatenate_v<SelectorStaticInfo<
            InputParamSelection<OverloadSelection, Is>>::kTypeName...>;
  };

  static constexpr std::string_view kParamSignature =
      metaprogramming::StringConcatenate_v<
          kLeftParenthesis,
          ParamHelper<std::make_index_sequence<kNumParams>>::val,
          kRightParenthesis>;

  static constexpr std::string_view GetReturnSignature() {
    if constexpr (MethodSelectionT::kIsConstructor) {
      return "V";
    } else {
      return SelectorStaticInfo<
          InputParamSelection<OverloadSelection, kIsReturnIdx>>::kTypeName;
    }
  }

  static constexpr std::string_view kReturnSignature = GetReturnSignature();

  static constexpr std::string_view GetOverloadSignature() {
    return metaprogramming::StringConcatenate_v<kParamSignature,
                                                kReturnSignature>;
  }
};

template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
struct OverloadSelectionForArgsImpl {
  using MethodSelectionForArgs =
      MethodSelection_t<class_loader_v_, class_v_, is_constructor, method_idx>;
  using OverloadSelectionForArgs =
      typename MethodSelectionForArgs::template FindOverloadSelection<Args...>;
  using OverloadRef =
      OverloadRef<MethodSelectionForArgs, OverloadSelectionForArgs>;

  static constexpr bool kIsValidArgSet =
      MethodSelectionForArgs::template ArgSetViable<Args...>();
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_SELECTION_H_
