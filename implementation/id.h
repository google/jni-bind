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
  CONSTRUCTOR_SET,
  CONSTRUCTOR,
  CONSTRUCTOR_PARAM,
  FIELD,
  OVERLOAD_SET,
  OVERLOAD,
  OVERLOAD_PARAM,
};

static constexpr std::size_t kNoIdxSpecified{
    std::numeric_limits<std::size_t>::max()};

template <typename JniType, const auto& root, IdType kIdType,
          std::size_t idx = kNoIdxSpecified,
          std::size_t secondary_idx = kNoIdxSpecified,
          std::size_t tertiary_idx = kNoIdxSpecified>
struct Id {
  static constexpr const auto& Val() {
    if constexpr (kIdType == IdType::CLASS) {
      return root;
    } else if constexpr (kIdType == IdType::CONSTRUCTOR) {
      return std::get<idx>(root.constructors_);
    } else if constexpr (kIdType == IdType::CONSTRUCTOR_SET) {
      return root.constructors_;
    } else if constexpr (kIdType == IdType::CONSTRUCTOR_PARAM) {
      return std::get<secondary_idx>(
          std::get<idx>(root.constructors_).params_.values_);
    } else if constexpr (kIdType == IdType::FIELD) {
      return std::get<idx>(root.fields_);
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      return std::get<idx>(root.methods_);
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      return std::get<secondary_idx>(std::get<idx>(root.methods_).invocations_);
    } else if constexpr (kIdType == IdType::OVERLOAD_PARAM) {
      if constexpr (tertiary_idx == kNoIdxSpecified) {
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

  using RawValT = std::decay_t<decltype(Val())>;
  static constexpr std::size_t kRank = Rankifier<RawValT>::Rank(Val());

  static constexpr const char* Name() {
    if constexpr (kIdType == IdType::CONSTRUCTOR) {
      return "<init>";
    } else {
      return Val().name_;
    }
  }

  static constexpr std::size_t NumParams() {
    if constexpr (kIdType == IdType::CONSTRUCTOR) {
      static_assert(idx != kNoIdxSpecified &&
                    secondary_idx == kNoIdxSpecified &&
                    tertiary_idx == kNoIdxSpecified);
      return std::tuple_size_v<decltype(Val().params_.values_)>;
    } else if (kIdType == IdType::OVERLOAD) {
      static_assert(idx != kNoIdxSpecified &&
                    secondary_idx != kNoIdxSpecified &&
                    tertiary_idx == kNoIdxSpecified);
      return std::tuple_size_v<decltype(Val().params_.values_)>;
    }

    // Other types don't have meaningful use of this.
    return 1;
  }

  // Represents the IdType for the natural sub-object (if any), else CLASS.
  static constexpr IdType kChildIdType =
      kIdType == IdType::CONSTRUCTOR
          ? IdType::CONSTRUCTOR_PARAM
          : (kIdType == IdType::OVERLOAD ? IdType::OVERLOAD_PARAM
                                         : IdType::CLASS);

  // This can be further generalised to accept a container which will be a
  // useful refactor in the future.
  // Note, in order to represent void, the |NumParams()|+1 is passed.
  template <typename IdxPack, std::size_t iterator_idx_>
  struct Helper;

  template <std::size_t... Is, std::size_t iterator_idx_>
  struct Helper<std::index_sequence<Is...>, iterator_idx_> {
    // All parameters from 2 onward (note the leading ",").
    template <std::size_t I>
    struct Val {
      static constexpr std::string_view kVal =
          Id<JniType, root, kChildIdType, (iterator_idx_ == 0 ? (I - 1) : idx),
             (iterator_idx_ == 1 ? (I - 1) : secondary_idx),
             (iterator_idx_ == 2 ? (I - 1) : tertiary_idx)>::Signature();
    };

    // First paramater (if one exists).  Note, no leading comma.
    template <>
    struct Val<1> {
      static constexpr std::string_view kVal =
          Id<JniType, root, kChildIdType, (iterator_idx_ == 0 ? 0 : idx),
             (iterator_idx_ == 1 ? 0 : secondary_idx),
             (iterator_idx_ == 2 ? 0 : tertiary_idx)>::Signature();
    };

    // Void function (note, this will be mutually exclusive with the above).
    template <>
    struct Val<0> {
      static constexpr std::string_view kVal = "";
    };

    static constexpr std::string_view kVal =
        metaprogramming::StringConcatenate_v<Val<Is>::kVal...>;
  };

  struct ReturnHelper {
    static constexpr std::string_view kVal =
        Id<JniType, root, kChildIdType, idx, secondary_idx,
           kNoIdxSpecified>::Signature();
  };

  // Generates the body of the signature for methods and constructors.
  static constexpr std::string_view SignatureBodyHelper() {
    using Idxs = std::make_index_sequence<NumParams() + 1>;

    if constexpr (kIdType == IdType::CONSTRUCTOR) {
      return metaprogramming::StringConcatenate_v<
          kLeftParenthesis, Helper<Idxs, 1>::kVal, kRightParenthesis, kLetterV>;
    } else {
      return metaprogramming::StringConcatenate_v<
          kLeftParenthesis, Helper<Idxs, 2>::kVal, kRightParenthesis,
          ReturnHelper::kVal>;
    }
  }

  // For methods and ctors generates the signature, e.g. "(II)LClass1;".
  // For parameters, emits just a type name.
  static constexpr std::string_view Signature() {
    if constexpr (kIdType == IdType::CONSTRUCTOR) {
      return SignatureBodyHelper();
    } else if constexpr (kIdType == IdType::CONSTRUCTOR_PARAM) {
      return SelectorStaticInfo<Id>::TypeName();
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      return SignatureBodyHelper();
    } else if constexpr (kIdType == IdType::OVERLOAD_PARAM) {
      return SelectorStaticInfo<Id>::TypeName();
    }

    return "NOT_IMPLEMENTED";
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ID_H_
