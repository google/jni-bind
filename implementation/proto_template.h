/*
 * Copyright 2026 Google LLC
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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_TEMPLATE_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_TEMPLATE_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "implementation/proto_field.h"

namespace proto {

inline constexpr const char* kAnonymousTemplateName =
    "__PROTO_BIND__ANONYMOUS__";

// Base type for all `proto::Template` definitions. Mirrors `jni::Object`.
struct TemplateBase {
  const char* name_;

  constexpr explicit TemplateBase(const char* name) : name_(name) {}
};

// Type adapter that wraps a `static constexpr proto::Template` NTTP so it can
// be passed as the `typename Raw_` argument of `proto::Optional<kTag, Raw_>`,
// `proto::Required<kTag, Raw_>`, or `proto::Repeated<kTag, Raw_>`.
template <const auto& sub_template_v_>
struct SubMessage : public TemplateBase {
  static constexpr const auto& kTemplate = sub_template_v_;

  constexpr SubMessage() : TemplateBase(sub_template_v_.name_) {}
};

template <typename T>
struct IsSubMessage : std::false_type {};

template <const auto& sub_template_v_>
struct IsSubMessage<SubMessage<sub_template_v_>> : std::true_type {};

template <typename T>
inline constexpr bool IsSubMessage_v = IsSubMessage<std::decay_t<T>>::value;

// Compile-time schema definition for a Protocol Buffer message.
// Mirrors `jni::Class` in `implementation/class.h`.
//
// Supported declaration styles:
//
// 1. CTAD with anonymous message name:
//   static constexpr proto::Template kTemplate{
//       proto::Optional<0, int>{"some_optional_int_field"},
//       proto::Required<1, std::string>{"some_string_field"},
//   };
//
// 2. CTAD with explicit protobuf message full name:
//   static constexpr proto::Template kTemplate{
//       "package.MyMessage",
//       proto::Optional<1, int>{"some_optional_int_field"},
//       proto::Required<2, std::string>{"some_string_field"},
//   };
//
// 3. Explicit template parameters with field names in constructor or NTTP:
//   static constexpr proto::Template<
//       proto::Optional<0, int>,
//       proto::Required<1, std::string>
//   > kTemplate{"some_optional_int_field", "some_string_field"};
template <typename... Fields_>
constexpr bool AreFieldTagsUnique() {
  if constexpr (sizeof...(Fields_) <= 1) {
    return true;
  } else {
    constexpr std::size_t tags[] = {Fields_::kTag...};
    for (std::size_t i = 0; i < sizeof...(Fields_); ++i) {
      for (std::size_t j = i + 1; j < sizeof...(Fields_); ++j) {
        if (tags[i] == tags[j]) return false;
      }
    }
    return true;
  }
}

template <typename... Fields_>
struct Template : public TemplateBase {
  static_assert((std::is_base_of_v<ProtoFieldBase, Fields_> && ...),
                "All elements of proto::Template must be proto field specs "
                "(Optional, Required, or Repeated).");
  static_assert(AreFieldTagsUnique<Fields_...>(),
                "All fields in a proto::Template must have unique tags.");

  const std::tuple<Fields_...> fields_;

  constexpr Template() : TemplateBase(kAnonymousTemplateName), fields_() {}

  template <std::size_t N = sizeof...(Fields_),
            typename = std::enable_if_t<(N > 0)>>
  constexpr explicit Template(Fields_... fields)
      : TemplateBase(kAnonymousTemplateName), fields_(fields...) {}

  constexpr explicit Template(const char* name, Fields_... fields)
      : TemplateBase(name), fields_(fields...) {}

  template <
      typename... Names,
      typename = std::enable_if_t<
          (sizeof...(Names) > 0) && (sizeof...(Names) == sizeof...(Fields_)) &&
          !(std::is_base_of_v<ProtoFieldBase, std::decay_t<Names>> || ...) &&
          (std::is_convertible_v<Names, const char*> && ...)>>
  constexpr explicit Template(Names... names)
      : TemplateBase(kAnonymousTemplateName), fields_(Fields_{names}...) {}

  template <
      typename... Names,
      typename = std::enable_if_t<
          (sizeof...(Names) > 0) && (sizeof...(Names) == sizeof...(Fields_)) &&
          !(std::is_base_of_v<ProtoFieldBase, std::decay_t<Names>> || ...) &&
          (std::is_convertible_v<Names, const char*> && ...)>,
      typename = void>
  constexpr explicit Template(const char* name, Names... names)
      : TemplateBase(name), fields_(Fields_{names}...) {}
};

template <typename LhsField, typename RhsField>
constexpr bool FieldRawEqual();

template <typename LhsField, typename RhsField>
constexpr bool SingleFieldEqual(const LhsField& lhs, const RhsField& rhs) {
  return LhsField::kTag == RhsField::kTag &&
         LhsField::kLabel == RhsField::kLabel &&
         FieldRawEqual<LhsField, RhsField>() &&
         std::string_view{lhs.name_} == std::string_view{rhs.name_};
}

template <typename LhsTuple, typename RhsTuple, std::size_t... Is>
constexpr bool TupleFieldsEqualImpl(const LhsTuple& lhs, const RhsTuple& rhs,
                                    std::index_sequence<Is...>) {
  return (SingleFieldEqual(std::get<Is>(lhs), std::get<Is>(rhs)) && ...);
}

template <typename... LhsFields, typename... RhsFields>
constexpr bool operator==(const Template<LhsFields...>& lhs,
                          const Template<RhsFields...>& rhs) {
  if (std::string_view{lhs.name_} != std::string_view{rhs.name_}) {
    return false;
  }
  if (std::string_view{lhs.name_} != std::string_view{kAnonymousTemplateName}) {
    return true;
  }
  if constexpr (sizeof...(LhsFields) != sizeof...(RhsFields)) {
    return false;
  } else {
    return TupleFieldsEqualImpl(
        lhs.fields_, rhs.fields_,
        std::make_index_sequence<sizeof...(LhsFields)>{});
  }
}

template <typename LhsField, typename RhsField>
constexpr bool FieldRawEqual() {
  if constexpr (IsSubMessage_v<typename LhsField::Raw> &&
                IsSubMessage_v<typename RhsField::Raw>) {
    return LhsField::Raw::kTemplate == RhsField::Raw::kTemplate;
  } else {
    return std::is_same_v<typename LhsField::Raw, typename RhsField::Raw>;
  }
}

template <typename... LhsFields, typename... RhsFields>
constexpr bool operator!=(const Template<LhsFields...>& lhs,
                          const Template<RhsFields...>& rhs) {
  return !(lhs == rhs);
}

////////////////////////////////////////////////////////////////////////////////
// CTAD.
////////////////////////////////////////////////////////////////////////////////
template <typename... Fields>
Template(Fields...) -> Template<Fields...>;

template <typename... Fields>
Template(const char*, Fields...) -> Template<Fields...>;

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_TEMPLATE_H_
