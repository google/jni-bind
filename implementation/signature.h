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
#ifndef JNI_BIND_IMPLEMENTATION_SIGNATURE_H_
#define JNI_BIND_IMPLEMENTATION_SIGNATURE_H_

#include <type_traits>

#include "implementation/array.h"
#include "implementation/class.h"
#include "implementation/field.h"
#include "implementation/id_type.h"
#include "implementation/name_constants.h"
#include "implementation/no_idx.h"
#include "implementation/proxy_convenience_aliases.h"
#include "implementation/selector_static_info.h"

namespace jni {

template <typename JniType_, IdType kIdType_, std::size_t idx,
          std::size_t secondary_idx, std::size_t tertiary_idx>
struct Id;

// Takes an Id and produces its signature.
template <typename>
struct Signature {};

template <typename JniType_, IdType kIdType_, std::size_t idx,
          std::size_t secondary_idx, std::size_t tertiary_idx>
struct Signature<Id<JniType_, kIdType_, idx, secondary_idx, tertiary_idx>> {
  using IdT = Id<JniType_, kIdType_, idx, secondary_idx, tertiary_idx>;

  template <typename IdxPack>
  struct Helper;

  template <std::size_t... Is>
  struct Helper<std::index_sequence<Is...>> {
    template <std::size_t I>
    struct Val {
      static constexpr std::string_view val = Signature<
          Id<JniType_, IdType::OVERLOAD_PARAM, idx, secondary_idx, I>>::val;
    };

    static constexpr std::string_view val =
        metaprogramming::StringConcatenate_v<Val<Is>::val...>;
  };

  struct ReturnHelper {
    static constexpr std::string_view val = Signature<
        Id<JniType_, IdType::OVERLOAD_PARAM, idx, secondary_idx, kNoIdx>>::val;
  };

  // For methods and ctors generates the signature, e.g. "(II)LClass1;".
  // For parameters, emits just a type name.
  static constexpr std::string_view Val() {
    if constexpr (kIdType_ == IdType::FIELD) {
      return SelectorStaticInfo<IdT>::TypeName();
    } else if constexpr (kIdType_ == IdType::OVERLOAD_SET) {
      return "NOT_IMPLEMENTED";
    } else if constexpr (kIdType_ == IdType::OVERLOAD) {
      using Idxs = std::make_index_sequence<IdT::NumParams()>;
      if constexpr (IdT::kIsConstructor) {
        return metaprogramming::StringConcatenate_v<
            kLeftParenthesis, Helper<Idxs>::val, kRightParenthesis, kLetterV>;
      } else {
        return metaprogramming::StringConcatenate_v<
            kLeftParenthesis, Helper<Idxs>::val, kRightParenthesis,
            ReturnHelper::val>;
      }
    } else if constexpr (kIdType_ == IdType::OVERLOAD_PARAM) {
      return SelectorStaticInfo<IdT>::TypeName();
    }

    return "NOT_IMPLEMENTED";
  }

  static constexpr std::string_view val{Val()};
};

template <typename T>
static constexpr auto Signature_v = Signature<T>::val;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_SIGNATURE_H_
