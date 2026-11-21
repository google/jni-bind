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

#ifndef JNI_BIND_IMPLEMENTATION_MESSAGE_REF_H_
#define JNI_BIND_IMPLEMENTATION_MESSAGE_REF_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

#include "bind_lang/id_type.h"
#include "bind_lang/lifecycle.h"
#include "bind_lang/metaprogramming/queryable_map.h"
#include "bind_lang/metaprogramming/queryable_map_20.h"
#include "bind_lang/metaprogramming/string_literal.h"
#include "bind_lang/no_idx.h"
#include "bind_lang/ref_base.h"
#include "implementation/descriptor_ref.h"
#include "implementation/proto_field_ref.h"
#include "implementation/proto_helper/lifecycle_proto.h"
#include "implementation/proto_id.h"
#include "proto_dep.h"

namespace proto {

// Represents a runtime instance of a Protocol Buffer message bound to a
// compile-time `ProtoT` schema. Mirrors `jni::ObjectRef` in
// `implementation/object_ref.h` and reuses `QueryableMap_t`, `QueryableMap20`,
// and `RefBase`.
template <typename ProtoT_>
class MessageRef
    : public bind_lang::metaprogramming::QueryableMap_t<
          MessageRef<ProtoT_>, ProtoT_::stripped_template_v,
          decltype(&ProtoT_::TemplateT::fields_), &ProtoT_::TemplateT::fields_>,
      public bind_lang::metaprogramming::QueryableMap20<
          MessageRef<ProtoT_>, ProtoT_::stripped_template_v,
          MessageRef<ProtoT_>, decltype(&ProtoT_::TemplateT::fields_),
          &ProtoT_::TemplateT::fields_>,
      public bind_lang::RefBase<typename ProtoT_::StorageType> {
 protected:
  using RefBaseT = bind_lang::RefBase<typename ProtoT_::StorageType>;
  using LifecycleT =
      bind_lang::LifecycleHelper<google::protobuf::Message*,
                                 bind_lang::LifecycleType::LOCAL>;

  MessageRef() = delete;
  MessageRef& operator=(const MessageRef& rhs) = delete;

  const google::protobuf::Descriptor* GetDescriptor() const {
    return DescriptorRef<ProtoT_>::GetAndMaybeLoadDescriptor();
  }

  template <std::size_t kTargetTag, std::size_t... Is>
  static constexpr std::size_t FindIndexForTagImpl(std::index_sequence<Is...>) {
    std::size_t found = bind_lang::kNoIdx;
    ((ProtoId<ProtoT_, bind_lang::IdType::FIELD, Is>::kTag == kTargetTag
          ? (void)(found = Is)
          : (void)0),
     ...);
    return found;
  }

  template <std::size_t kTargetTag>
  static constexpr std::size_t FindIndexForTag() {
    return FindIndexForTagImpl<kTargetTag>(
        std::make_index_sequence<ProtoT_::kNumFields>{});
  }

 public:
  MessageRef(std::nullptr_t) : RefBaseT(nullptr) {}
  explicit MessageRef(typename ProtoT_::StorageType msg) : RefBaseT(msg) {}
  explicit MessageRef(RefBaseT&& rhs) : RefBaseT(std::move(rhs)) {}

#if __clang__
  // Invoked through CRTP from `bind_lang::metaprogramming::QueryableMap`.
  // Enables `msg["field_name"].Get()` and `msg["field_name"].Set(...)`.
  template <std::size_t I>
  auto QueryableMapCall(const char* key) const {
    return ProtoFieldRef<ProtoT_, I>{GetDescriptor(), RefBaseT::object_ref_};
  }
#endif  // __clang__

#if __cplusplus >= 202002L
  // Invoked through CRTP from `bind_lang::metaprogramming::QueryableMap20`.
  // Enables `msg.Access<"field_name">().Get()`.
  template <std::size_t I,
            bind_lang::metaprogramming::StringLiteral key_literal>
  auto QueryableMap20Call() const {
    return ProtoFieldRef<ProtoT_, I>{GetDescriptor(), RefBaseT::object_ref_};
  }
#endif  // __cplusplus >= 202002L

  // Accesses a field by its 0-based declaration index in `proto::Template`.
  template <std::size_t I>
  auto AccessAtIdx() const {
    static_assert(I < ProtoT_::kNumFields,
                  "Field index out of bounds for proto::Template.");
    return ProtoFieldRef<ProtoT_, I>{GetDescriptor(), RefBaseT::object_ref_};
  }

  template <std::size_t I>
  auto field_at() const {
    return AccessAtIdx<I>();
  }

  // Accesses a field by its compile-time tag number (`kTag`).
  template <std::size_t kTargetTag>
  auto AccessByTag() const {
    constexpr std::size_t kIdx = FindIndexForTag<kTargetTag>();
    static_assert(kIdx != bind_lang::kNoIdx,
                  "No field with the specified tag exists in proto::Template.");
    return ProtoFieldRef<ProtoT_, kIdx>{GetDescriptor(), RefBaseT::object_ref_};
  }

  template <std::size_t kTargetTag>
  auto field() const {
    return AccessByTag<kTargetTag>();
  }

  google::protobuf::Message* RawMessage() const {
    return RefBaseT::object_ref_;
  }

  std::string SerializeAsString() const {
    return LifecycleT::SerializeAsString(RefBaseT::object_ref_);
  }

  bool ParseFromString(std::string_view serialized) {
    return LifecycleT::ParseFromString(RefBaseT::object_ref_, serialized);
  }
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_MESSAGE_REF_H_
