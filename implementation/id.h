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
#include "implementation/name_constants.h"
#include "implementation/selector_static_info.h"

namespace jni {

enum class IdType {
  CLASS,
  FIELD,
  OVERLOAD_SET,
  OVERLOAD,
  OVERLOAD_PARAM,
};

static constexpr std::size_t kNoIdxSpecified{
    std::numeric_limits<std::size_t>::max()};

template <typename JniType, IdType kIdType, std::size_t idx = kNoIdxSpecified,
          std::size_t secondary_idx = kNoIdxSpecified,
          std::size_t tertiary_idx = kNoIdxSpecified>
struct Id {
  static constexpr auto& root = JniType::GetClass();

  static constexpr const auto& Val() {
    if constexpr (kIdType == IdType::CLASS) {
      return root;
    } else if constexpr (kIdType == IdType::FIELD) {
      return std::get<idx>(root.fields_).raw_;
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      if constexpr (idx == kNoIdxSpecified) {
        return root.constructors_;
      } else {
        return std::get<idx>(root.methods_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      if constexpr (idx == kNoIdxSpecified) {
        // Constructor.
        return std::get<secondary_idx>(root.constructors_);
      } else {
        // Overload.
        return std::get<secondary_idx>(
            std::get<idx>(root.methods_).invocations_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD_PARAM) {
      if constexpr (idx == kNoIdxSpecified) {
        // Constructor.
        if constexpr (tertiary_idx == kNoIdxSpecified) {
          // Return.
          return Void{};
        } else {
          return std::get<tertiary_idx>(
              std::get<secondary_idx>(root.constructors_).params_.values_);
        }
      } else {
        // Overload.
        if constexpr (tertiary_idx == kNoIdxSpecified) {
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

  using RawValT = ArrayStrip_t<std::decay_t<decltype(Val())>>;

  static constexpr std::size_t kRank = Rankifier<RawValT>::Rank(Val());

  static constexpr const char* Name() {
    if constexpr (kIdType == IdType::OVERLOAD_SET && idx == kNoIdxSpecified) {
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
      return metaprogramming::StringConcatenate_v<
          kLeftParenthesis, Helper<Idxs>::kVal, kRightParenthesis,
          ReturnHelper::kVal>;
    } else if constexpr (kIdType == IdType::OVERLOAD_PARAM) {
      return SelectorStaticInfo<Id>::TypeName();
    }

    return "NOT_IMPLEMENTED";
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ID_H_
