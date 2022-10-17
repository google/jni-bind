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

#ifndef JNI_BIND_IMPLEMENTATION_ID_H_
#define JNI_BIND_IMPLEMENTATION_ID_H_

#include <cstddef>
#include <limits>
#include <string_view>
#include <utility>

#include "implementation/array.h"
#include "implementation/class.h"
#include "implementation/field.h"
#include "implementation/name_constants.h"
#include "implementation/no_idx.h"
#include "implementation/proxy_convenience_aliases.h"
#include "implementation/selector_static_info.h"

namespace jni {

enum class IdType {
  CLASS,
  FIELD,
  OVERLOAD_SET,
  OVERLOAD,
  OVERLOAD_PARAM,
};

template <typename JniType_, IdType kIdType_, std::size_t idx = kNoIdx,
          std::size_t secondary_idx = kNoIdx, std::size_t tertiary_idx = kNoIdx>
struct Id {
  using JniType = JniType_;
  static constexpr IdType kIdType = kIdType_;
  static constexpr auto& root = JniType::GetClass();
  static constexpr std::size_t kIdx = idx;
  static constexpr std::size_t kSecondaryIdx = secondary_idx;
  static constexpr std::size_t kTertiaryIdx = tertiary_idx;

  static constexpr bool kIsConstructor =
      (kIdType == IdType::OVERLOAD || kIdType == IdType::OVERLOAD_PARAM ||
       kIdType == IdType::OVERLOAD_SET) &&
      (kIdx == kNoIdx);

  template <IdType new_id_type>
  using ChangeIdType =
      Id<JniType, new_id_type, idx, secondary_idx, tertiary_idx>;

  template <std::size_t kIdxToChange, std::size_t kNewValue>
  using ChangeIdx = Id<JniType, kIdType, (kIdxToChange == 0 ? kNewValue : idx),
                       (kIdxToChange == 1 ? kNewValue : secondary_idx),
                       (kIdxToChange == 2 ? kNewValue : tertiary_idx)>;

  static constexpr const auto& Val() {
    if constexpr (kIdType == IdType::CLASS) {
      return root;
    } else if constexpr (kIdType == IdType::FIELD) {
      static_assert(idx != kNoIdx);
      return std::get<idx>(root.fields_).raw_;
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        return root.constructors_;
      } else {
        // Overload.
        return std::get<idx>(root.methods_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        return std::get<secondary_idx>(root.constructors_);
      } else {
        // Overload.
        return std::get<secondary_idx>(
            std::get<idx>(root.methods_).invocations_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD_PARAM) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        if constexpr (tertiary_idx == kNoIdx) {
          // Return.
          return root;
        } else {
          // Overload return.
          return std::get<tertiary_idx>(
              std::get<secondary_idx>(root.constructors_).params_.values_);
        }
      } else {
        // Overload.
        if constexpr (tertiary_idx == kNoIdx) {
          // Return.
          return std::get<secondary_idx>(
                     std::get<idx>(root.methods_).invocations_)
              .return_.raw_;
        } else {
          return std::get<tertiary_idx>(
              std::get<secondary_idx>(std::get<idx>(root.methods_).invocations_)
                  .params_.values_);
        }
      }
    }
  }

  // Returns root for constructor, else return's "raw_" member.
  static constexpr auto Materialize() {
    if constexpr (kIdType == IdType::OVERLOAD) {
      if constexpr (kIdx == kNoIdx) {
        // Constructor.
        return root;
      } else {
        // Overload.
        return std::get<secondary_idx>(
                   std::get<idx>(root.methods_).invocations_)
            .return_.raw_;
      }
    } else {
      // Not implemented.
      return Void{};
    }
  }

  using MaterializeT = std::decay_t<decltype(Materialize())>;
  using RawValT = ArrayStrip_t<std::decay_t<decltype(Val())>>;
  using UnstrippedRawVal = std::decay_t<decltype(Val())>;
  using CDecl = CDecl_t<VoidIfVoid_t<MaterializeT>>;

  static constexpr std::size_t kRank = Rankifier<RawValT>::Rank(Val());
  static constexpr Return kObjectWhenConstructed{
      Class{JniType::GetClass().name_}};

  static constexpr const char* Name() {
    if constexpr (kIdType == IdType::OVERLOAD_SET && idx == kNoIdx) {
      return "<init>";
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      return Val().name_;
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      return Id<JniType, IdType::OVERLOAD_SET, idx, secondary_idx>::Name();
    } else if constexpr (kIdType == IdType::FIELD) {
      return std::get<idx>(root.fields_).name_;
    } else {
      return "NO_NAME";
    }
  }

  static constexpr std::size_t NumParams() {
    if constexpr (kIdType == IdType::OVERLOAD) {
      return std::tuple_size_v<decltype(Val().params_.values_)>;
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      if constexpr (idx == kNoIdx) {
        return std::tuple_size_v<std::decay_t<decltype(Val())>>;
      } else {
        return std::tuple_size_v<std::decay_t<decltype(Val().invocations_)>>;
      }
    } else {
      // Other types don't have meaningful use of this.
      return 1;
    }
  }

  template <typename IdxPack>
  struct Helper;

  template <std::size_t... Is>
  struct Helper<std::index_sequence<Is...>> {
    template <std::size_t I>
    struct Val {
      static constexpr std::string_view kVal =
          Id<JniType, IdType::OVERLOAD_PARAM, idx, secondary_idx,
             I>::Signature();
    };

    static constexpr std::string_view kVal =
        metaprogramming::StringConcatenate_v<Val<Is>::kVal...>;
  };

  struct ReturnHelper {
    static constexpr std::string_view kVal =
        Id<JniType, IdType::OVERLOAD_PARAM, idx, secondary_idx>::Signature();
  };

  // For methods and ctors generates the signature, e.g. "(II)LClass1;".
  // For parameters, emits just a type name.
  static constexpr std::string_view Signature() {
    if constexpr (kIdType == IdType::FIELD) {
      return SelectorStaticInfo<Id>::TypeName();
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      return "NOT_IMPLEMENTED";
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      using Idxs = std::make_index_sequence<NumParams()>;
      if constexpr (kIsConstructor) {
        return metaprogramming::StringConcatenate_v<
            kLeftParenthesis, Helper<Idxs>::kVal, kRightParenthesis, kLetterV>;
      } else {
        return metaprogramming::StringConcatenate_v<
            kLeftParenthesis, Helper<Idxs>::kVal, kRightParenthesis,
            ReturnHelper::kVal>;
      }
    } else if constexpr (kIdType == IdType::OVERLOAD_PARAM) {
      return SelectorStaticInfo<Id>::TypeName();
    }

    return "NOT_IMPLEMENTED";
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ID_H_
