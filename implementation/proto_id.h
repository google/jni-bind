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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_ID_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_ID_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <cstddef>
#include <string_view>
#include <type_traits>

#include "bind_lang/id_type.h"
#include "bind_lang/no_idx.h"
#include "implementation/proto_field.h"
#include "implementation/proto_template.h"
#include "implementation/proxy_definitions_proto.h"

namespace proto {

// Compile-time field/template selector for `ProtoT`.
// Mirrors `jni::Id` in `implementation/id.h`.
template <typename ProtoT_, bind_lang::IdType kIdType_,
          std::size_t idx = bind_lang::kNoIdx>
struct ProtoId {
  static_assert(idx != bind_lang::kNoIdx,
                "Field ProtoId requires a valid field index.");
  using _ProtoT = ProtoT_;
  static constexpr bind_lang::IdType kIdType = kIdType_;
  static constexpr std::size_t kIdx = idx;

  static constexpr auto GetTemplate() { return _ProtoT::GetTemplate(); }

  static constexpr auto FieldSpec() {
    if constexpr (idx != bind_lang::kNoIdx) {
      return std::get<idx>(GetTemplate().fields_);
    } else {
      return ProtoFieldSpec<0, FieldLabel::OPTIONAL, int, "">{};
    }
  }

  using FieldSpecT = std::decay_t<decltype(FieldSpec())>;
  using RawValT = typename FieldSpecT::Raw;

  static constexpr std::size_t kTag = FieldSpecT::kTag;
  static constexpr FieldLabel kLabel = FieldSpecT::kLabel;
  static constexpr bool kIsOptional = (kLabel == FieldLabel::OPTIONAL);
  static constexpr bool kIsRequired = (kLabel == FieldLabel::REQUIRED);
  static constexpr bool kIsRepeated = (kLabel == FieldLabel::REPEATED);
  static constexpr std::size_t kRank = kIsRepeated ? 1 : 0;

  static constexpr auto Val() {
    if constexpr (IsSubMessage_v<RawValT>) {
      return RawValT::kTemplate;
    } else {
      return FieldSpec().raw_;
    }
  }

  using MaterializeCDeclT = ProtoCDecl_t<RawValT>;

  static constexpr const char* Name() { return FieldSpec().name_; }

  static constexpr std::string_view kName = Name();
};

template <typename ProtoT_, std::size_t idx>
struct ProtoId<ProtoT_, bind_lang::IdType::CLASS, idx> {
  using _ProtoT = ProtoT_;
  static constexpr bind_lang::IdType kIdType = bind_lang::IdType::CLASS;
  static constexpr std::size_t kIdx = idx;

  static constexpr auto GetTemplate() { return _ProtoT::GetTemplate(); }

  static constexpr auto Val() { return GetTemplate(); }

  static constexpr const char* Name() { return GetTemplate().name_; }

  static constexpr std::string_view kName = Name();
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_ID_H_
