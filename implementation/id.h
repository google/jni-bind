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
#include "implementation/class_loader.h"
#include "implementation/field.h"
#include "implementation/id_type.h"
#include "implementation/name_constants.h"
#include "implementation/no_idx.h"
#include "implementation/proxy_convenience_aliases.h"
#include "implementation/selector_static_info.h"
#include "implementation/self.h"
#include "implementation/signature.h"
#include "metaprogramming/replace_string.h"

namespace jni {

template <typename JniT_, IdType kIdType_, std::size_t idx = kNoIdx,
          std::size_t secondary_idx = kNoIdx, std::size_t tertiary_idx = kNoIdx>
struct Id {
  using JniT = JniT_;
  static constexpr IdType kIdType = kIdType_;

  static constexpr auto Class() { return JniT::GetClass(); }

  static constexpr std::size_t kIdx = idx;
  static constexpr std::size_t kSecondaryIdx = secondary_idx;
  static constexpr std::size_t kTertiaryIdx = tertiary_idx;

  static constexpr bool kIsConstructor =
      (kIdType == IdType::OVERLOAD || kIdType == IdType::OVERLOAD_PARAM ||
       kIdType == IdType::OVERLOAD_SET) &&
      (kIdx == kNoIdx);

  static constexpr bool kIsStatic = kIdType == IdType::STATIC_OVERLOAD_SET ||
                                    kIdType == IdType::STATIC_OVERLOAD ||
                                    kIdType == IdType::STATIC_OVERLOAD_PARAM ||
                                    kIdType == IdType::STATIC_FIELD;

  template <IdType new_id_type>
  using ChangeIdType = Id<JniT, new_id_type, idx, secondary_idx, tertiary_idx>;

  template <std::size_t kIdxToChange, std::size_t kNewValue>
  using ChangeIdx = Id<JniT, kIdType, (kIdxToChange == 0 ? kNewValue : idx),
                       (kIdxToChange == 1 ? kNewValue : secondary_idx),
                       (kIdxToChange == 2 ? kNewValue : tertiary_idx)>;

  static constexpr auto Val() {
    if constexpr (kIdType == IdType::CLASS) {
      return Class();
    } else if constexpr (kIdType == IdType::STATIC_FIELD) {
      static_assert(idx != kNoIdx);
      return std::get<idx>(Class().static_.fields_).raw_;
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_SET) {
      // Overload (no such thing as static constructor).
      static_assert(idx != kNoIdx);
      return std::get<idx>(Class().static_.methods_);
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD) {
      // Overload (no such thing as static constructor).
      static_assert(idx != kNoIdx);
      return std::get<secondary_idx>(
          std::get<idx>(Class().static_.methods_).invocations_);
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_PARAM) {
      // Overload.
      if constexpr (tertiary_idx == kNoIdx) {
        // Return.
        return std::get<secondary_idx>(
                   std::get<idx>(Class().static_.methods_).invocations_)
            .return_.raw_;
      } else {
        return std::get<tertiary_idx>(
            std::get<secondary_idx>(
                std::get<idx>(Class().static_.methods_).invocations_)
                .params_.values_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        return Class().constructors_;
      } else {
        // Overload.
        return std::get<idx>(Class().methods_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        return std::get<secondary_idx>(Class().constructors_);
      } else {
        // Overload.
        return std::get<secondary_idx>(
            std::get<idx>(Class().methods_).invocations_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD_PARAM) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        if constexpr (tertiary_idx == kNoIdx) {
          // Return.
          return Class();
        } else {
          // Overload return.
          return std::get<tertiary_idx>(
              std::get<secondary_idx>(Class().constructors_).params_.values_);
        }
      } else {
        // Overload.
        if constexpr (tertiary_idx == kNoIdx) {
          // Return.
          return std::get<secondary_idx>(
                     std::get<idx>(Class().methods_).invocations_)
              .return_.raw_;
        } else {
          return std::get<tertiary_idx>(
              std::get<secondary_idx>(
                  std::get<idx>(Class().methods_).invocations_)
                  .params_.values_);
        }
      }
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_SET) {
      // Overload (no static_assert).
      static_assert(idx != kNoIdx);

      return std::get<secondary_idx>(
          std::get<idx>(Class().static_methods_).invocations_);
    } else if constexpr (kIdType == IdType::FIELD) {
      static_assert(idx != kNoIdx);
      return std::get<idx>(Class().fields_).raw_;
    }
  }

  // Returns root for constructor, else return's "raw_" member.
  static constexpr auto Materialize() {
    if constexpr (kIdType == IdType::STATIC_OVERLOAD) {
      return Val().return_.raw_;
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      if constexpr (kIdx == kNoIdx) {
        // Constructor.
        return Class();
      } else {
        // Overload return value.
        return Val().return_.raw_;
      }
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_SET ||
                         kIdType == IdType::OVERLOAD_SET) {
      // Not implemented.
      return Void{};
    } else {
      return Val();
    }
  }

  using MaterializeT = std::decay_t<decltype(Materialize())>;
  using RawMaterializeT = ArrayStrip_t<MaterializeT>;
  static constexpr std::size_t kMaterializedRank =
      Rankifier::Rank(Materialize());
  using MaterializeCDeclT = std::decay_t<VoidIfVoid_t<decltype(Materialize())>>;

  using RawValT = ArrayStrip_t<std::decay_t<decltype(Val())>>;
  using UnstrippedRawVal = std::decay_t<decltype(Val())>;
  using CDecl = CDecl_t<VoidIfVoid_t<MaterializeT>>;

  static constexpr std::size_t kRank = Rankifier::Rank(Val());
  static constexpr bool kIsSelf = std::is_same_v<RawValT, Self>;

  static constexpr const char* Name() {
    if constexpr (kIdType == IdType::CLASS) {
      return Class().name_;
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_SET) {
      return Val().name_;
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD) {
      return Id<JniT, IdType::STATIC_OVERLOAD_SET, idx, secondary_idx>::Name();
    } else if constexpr (kIdType == IdType::STATIC_FIELD) {
      return std::get<idx>(Class().static_.fields_).name_;
    } else if constexpr (kIdType == IdType::OVERLOAD_SET && idx == kNoIdx) {
      return "<init>";
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      return Val().name_;
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      return Id<JniT, IdType::OVERLOAD_SET, idx, secondary_idx>::Name();
    } else if constexpr (kIdType == IdType::FIELD) {
      return std::get<idx>(Class().fields_).name_;
    } else {
      return "NO_NAME";
    }
  }
  static constexpr std::string_view kName = Name();
  static constexpr std::string_view kNameUsingDots =
      metaprogramming::Replace_v<kName, '/', '.'>;

  static constexpr std::size_t NumParams() {
    if constexpr (kIdType == IdType::OVERLOAD ||
                  kIdType == IdType::STATIC_OVERLOAD) {
      return std::tuple_size_v<decltype(Val().params_.values_)>;
    } else if constexpr (kIdType == IdType::OVERLOAD_SET ||
                         kIdType == IdType::STATIC_OVERLOAD_SET) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        return std::tuple_size_v<std::decay_t<decltype(Val())>>;
      } else {
        return std::tuple_size_v<std::decay_t<decltype(Val().invocations_)>>;
      }
    } else {
      // Other types don't have meaningful use of this.
      return 1;
    }
  }

  static constexpr std::size_t kNumParams = NumParams();
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ID_H_
