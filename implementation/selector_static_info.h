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

#ifndef JNI_BIND_SELECTOR_STATIC_INFO_H_
#define JNI_BIND_SELECTOR_STATIC_INFO_H_

#include <string_view>

#include "implementation/array.h"
#include "implementation/id_type.h"
#include "implementation/jni_helper/jni_typename_to_string.h"
#include "implementation/name_constants.h"
#include "implementation/object.h"
#include "implementation/return.h"
#include "implementation/self.h"
#include "implementation/void.h"
#include "metaprogramming/name_constants.h"
#include "metaprogramming/repeat_string.h"
#include "metaprogramming/string_concatenate.h"

namespace jni {

template <bool useParent, typename T>
struct ParentIfSelf {
  using type = T;
};

template <typename T>
struct ParentIfSelf<true, T> {
  using type = typename T::template ChangeIdType<IdType::CLASS>;
};

// Helper to generate full signature information for a "selected" value, and
// possibly some container information.  Here, |Selector| is |MethodSelection|,
// |FieldSelection|, etc.
//
// Unfortunately, the type system does not permit passing subobjects, however,
// types can be used that represent a specific selection upon an object.  This
// consolidates all signature info.
//
// |Selector| a type alias |RawValT| (e.g. jint, Class{...}, etc...).
template <typename SelectorIn>
struct SelectorStaticInfo {
  static constexpr inline bool kIsSelf =
      std::is_same_v<Self, typename SelectorIn::RawValT>;
  using Selector = typename ParentIfSelf<kIsSelf, SelectorIn>::type;

  template <std::size_t I>
  struct IthRawTypeMember {
    template <typename T>
    static constexpr const auto& Val(const T& val) {
      return IthRawTypeMember<I - 1>::Val(val.raw_);
    }
  };

  template <>
  struct IthRawTypeMember<0> {
    template <typename T>
    static constexpr const auto& Val(const T& val) {
      return val;
    }
  };

  // Strangely, the compiler refuses to peer through Val and loses the
  // constexpr-ness (i.e std::decay_t<decltype(Val())>; is not a constant
  // expression).
  static constexpr inline auto Val() {
    if constexpr (Selector::kRank == 0) {
      return Selector::Val();
    } else {
      return Selector::Val().raw_;
    }
  }

  using RawValT = typename Selector::RawValT;

  static constexpr inline bool kIsObject = std::is_base_of_v<Object, RawValT>;
  static constexpr inline bool kIsVoid = std::is_same_v<Void, RawValT>;
  static constexpr std::size_t kRank = Selector::kRank;

  static constexpr std::string_view TypeNameOrNothingIfNotAnObject() {
    if constexpr (kIsObject) {
      return Val().name_;
    } else {
      return "";
    }
  }

  static constexpr std::string_view kTypeNameOrNothingIfNotAnObject =
      TypeNameOrNothingIfNotAnObject();

  static constexpr std::string_view kEmptyStr = "";
  static constexpr std::string_view kModifierStr =
      (kRank == 0) ? ""
                   : metaprogramming::RepeatString_v<
                         kRank, metaprogramming::Constants::left_bracket>;

  static constexpr std::string_view UndecoratedTypeName() {
    if constexpr (kIsObject) {
      return metaprogramming::StringConcatenate_v<
          metaprogramming::Constants::L, kTypeNameOrNothingIfNotAnObject,
          metaprogramming::Constants::semi_colon>;
    } else if constexpr (kIsSelf) {
      return metaprogramming::StringConcatenate_v<
          metaprogramming::Constants::L, kTypeNameOrNothingIfNotAnObject,
          metaprogramming::Constants::semi_colon>;
    } else if constexpr (kIsVoid) {
      return JavaTypeToString<void>();
    } else {
      return JavaTypeToString<RawValT>();
    }
  }

  static constexpr std::string_view kUndecoratedTypeName =
      UndecoratedTypeName();

  static constexpr std::string_view TypeName() {
    return metaprogramming::StringConcatenate_v<kModifierStr,
                                                kUndecoratedTypeName>;
  }

  static constexpr std::string_view kTypeName = TypeName();
};

}  // namespace jni

#endif  // JNI_BIND_SELECTOR_STATIC_INFO_H_
