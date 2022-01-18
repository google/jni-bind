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

#ifndef JNI_BIND_FIELD_SELECTION_H_
#define JNI_BIND_FIELD_SELECTION_H_

#include "implementation/jni_helper/jni_typename_to_string.h"
#include "implementation/name_constants.h"
#include "implementation/object.h"
#include "metaprogramming/string_concatenate.h"

namespace jni {

template <const auto& class_loader_v_, const auto& class_v_, size_t field_idx_>
struct FieldSelection {
  static constexpr const auto& GetClass() { return class_v_; }
  static constexpr const auto& GetClassLoader() { return class_loader_v_; }
  static auto& GetField() { return std::get<field_idx_>(class_v_.fields_); }

  using FieldT = std::decay_t<decltype(GetField().value_raw_)>;

  static constexpr bool kIsObject = std::is_base_of_v<Object, FieldT>;

  static constexpr std::string_view NameOrNothingIfNotAnObject() {
    if constexpr (kIsObject) {
      return GetField().name_;
    } else {
      return "";
    }
  }

  static constexpr std::string_view kNameOrNothingIfNotAnObject =
      NameOrNothingIfNotAnObject();

  static constexpr std::string_view FieldTypeName() {
    if constexpr (kIsObject) {
      return metaprogramming::StringConcatenate_v<
          kLetterL, kNameOrNothingIfNotAnObject, kSemiColon>;
    } else {
      return JavaTypeToString<FieldT>();
    }
  }

  static constexpr std::string_view kName = FieldTypeName();

  static constexpr std::string_view GetSignature() { return kName; }
};

}  // namespace jni

#endif  // JNI_BIND_FIELD_SELECTION_H_
