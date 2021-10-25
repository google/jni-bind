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

#include "default_class_loader.h"
#include "method_ref.h"
#include "proxy.h"
#include "metaprogramming/concatenate.h"
#include "metaprogramming/invoke.h"
#include "metaprogramming/n_bit_sequence.h"
#include "metaprogramming/string_concatenate.h"
#include "metaprogramming/tuple_manipulation.h"
#include "metaprogramming/type_index_mask.h"
#include "metaprogramming/type_of_nth_element.h"

namespace jni {

static constexpr std::size_t kNoSelection = std::numeric_limits<size_t>::max();

// Represents an indexing into a specific class and method.
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx>
struct MethodSelection;

// Represents an overload which itself may be a set of permutations.
template <typename MethodSelectionT, size_t overload_idx>
struct OverloadSelection;

// Represents a permutation (e.g. jstring => {std::string, const char*, etc...}
template <typename MethodSelectionT, typename OverloadSelectionT, size_t permutation_idx>
struct Permutation;

// Represents the exact selection of a specific parameter from a permutation.
template <typename MethodSelectionT, typename OverloadSelectionT, typename PermutationT,
          size_t param_idx>
struct ParamSelection;

// The the type of an exact selection parameter in a method as part of the class
// specification (vs. the selection of a parameter in some generated candidate).
template <typename MethodSelectionT, typename OverloadSelectionT,
          size_t param_idx>
struct InputParamSelection;

// Compares a ParamSelection (the type associated with an exact parameter of an
// exact permutation) and exposes a value if they are equal.
template <typename ParamSelectionT, typename Query>
struct ParamCompare;

// The type correlating to the selection of a return type of a method.
template <typename MethodSelectionT, typename OverloadSelectionT>
struct ReturnSelection;

// Represents the exact permutation selection for a set of arguments.
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
struct PermutationSelectionForArgs;

////////////////////////////////////////////////////////////////////////////////
// Helper Aliases.
////////////////////////////////////////////////////////////////////////////////
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx>
using MethodSelection_t =
    MethodSelection<class_loader_v_, class_v_, is_constructor, method_idx>;

template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
using PermutationSelectionForArgs_t =
    PermutationSelectionForArgs<class_loader_v_, class_v_, is_constructor,
                                method_idx, Args...>;

////////////////////////////////////////////////////////////////////////////////
// Constants for signature generation.
////////////////////////////////////////////////////////////////////////////////

static constexpr std::string_view kLeftParenthesis{"("};
static constexpr std::string_view kRightParenthesis{")"};
static constexpr std::string_view kInit{"<init>"};
static constexpr std::string_view kComma{","};
static constexpr std::string_view kSemiColon{";"};
static constexpr std::string_view kLetterL{"L"};

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
    static constexpr std::pair<std::size_t, std::size_t>
        overload_permutation_idx_if_valid{
            std::min({OverloadSelection<MethodSelection,
                               Is>::template OverloadIdxIfViable<Ts...>()...}),
            std::min(
                {OverloadSelection<MethodSelection,
                          Is>::template PermutationIdxIfViable<Ts...>()...})};
  };

  template <typename... Ts>
  static constexpr bool ArgSetViable() {
    return Helper<std::make_index_sequence<NumOverloads()>,
                  std::decay_t<Ts>...>::val;
  }

  // The method and permutation overload that is viable for a set of args, or
  // {kNoSelection, kNoSelection} if no permutation is valid.
  template <typename... Ts>
  static constexpr std::pair<size_t, size_t> IdxPair() {
    return Helper<std::make_index_sequence<NumOverloads()>,
                  std::decay_t<Ts>...>::overload_permutation_idx_if_valid;
  }

  template <typename... Ts>
  using FindOverloadSelection = OverloadSelection<MethodSelection, IdxPair<Ts...>().first>;

  template <typename... Ts>
  using FindPermutation = OverloadSelection<MethodSelection, IdxPair<Ts...>().second>;
};

template <typename MethodSelectionT, typename OverloadSelectionT>
struct ReturnSelection {
  using ReturnT =
      ReturnRaw_t<std::decay_t<decltype(OverloadSelectionT::GetReturn())>>;

  static constexpr bool kIsObject = std::is_base_of_v<Object, ReturnT>;

  static constexpr std::string_view NameOrNothingIfNotAnObject() {
    if constexpr (kIsObject) {
      return OverloadSelectionT::GetReturn().return_raw_.name_;
    } else {
      return "";
    }
  }

  static constexpr std::string_view kTypeNameOrNothingIfNotAnObject =
      NameOrNothingIfNotAnObject();

  static constexpr std::string_view ReturnName() {
    if constexpr (kIsObject) {
      return metaprogramming::StringConcatenate_v<
          kLetterL, kTypeNameOrNothingIfNotAnObject, kSemiColon>;
    } else {
      return JavaTypeToString<ReturnT>();
    }
  }

  static constexpr std::string_view kName = ReturnName();
};

template <typename MethodSelectionT, typename OverloadSelectionT,
          size_t param_idx>
struct InputParamSelection {
  static constexpr const auto& GetParam() {
    return std::get<param_idx>(OverloadSelectionT::GetParams().values_);
  }

  using ParamT = std::decay_t<decltype(GetParam())>;

  static constexpr bool kIsObject = std::is_base_of_v<Object, ParamT>;

  static constexpr std::string_view NameOrNothingIfNotAnObject() {
    if constexpr (kIsObject) {
      return GetParam().name_;
    } else {
      return "";
    }
  }

  static constexpr std::string_view kTypeNameOrNothingIfNotAnObject =
      NameOrNothingIfNotAnObject();

  static inline constexpr std::string_view Signature() {
    if constexpr (kIsObject) {
      return metaprogramming::StringConcatenate_v<
          kLetterL, kTypeNameOrNothingIfNotAnObject, kSemiColon>;
    } else {
      return JavaTypeToString<ParamT>();
    }
  }

  static constexpr std::string_view val = Signature();
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

  using CDecl = CDecl_t<ReturnRaw_t<std::decay_t<decltype(GetReturn())>>>;
  using ReturnProxied = Return_t<CDecl, OverloadSelection>;

  // Proxy every parameter argument as an argument that can be shown in a
  // function prototype.
  //
  // A void function generates no arguments, so we generate an empty tuple.
  using ParamsRaw = typename std::decay_t<decltype(GetParams())>::ParamsRawTup;
  using ParamsProxiedTemp =
      metaprogramming::InvokePerTupArg_t<ProxyAsArgMetaFunc, ParamsRaw>;
  using ParamsProxied =
      std::conditional_t<std::is_same_v<ParamsProxiedTemp, std::tuple<>>,
                         std::tuple<std::tuple<>>, ParamsProxiedTemp>;

  // Lastly, we create a sequence to iterate all viable permutations.
  using NBitSequence = metaprogramming::TupleUnroller_t<
      metaprogramming::GenerateBitSequenceFromTupSetsFunc, ParamsProxied>;

  static constexpr const size_t permutation_count =
      (NBitSequence::max_representable_size_ == 0
           ? 1
           : NBitSequence::max_representable_size_);

  template <typename Is, typename... Ts>
  struct Helper;

  // Iterator for Permutation
  template <size_t... Is, typename... Ts>
  struct Helper<std::index_sequence<Is...>, Ts...> {
    static_assert(sizeof...(Is) == permutation_count);

    static constexpr bool val =
        (Permutation<MethodSelectionT, OverloadSelection,
                     Is>::template PermutationViable<Ts...>() ||
         ...);

    static constexpr size_t first_valid_permutation = std::min(
        {Permutation<MethodSelectionT, OverloadSelection,
                     Is>::template PermutationIdxIfViable<Ts...>()...});
  };

  static constexpr size_t kNumParams =
      std::tuple_size_v<decltype(GetParams().values_)>;

  template <typename... Ts>
  static constexpr size_t PermutationIdxIfViable() {
    constexpr size_t num_params =
        std::tuple_size_v<decltype(GetParams().values_)>;
    if constexpr (sizeof...(Ts) != num_params) {
      return kNoSelection;
    } else {
      return Helper<std::make_index_sequence<permutation_count>,
                    Ts...>::first_valid_permutation;
    }
  }

  template <typename... Ts>
  static constexpr bool OverloadViable() {
    return PermutationIdxIfViable<Ts...>() != kNoSelection;
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
        metaprogramming::StringConcatenate_v<InputParamSelection<
            MethodSelectionT, OverloadSelection, Is>::val...>;
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
      return ReturnSelection<MethodSelectionT, OverloadSelection>::ReturnName();
    }
  }

  static constexpr std::string_view kReturnSignature = GetReturnSignature();

  static constexpr std::string_view GetOverloadSignature() {
    return metaprogramming::StringConcatenate_v<kParamSignature,
                                                kReturnSignature>;
  }
};

template <typename MethodSelectionT, typename OverloadSelectionT, size_t permutation_idx>
struct Permutation {
  using NBitSelection =
      metaprogramming::Increment_t<typename OverloadSelectionT::NBitSequence,
                                   permutation_idx>;

  template <size_t I>
  using Param = ParamSelection<MethodSelectionT, OverloadSelectionT, Permutation, I>;

  template <typename Is, typename... Ts>
  struct Helper;

  template <size_t... Is, typename... Ts>
  struct Helper<std::index_sequence<Is...>, Ts...> {
    static constexpr bool val = (Param<Is>::template viable<Ts> && ...);
  };

  template <typename... Ts>
  static constexpr bool PermutationViable() {
    return Helper<std::index_sequence_for<Ts...>, Ts...>::val;
  }

  template <typename... Ts>
  static constexpr size_t PermutationIdxIfViable() {
    return PermutationViable<Ts...>() ? permutation_idx : kNoSelection;
  }
};

template <typename MethodSelectionT, typename OverloadSelectionT, typename PermutationT,
          size_t param_idx>
struct ParamSelection {
  static constexpr size_t selection_idx =
      PermutationT::NBitSelection::template GetBit<param_idx>::value_;

  constexpr static auto& GetParam() {
    return std::get<param_idx>(OverloadSelectionT::GetParams().values_);
  }

  using ParamT = metaprogramming::TypeOfNthTupleElement_t<
      selection_idx, metaprogramming::TypeOfNthTupleElement_t<
                         param_idx, typename OverloadSelectionT::ParamsProxied>>;

  template <typename T>
  static constexpr bool viable = ParamCompare<ParamSelection, T>::val;
};

template <typename ParamSelectionT, typename Query>
struct ParamCompare {
  using ParamT = typename ParamSelectionT::ParamT;

  template <typename, typename Enable = void>
  struct Helper {
    static constexpr bool val = false;
  };

  template <typename T>
  struct Helper<T, std::enable_if_t<std::is_same_v<std::remove_cv_t<T>,
                                                   std::remove_cv_t<ParamT>>>> {
    static constexpr bool val = true;
  };

  // The partial specialisation to compare a Local or Global object
  // against the specific selected permutation.
  template <template <const auto&, const auto&, const auto&> class T,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  struct Helper<
      T<class_v, class_loader_v, jvm_v>,
      std::enable_if_t<std::is_base_of_v<RefBaseTag<jobject>, ParamT> &&
                       std::is_base_of_v<RefBaseTag<jobject>,
                                         T<class_v, class_loader_v, jvm_v>>>> {
    // TODO(b/174272629): Exclude objects loaded by invalid loaders.
    static constexpr bool val =
        std::string_view(class_v.name_) ==
        std::string_view(ParamSelectionT::GetParam().name_);
  };

  static constexpr bool val = Helper<Query>::val;
};

template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
struct PermutationSelectionForArgs {
  using MethodSelectionForArgs =
      MethodSelection_t<class_loader_v_, class_v_, is_constructor, method_idx>;
  using OverloadSelectionForArgs =
      typename MethodSelectionForArgs::template FindOverloadSelection<Args...>;
  using PermutationForArgs =
      typename MethodSelectionForArgs::template FindPermutation<Args...>;

  static constexpr bool kIsValidArgSet =
      MethodSelectionForArgs::template ArgSetViable<Args...>;

  using PermutationRef = PermutationRef<MethodSelectionForArgs, OverloadSelectionForArgs,
                                        PermutationForArgs>;
};

}  // namespace jni

#endif  // JNI_BIND_METHOD_SELECTION_H_
